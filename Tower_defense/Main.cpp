//#define GLM_FORCE_RADIANS

#include <Windows.h>
#include "GL\glew.h"
#include "GL\glut.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "Field.h"
#include "Camera.h"
#include "lodepng.h"
#include "NormalMob.h"
#include "FirstTurret.h"
#include "Arrow.h"
#include "models.h"

using namespace glm;

void init();
void createField(mat4 V, mat4 M);
void displayFrame();
void nextFrame();
void initializeGLUT(int* pargc, char **argv);
void initializeGLEW();
void specialKey(int key, int x, int y);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void mouseMovement(int x, int y);
void field();
void mapImage();
void turretImage();
void arrowImage();
void mobImage();
void game();
void menu();
void gameOver();
void createMob(mat4 V, mat4 M);
void deleteMob();
void MVP();
void drawMobOnField();
void drawTurretsOnField();
void changeLookAt();
void ghostBuild(mat4 V, mat4 M);
void changeGhostPosition();
void createSolidTurret();
void increaseLevel();
void selectMob();
bool checkPathRoute();
void loadModel(std::string path, BlenderModel& model);


GLuint minionFieldTex, buildFieldTex,mobTex,turretTex,arrowTex;
mat4 P, V, M;
FT_Library library;
FT_Face face;

enum GameState { MENU, GAME, GAME_OVER};
enum GamePhase { BUILD, MINION};
enum BuildPhase { GHOSTBUILD, GHOST, SOLID, UPGRADE};
enum KeyPressed { LEFTKEY, RIGHTKEY, NOTHING};

std::vector<NormalMob> mobAlive;
std::vector<FirstTurret> turret;
std::vector<Arrow> arrow;

int windowWidth = 800, windowHeight = 800;
int mobCount = 0, mobMaxCount = 10;
int maxMobAlive = 10, deathMobCount = 0;
int totalMobDied = 0;
int pressMouseX, pressMouseZ, mouseX, mouseZ;
int level = 1;
int gold = 200;
int lives = 50;
float income = 5;
const int turretCost = 50;
const int maxMobType = 1;
int mobType = 0;
bool start = true;
GLuint nexTex;
BlenderModel model;

GameState gamestate;
GamePhase gamephase = MINION;
BuildPhase buildphase = SOLID;
BuildPhase prevbuildphase = buildphase;
KeyPressed keypressed = NOTHING;
Camera camera;
NormalMob *normalMob;

int fieldTab[20][20];

struct Character
{
	GLuint textureID;
	vec2 size;
	vec2 bearing;
	GLuint advance;
};

std::map<GLchar, Character> Characters;
GLfloat texVertices[] = {
	0,0,	0, 1,	1, 1,	
	1,1,	1, 0,	0,0
};

void loadModel(std::string path, BlenderModel& model) {
	bool opened = BlenderModel::loadBlenderModerl(path, &model, true);
	if (opened)
		printf("Model: %s\n", model.toString().c_str());
	else
		printf("Not opened: %s\n", path.c_str());
}

void generateTexture()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte i = 0; i < 128; i++)
	{
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
			continue;
		}

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 
					 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(i, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(library);
}

void freeTypeLibraryInit()
{
	if (FT_Init_FreeType(&library))
		std::cout << "Error::FREETYPE: Could not init FreeType Library" << std::endl;
	else
		std::cout << "FreeType Library loaded properly" << std::endl;

	
	if (FT_New_Face(library, "GameData/fonts/arial.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	else
		std::cout << "Used font: arial.ttf" << std::endl;

	FT_Set_Pixel_Sizes(face, 0, 32);
}

void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, vec3 color)
{
	mat4 P = ortho(0.0f, static_cast<float> (windowWidth), 0.0f, static_cast<float> (windowHeight));
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(value_ptr(P));

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.bearing.x * scale;
		GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

		GLfloat w = ch.size.x * scale;
		GLfloat h = ch.size.y * scale;

		GLfloat vertices[6][3] = {
			{xpos, ypos + h, 0.0f},
			{xpos, ypos, 0.0f },
			{xpos + w, ypos, 0.0f},

			{ xpos + w, ypos, 0.0f },
			{ xpos + w, ypos + h, 0.0f },
			{ xpos, ypos + h, 0.0f }			
		};

		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glTexCoordPointer(2, GL_FLOAT, 0, texVertices);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		x += (ch.advance >> 6) * scale;
	}
}

