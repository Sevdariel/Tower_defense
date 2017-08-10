#define GLM_FORCE_RADIANS

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
#include "Field.h"
#include "Camera.h"

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

Camera camera;

int fieldTab[41][41];

void Field()
{
	std::ifstream field("D:/Polibuda/Semestr IV/Grafika Komputerowa i wizualizacja/Tower_defense/GameData/Plansza/lvl1.txt");
	if (field.good())
		std::cout << "Plik lvl1.txt zostal otwarty poprawnie";
	else
		std::cout << "Nie udalo sie wczytac pliku";

	std::string row;
	for (int i = 0; i < 41; i++)
		for (int j = 0; j < 41; j++)
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
void Keyboard(unsigned char key, int x, int y)
{
	if (key == 'o' || key == 'O')
		camera.leftCameraRotation();
	else if (key == 'p' || key == 'P')
		camera.rightCameraRotation();

	if (key == 's' || key == 'S')
		camera.startPos();
	
	/*if (key == '-')
		camera.cameraDistanceLonger();
	else if (key == '+')
		camera.cameraDistanceShorter();*/
	nextFrame();
}

void init()
{
	glClearColor(0, 0, 0, 1);

	Field();
}

//field creation vertices taken from "Field.h"
void createField(mat4 V)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(V));

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, fieldVertices);
	glColorPointer(3, GL_FLOAT, 0, fieldColors);
	glDrawArrays(GL_QUADS, 0, fieldVertexCount);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

//Display function
void displayFrame(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 P = perspective(50.0f, 1.0f, 1.0f, 50.0f);

	mat4 V = lookAt(vec3(camera.getPosX(), camera.getPosY(), camera.getPosZ()),
					vec3(camera.getAtX(), camera.getAtY(), camera.getAtZ()),
					vec3(0.0f, 1.0f, 0.0f));

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(value_ptr(P));

	createField(V);

	glutSwapBuffers();
}

//Redisplay function
void nextFrame()
{
	glutPostRedisplay();
}

void initializeGLUT(int* pargc, char **argv)
{
	glutInit(pargc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(800, 0);
	glutInitWindowSize(800, 800);
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

int main(int argc, char** argv)
{
	initializeGLUT(&argc, argv);
	initializeGLEW();
	init();

	glutMainLoop();

	return 0;
}