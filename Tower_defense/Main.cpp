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

using namespace glm;

void init();
void createField(mat4 V);
void displayFrame(void);
void nextFrame();
void initializeGLUT(int* pargc, char **argv);
void initializeGLEW();
void specialKey(int key, int x, int y);
void Keyboard(unsigned char key, int x, int y);
void Field();
void loadImage();
void game();
void menu();
void gameOver();
void createMob(mat4 V, mat4 M);

GLuint fieldTex;
enum GameState { MENU, GAME, GAME_OVER};
int windowWidth = 800, windowHeight = 800;
int mobCount = 0, mobMaxCount = 30;
std::vector<NormalMob> mobAlive;
int count = 0;

GameState gamestate;
Camera camera;
NormalMob *normalMob;

int fieldTab[21][21];

//field array
void Field()
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

	/*for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 21; j++)
		{
			std::cout << fieldTab[i][j] << " ";
		}
		std::cout << std::endl;
	}*/
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
void Keyboard(unsigned char key, int x, int y)
{
	if (key == 'o' || key == 'O')
		camera.leftCameraRotation();
	else if (key == 'p' || key == 'P')
		camera.rightCameraRotation();

	if (key == 's' || key == 'S')
		camera.startPos();

	//quit when pushing esc button
	if (key == 27)
		exit(1);
	
	nextFrame();
}

void init()
{
	glClearColor(0, 0, 0, 1);
	glEnable(GL_TEXTURE_2D);
	
	loadImage();
	Field();

	gamestate = GAME;		//menu isnt create 
}
//load image to graphic card memory
void loadImage()
{
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, "D:/Polibuda/Semestr IV/Grafika Komputerowa i wizualizacja/Tower_defense/GameData/Plansza/fieldTexture.png");

	glGenTextures(1, &fieldTex);
	glBindTexture(GL_TEXTURE_2D, fieldTex);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

//field creation vertices taken from "Field.h" add textures to field
void createField(mat4 V)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(V));

	glBindTexture(GL_TEXTURE_2D, fieldTex);

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

//Display function
void displayFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 P = perspective(50.0f, 1.0f, 1.0f, 50.0f);

	mat4 V = lookAt(vec3(camera.getPosX(), camera.getPosY(), camera.getPosZ()),
		vec3(camera.getAtX(), camera.getAtY(), camera.getAtZ()),
		vec3(0.0f, 1.0f, 0.0f));

	mat4 M = mat4(1.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(value_ptr(P));

	createField(V);

	if (mobAlive.size() != 1)
		createMob(V, M);
	else
		//mobAlive[0].startDrawMob(V, M);
		mobAlive[0].drawMob(V, M, fieldTab);

	/*if (gamestate == MENU)
		menu();
	else if (gamestate == GAME)
		game();
	else if (gamestate == GAME_OVER)
		gameOver();*/
	
	glutSwapBuffers();
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

	mat4 P = perspective(50.0f, 1.0f, 1.0f, 50.0f);

	mat4 V = lookAt(vec3(camera.getPosX(), camera.getPosY(), camera.getPosZ()),
		vec3(camera.getAtX(), camera.getAtY(), camera.getAtZ()),
		vec3(0.0f, 1.0f, 0.0f));

	mat4 M = mat4(1.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(value_ptr(P));

	createField(V);

	if (mobAlive.size() != 1)
		createMob(V, M);
	else
		//mobAlive[0].startDrawMob(V, M);
		mobAlive[0].drawMob(V, M, fieldTab);
	
}

void createMob(mat4 V, mat4 M)
{
	//normalMob = new NormalMob(V, M, fieldTab);
	mobAlive.push_back(NormalMob(V, M, fieldTab));
	std::cout << mobAlive.size() << std::endl;
}

void initializeGLUT(int* pargc, char **argv)
{
	glutInit(pargc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(800, 0);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Tower Defense");
	glutDisplayFunc(displayFrame);
	glutIdleFunc(nextFrame);
	glutSpecialFunc(specialKey);
	glutKeyboardFunc(Keyboard);
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
	glDeleteTextures(1, &fieldTex);
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