void mobImage() {
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, "GameData/Plansza/scaryface.png");
	if (!error)
		std::cout << "Mob texture loaded properly\n";
	else
		std::cout << "Mob texture didnt loaded\n";
	glGenTextures(1, &mobTex);
	glBindTexture(GL_TEXTURE_2D, mobTex);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void turretImage() {
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, "GameData/Plansza/turretface.png");
	if (!error)
		std::cout << "Turret texture loaded properly\n";
	else
		std::cout << "Turret texture didnt loaded\n";
	glGenTextures(1, &turretTex);
	glBindTexture(GL_TEXTURE_2D, turretTex);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
}

void faq()
{
	std::string string;
	std::ifstream faq("GameData/Text/FAQ.txt");
	if (faq.good())
		while (!faq.eof())
		{
			getline(faq, string); 
			std::cout << string << std::endl;
		}
	else
		std::cout << "FAQ couldnt loaded.";
	std::cout << std::endl;
}

//field array
void field()
{
	std::ifstream field("GameData/Plansza/lvl.txt");
	if (field.good())
		std::cout << "Map file loaded properly" << std::endl;
	else
		std::cout << "Couldnt load map file" << std::endl;;

	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
		{
			field >> fieldTab[i][j];
		}

	field.close();
}

//increasing level
void increaseLevel()
{
	if (deathMobCount == maxMobAlive)
	{
		level++;
		income *= 1.1;
		
		camera.buildPhase = true;
		camera.changeCameraLookAt();
		gamephase = BUILD;
		deathMobCount = 0;
		mobAlive.clear();
		arrow.clear();

		maxMobAlive++;
		mobMaxCount++;
		selectMob();
	}
}

void selectMob()
{
	srand(time(NULL));
	mobType = rand() % maxMobType;
}

bool checkPathRoute()
{
	int x = (-pressMouseX + 20) / 2;
	int z = (pressMouseZ + 20) /2;
	if (fieldTab[z][x] == 2)
		return false;
	else
		return true;
}

//special keyboard keys
void specialKey(int key, int x, int y)
{
	//camera position change
	if (key == GLUT_KEY_UP)
	{
		camera.cameraUp();
	}
	if (key == GLUT_KEY_DOWN)
	{
		camera.cameraDown();
	}
	if (key == GLUT_KEY_LEFT)
	{
		camera.cameraLeft();
	}
	if (key == GLUT_KEY_RIGHT)
	{
		camera.cameraRight();
	}
	nextFrame();
}

//standard keyboard keys
void keyboard(unsigned char key, int x, int y)
{
	if (key == 'o' || key == 'O')
		camera.leftCameraRotation();
	else if (key == 'p' || key == 'P')
		camera.rightCameraRotation();

	if (key == 's' || key == 'S')
		camera.startPos();

	if (key == 'b' || key == 'B')
	{
		gamephase = BUILD;
		changeLookAt();
	}
	else if (key == 'm' || key == 'M')
	{
		gamephase = MINION;
		changeLookAt();
	}

	if (gamephase == BUILD && (key == 'f' || key == 'F'))
		buildphase = GHOSTBUILD;
	else if (gamephase == BUILD && (key == 'u' || key == 'U'))
		buildphase = UPGRADE;
	//quit when pushing esc button
	if (key == 27)
		exit(1);

	if (key == 'r' || key == 'R')
		gamestate = GAME;
	
	nextFrame();
}

//mouse clicking function
void mouse(int button, int state, int x, int z)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		keypressed = LEFTKEY;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		keypressed = NOTHING;
	pressMouseX = -(x/20 - 20);
	pressMouseZ = z/ 20 - 20;
}

//mouse movement function
void mouseMovement(int x, int z)
{
	mouseX = -(x/20 - 20);
	mouseZ = z/20 - 20;
}

