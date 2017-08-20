//#define GLM_FORCE_RADIANS

#include <Windows.h>
#include "GL\glew.h"
#include "GL\glut.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include "Field.h"
#include "Camera.h"
#include "lodepng.h"
#include "NormalMob.h"
#include "FirstTurret.h"

using namespace glm;

void init();
void createField(mat4 V);
void displayFrame();
void nextFrame();
void initializeGLUT(int* pargc, char **argv);
void initializeGLEW();
void specialKey(int key, int x, int y);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void mouseMovement(int x, int y);
void field();
void loadImage();
void game();
void menu();
void gameOver();
void createMob(mat4 V, mat4 M);
void deleteMob();
void MVP();
void drawMobOnField();
void changeLookAt();
void ghostBuild(mat4 V, mat4 M);
void changeGhostPosition();
void createSolidTurret();

GLuint minionFieldTex, buildFieldTex;
mat4 P, V, M;
enum GameState { MENU, GAME, GAME_OVER};
enum GamePhase { BUILD, MINION};
enum BuildPhase { GHOSTBUILD, GHOST, SOLID};
enum KeyPressed { LEFTKEY, RIGHTKEY, NOTHING};
int windowWidth = 800, windowHeight = 800;
int mobCount = 0, mobMaxCount = 30;
std::vector<NormalMob> mobAlive;
std::vector<FirstTurret> turret;
int maxMobAlive = 30, deathMobCount = 0;
int pressMouseX, pressMouseZ, mouseX, mouseZ;

GameState gamestate;
GamePhase gamephase = MINION;
BuildPhase buildphase = SOLID;
KeyPressed keypressed = NOTHING;
Camera camera;
NormalMob *normalMob;

int fieldTab[21][21];

//field array
void field()
{
	std::ifstream field("D:/Polibuda/Semestr IV/Grafika Komputerowa i wizualizacja/Tower_defense/GameData/Plansza/lvl1.txt");
	if (field.good())
		std::cout << "Map file loaded properly" << std::endl;
	else
		std::cout << "Couldnt load map file" << std::endl;;

	std::string row;
	for (int i = 0; i < 21; i++)
		for (int j = 0; j < 21; j++)
		{
			field >> fieldTab[i][j];
		}

	field.close();
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
	//quit when pushing esc button
	if (key == 27)
		exit(1);
	
	nextFrame();
}

void mouse(int button, int state, int x, int z)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		keypressed = LEFTKEY;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		keypressed = NOTHING;
	pressMouseX = x;
	pressMouseZ = z;
}

void mouseMovement(int x, int z)
{
	mouseX = x/20 - 20;
	mouseZ = z/20 - 20;
}
void init()
{
	glClearColor(0, 0, 0, 1);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	loadImage();
	field();

	gamestate = GAME;		//menu isnt create 
}
//load image to graphic card memory
void loadImage()
{
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, "D:/Polibuda/Semestr IV/Grafika Komputerowa i wizualizacja/Tower_defense/GameData/Plansza/fieldTexture.png");
	if (!error)
		std::cout << "Minion map texture loaded properly\n";
	else
		std::cout << "Minion map texture didnt loaded\n";
	glGenTextures(1, &minionFieldTex);
	glBindTexture(GL_TEXTURE_2D, minionFieldTex);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

//field creation vertices taken from "Field.h" add textures to field
void createField(mat4 V)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(V));

	/*if (gamephase == MINION)
		glBindTexture(GL_TEXTURE_2D, minionFieldTex);
	else if (gamephase == BUILD)
		glBindTexture(GL_TEXTURE_2D, buildFieldTex);*/

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, fieldVertices);
	glColorPointer(3, GL_FLOAT, 0, fieldColors);
	glTexCoordPointer(2, GL_FLOAT, 0, fieldTextureCoords);
	glDrawArrays(GL_QUADS, 0, fieldVertexCount);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ghostBuild(mat4 V, mat4 M)
{
	turret.push_back(FirstTurret(V, M, fieldTab));
	changeGhostPosition();
	buildphase = GHOST;
}

void changeGhostPosition()
{
	turret.back().changePosX(mouseX);
	turret.back().changePosZ(mouseZ);
}
void createSolidTurret()
{
	turret.back().isGhost = false;
	buildphase = SOLID;
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

}
//game gamestate
void game()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	switch (gamephase)
	{
		case MINION:
		{
			MVP();
			
			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(value_ptr(P));

			createField(V);

			for (int i = 0; i < mobAlive.size(); i++)
				mobAlive[i].buildPhase = false;

			if (mobAlive.size() != maxMobAlive + deathMobCount && mobAlive.size() == 0)
				createMob(V, M);
			else if (mobAlive.size() != maxMobAlive + deathMobCount && mobAlive.back().getTabPosX() != 0)
				createMob(V, M);

			deleteMob();

			drawMobOnField();
			break;
		}
		case BUILD:
		{
			MVP();
			
			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(value_ptr(P));
			createField(V);

			if (buildphase == GHOSTBUILD)
				ghostBuild(V, M);

			if (buildphase == GHOST)
			{
				changeGhostPosition();
				for (int i = 0; i < turret.size() - 1; i++)
					turret[i].drawSolidTurret(V, M, fieldTab);
				turret.back().drawGhostTurret(V, M, fieldTab);
				if (keypressed == LEFTKEY)
					createSolidTurret();
			}
			for (int i = 0; i < turret.size(); i++)
				if (turret[i].isGhost == false)
					turret[i].drawSolidTurret(V, M, fieldTab);


			for (int i = 0; i < mobAlive.size(); i++)
				mobAlive[i].buildPhase = true;

			drawMobOnField();
			break;
		}
	}
}

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

void drawMobOnField()
{
	if (mobAlive.size() > 0)
		for (int i = 0; i < mobAlive.size(); i++)
			mobAlive[i].drawMob(V, M, fieldTab);
}

void createMob(mat4 V, mat4 M)
{
	mobAlive.push_back(NormalMob(V, M, fieldTab));
}

void deleteMob()
{
	for (int i = 0; i < mobAlive.size(); i++)
		if (mobAlive[i].getTabPosX() == 20 || 
			mobAlive[i].getTabPosZ() == 20 || 
			mobAlive[i].getHealth() == 0)
		{
			mobAlive.erase(mobAlive.begin() + i);
			deathMobCount++;
		}
}

void initializeGLUT(int* pargc, char **argv)
{
	glutInit(pargc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
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
	glDeleteTextures(1, &buildFieldTex);
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