//initialization function
void init()
{
	glClearColor(0, 0, 0, 1);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_COLOR_MATERIAL);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	freeTypeLibraryInit();
	generateTexture();
	field();
	mapImage();
	mobImage();
	turretImage();
	arrowImage();

	nexTex = readTexture("GameData/Plansza/gold.png");
	loadModel("GameData/obj/suzanne1.obj", model);
	

	faq();


	gamestate = GAME;		//menu isnt create 
}

//load image to graphic card memory
void mapImage()
{
	std::vector<unsigned char> image;
	unsigned width, height;
	//unsigned error = lodepng::decode(image, width, height, "D:/Polibuda/Semestr IV/Grafika komputerowa i wizualizacja/Tower_defense/GameData/Plansza/fieldTextureNew.png");
	unsigned error = lodepng::decode(image, width, height, "GameData/Plansza/fieldTextureBig.png");
	if (!error)
		std::cout << "Minion map texture loaded properly\n";
	else
		std::cout << "Minion map texture didnt loaded\n";
	glGenTextures(1, &minionFieldTex);
	glBindTexture(GL_TEXTURE_2D, minionFieldTex);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void arrowImage() {
	std::vector<unsigned char> image;
	unsigned width, height;
	//unsigned error = lodepng::decode(image, width, height, "D:/Polibuda/Semestr IV/Grafika komputerowa i wizualizacja/Tower_defense/GameData/Plansza/fieldTextureNew.png");
	unsigned error = lodepng::decode(image, width, height, "GameData/Plansza/moro.png");
	if (!error)
		std::cout << "Arrow texture loaded properly\n";
	else
		std::cout << "Arrow texture didnt loaded\n";
	glGenTextures(1, &arrowTex);
	glBindTexture(GL_TEXTURE_2D, arrowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

//field creation vertices taken from "Field.h" add textures to field
void createField(mat4 V, mat4 M)
{
	glMatrixMode(GL_MODELVIEW);
	
	glLoadMatrixf(glm::value_ptr(V*M));
	float lightPos0[] = { -30.0, -0.8, -7.4 };
	float lightPos1[] = { (-30.0, -0.8, -7.4) };
	float lightColorAmbient0[] = { 0,0.1,0.1,1 };
	float lightColor0[] = { 0,0.1,0.1,0 };
	float lightColorAmbient1[] = {0,0,0,1};
	float lightColor1[] = { 1,0,0,0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightColorAmbient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor0);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightColorAmbient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightColor1);
	
	glBindTexture(GL_TEXTURE_2D, minionFieldTex);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnable(GL_NORMALIZE);

	glVertexPointer(3, GL_FLOAT, 0, fieldVertices);
	glColorPointer(3, GL_FLOAT, 0, fieldColors);
	glTexCoordPointer(2, GL_FLOAT, 0, fieldTextureCoords);
	glNormalPointer(GL_FLOAT, 0, fieldVertices);
	glDrawArrays(GL_QUADS, 0, fieldVertexCount);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

//creating ghost turret
void ghostBuild(mat4 V, mat4 M)
{
	turret.push_back(FirstTurret(V, M,turretTex));
	changeGhostPosition();
	buildphase = GHOST;
}

//changing position in ghost buildphase
void changeGhostPosition()
{
	turret.back().setPosX(mouseX);
	turret.back().setPosZ(mouseZ);
}

//creating turret which is able to attack
void createSolidTurret()
{
	turret.back().isGhost = false;
	buildphase = SOLID;
}

void createArrow(mat4 V, mat4 M, int i)
{
	arrow.push_back(Arrow(V, M, turret[i].getPosX(), turret[i].getPosY(), turret[i].getPosZ()));
	arrow.back().setAttackedMob(turret[i].getAttackedNumber());
	arrow.back().setTurretNumber(i);
}

void createMob(mat4 V, mat4 M)
{
	switch (mobType)
	{
		case 0:
			mobAlive.push_back(NormalMob(V, M, fieldTab));
			if (level > 1)
				mobAlive.back().setHealth(level);
			break;
	}
}

//Display function
void displayFrame()
{
	if (gamestate == MENU)
		menu();
	else if (gamestate == GAME)
		game();
	else if (gamestate == GAME_OVER)
		gameOver();
	
	glFlush();
	glutSwapBuffers();
	Sleep(20);
}

//Redisplay function
void nextFrame()
{
	glutPostRedisplay();
}

//menu gamestate
void menu()
{

}

//gameover gamestate
void gameOver()
{
	std::cout << "RETRY?" << std::endl;
}


//game gamestate
void game()
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	switch (gamephase)
	{
		case MINION:
		{
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_LIGHTING);
			if (start)
			{
				std::cout << "Gold = " << gold << std::endl;
				start = false;
			}

			MVP();
			
			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(value_ptr(P));
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(value_ptr(V));
			glLoadIdentity();
			
			
			createField(V, M);
			model.draw(nexTex, V, M);
			

			for (int i = 0; i < mobAlive.size(); i++)
				mobAlive[i].buildPhase = false;

			for (int i = 0; i < arrow.size(); i++)
				arrow[i].buildPhase = false;

			if (mobAlive.size() + deathMobCount != maxMobAlive && mobAlive.size() == 0)
				createMob(V, M);
			else if (mobAlive.size() + deathMobCount != maxMobAlive && mobAlive.back().getTabPosX() != 0)
				createMob(V, M);

			if (turret.size() > 0)
				for (int i = 0; i < turret.size(); i++)
					if (turret[i].canAttack(mobAlive) && turret[i].canCreateArrow())
						createArrow(V, M, i);

			if (arrow.size() > 0)
					for (int j = 0; j < arrow.size(); j++)
					{
						arrow[j].setMobPosition(mobAlive[arrow[j].getAttackedMob()].getPosX(),
												mobAlive[arrow[j].getAttackedMob()].getPosY(),
												mobAlive[arrow[j].getAttackedMob()].getPosZ());
						arrow[j].setDistance();
						arrow[j].drawArrow(V, M);
						if (arrow[j].getDistanceX() <= 0.5f && arrow[j].getDistanceZ() <= 0.5f && arrow[j].getDistanceY() <= 0.5f)
						{
							mobAlive[arrow[j].getAttackedMob()].decreaseHealth(turret[arrow[j].getTurretNumber()].getDamage());
							arrow.erase(arrow.begin() + j);
							for (int i = 0; i < mobAlive.size(); i++)
								if (mobAlive[i].getHealth() <= 0)
								{
									mobAlive.erase(mobAlive.begin() + i);
									deathMobCount++;
									totalMobDied++;
									gold += static_cast<int> (income);
									std::cout << "Gold = " << gold << std::endl;	
								}
						}
					}
				
			deleteMob();

			drawMobOnField();
			drawTurretsOnField();

			increaseLevel();
			std::string goldString;
			goldString = std::to_string(gold);
			renderText("Gold = " + goldString, 25.0f, 750.0f, 1.0f, vec3(0.5f, 0.8f, 0.2f));
			break;
		}
		case BUILD:
		{
			glDisable(GL_DEPTH_TEST);
			MVP();
			
			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(value_ptr(P));
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glm::value_ptr(V));
			
			createField(V, M);
			model.draw(nexTex, V, M);
			if (buildphase == GHOSTBUILD && prevbuildphase != GHOSTBUILD)
			{
				if (gold >= turretCost)
				{
					ghostBuild(V, M);
					prevbuildphase = GHOSTBUILD;
				}
				else
					std::cout << "You havent got enough gold!" << std::endl;
			}
			if (buildphase == GHOST)
			{
				prevbuildphase = GHOST;
				changeGhostPosition();
				turret.back().drawGhostTurret(V, M,turretTex);
				if (keypressed == LEFTKEY)
				{
					if (turret.size() > 1)
						for (int i = 0; i < turret.size() - 1; i++)
						{
							if (turret.back().getPosX() != turret[i].getPosX() ||
								turret.back().getPosZ() != turret[i].getPosZ())
							{
								if (checkPathRoute())
								{
									createSolidTurret();
									gold -= turretCost;
									std::cout << "Gold = " << gold << std::endl;
									break;
								}
								else
									std::cout << "You cant place turret at minions path." << std::endl;
							}
							else
							{
								gold += turretCost;
								std::cout << "Couldnt create turret. Another turret is placed here!" << std::endl;
							}
						}
					else
					{
						if (checkPathRoute())
						{
							createSolidTurret();
							gold -= turretCost;
							std::cout << "Gold = " << gold << std::endl;
						}
						else
							std::cout << "You cant place turret at minions path." << std::endl;
					}
				}
			}

			if (buildphase == UPGRADE)
			{
				prevbuildphase = UPGRADE;
				if (keypressed == LEFTKEY)
					for (int i = 0; i < turret.size(); i++)
					{
						if (turret[i].getPosX() == pressMouseX &&
							turret[i].getPosZ() == pressMouseZ)
						{
							if (turret[i].getUpgradeCost() <= gold)
							{
								gold -= turret[i].getUpgradeCost();
								turret[i].upgrade();
								std::cout << "Turret " << i << " upgraded successfully." << std::endl;
								std::cout << "Gold = " << gold << std::endl;
							}
							else
								std::cout << "You havent got enough gold." << std::endl;
						}
						else
						{
							std::cout << "There isnt turret" << std::endl;
						}
					}
			}
			
			for (int i = 0; i < turret.size(); i++)
				if (turret[i].isGhost == false)
					turret[i].drawSolidTurret(V, M, mobAlive,turretTex);

			for (int i = 0; i < arrow.size(); i++)
				arrow[i].buildPhase = true;

			for (int i = 0; i < mobAlive.size(); i++)
				mobAlive[i].buildPhase = true;

			for (int i = 0; i < arrow.size(); i++)
				arrow[i].drawArrow(V, M);

			drawMobOnField();
			keypressed = NOTHING;

			break;
		}
	}
}

//init perspective, look at and and model view
void MVP()
{
	if (gamephase == MINION)
		P = perspective(50.0f, 1.0f, 1.0f, 50.0f);
	else if (gamephase == BUILD)
		P = perspective(51.05f, 1.0f, 1.0f, 50.0f);

	V = lookAt(vec3(camera.getPosX(), camera.getPosY(), camera.getPosZ()),
		vec3(camera.getAtX(), camera.getAtY(), camera.getAtZ()),
		vec3(camera.getNoseX(), camera.getNoseY(), camera.getNoseZ ()));
	
	M = mat4(1.0f);
}

//changing camera position by gamephase
void changeLookAt()
{
	if (gamephase == MINION)
	{
		camera.buildPhase = false;
		camera.changeCameraLookAt();
		
	}
	else if (gamephase == BUILD)
	{
		camera.buildPhase = true;
		camera.changeCameraLookAt();
		
	}
}

//draw turrets on field
void drawTurretsOnField()
{
	if (turret.size() > 0)
		for (int i = 0; i < turret.size(); i++)
			turret[i].drawSolidTurret(V, M, mobAlive,turretTex);
}

//drawing mobs on field
void drawMobOnField()
{
	if (mobAlive.size() > 0)
		for (int i = 0; i < mobAlive.size(); i++)
			mobAlive[i].drawMob(V, M, fieldTab ,mobTex);
}

//deleting mobs and adding gold
void deleteMob()
{
	for (int i = 0; i < mobAlive.size(); i++)
	{
		if (mobAlive[i].getTabPosX() == 19 || mobAlive[i].getTabPosZ() == 19)
		{
			mobAlive.erase(mobAlive.begin() + i);
			deathMobCount++;
			totalMobDied++;
			lives--;
			if (lives == 0)
				gamestate = GAME_OVER;
		}
	}
}

void initializeGLUT(int* pargc, char **argv)
{
	glutInit(pargc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(800, 0);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Tower Defense");
	glutDisplayFunc(displayFrame);
	glutIdleFunc(nextFrame);
	glutSpecialFunc(specialKey);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(mouseMovement);
}

void initializeGLEW()
{
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(1);
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}
//delete unnecessary things
void deleteThings()
{
	glDeleteTextures(1, &minionFieldTex);
}

int main(int argc, char** argv)
{
	initializeGLUT(&argc, argv);
	initializeGLEW();
	init();

	glutMainLoop();

	deleteThings();
	return 0;
}