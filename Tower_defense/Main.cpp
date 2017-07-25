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
#include "Field.h"

using namespace glm;

void init();
void createField(mat4 V);
void displayFrame(void);
void nextFrame();
void initializeGLUT(int* pargc, char **argv);
void initializeGLEW();
void specialKey(int key, int x, int y);
void Keyboard(unsigned char key, int x, int y);

//camera  definition
struct Camera
{
	float posX, posY, posZ;
	float atX, atY, atZ;
};
Camera camera;
//rotation speed
const float rad = 1.5f;//0.75f/3;
float tmpX = 0, tmpY = 0, tmpZ = 0;

//special keyboard keys
void specialKey(int key, int x, int y)
{
	//camera position change
	if (key == GLUT_KEY_UP)
	{
		camera.posY += 1.0f;
		camera.atY += 1.0f;
		tmpY += 1.0f;
	}
	if (key == GLUT_KEY_DOWN)
	{
		camera.posY -= 1.0f;
		camera.atY -= 1.0f;
		tmpY -= 1.0f;
	}
	if (key == GLUT_KEY_LEFT)
	{
		camera.posZ += 1.0f;
		camera.atZ += 1.0f;
		tmpZ += 1.0f;
	}
	if (key == GLUT_KEY_RIGHT)
	{
		camera.posZ -= 1.0f;
		camera.atZ -= 1.0f;
		tmpZ -= 1.0f;
	}
	nextFrame();
}

//standard keyboard keys
void Keyboard(unsigned char key, int x, int y)
{
	//left camera rotation
	if (key == 'o' || key == 'O')
	{
		if (camera.posX <= 0 && camera.posX > -15.0f &&
			camera.posZ >= -15.0f + tmpZ && camera.posZ < 0 + tmpZ)
		{
			camera.posX -= rad;
			camera.posZ += rad;
		}
		else if (camera.posX >= -15.0f && camera.posX < 0 &&
			camera.posZ >= 0 + tmpZ && camera.posZ < 15.0f + tmpZ)
		{
			camera.posX += rad;
			camera.posZ += rad;
		}
		else if (camera.posX >= 0 && camera.posX < 15.0f &&
			camera.posZ <= 15.0f + tmpZ && camera.posZ > 0 + tmpZ)
		{
			camera.posX += rad;
			camera.posZ -= rad;
		}
		else if (camera.posX <= 15.0f && camera.posX > 0 &&
			camera.posZ <= 0 + tmpZ && camera.posZ > -15.0f + tmpZ)
		{
			camera.posX -= rad;
			camera.posZ -= rad;
		}
	}
	//right camera rotation
	else if (key == 'p' || key == 'P')
	{
		if (camera.posX >= 0 && camera.posX < 15.0f &&
			camera.posZ >= -15.0f + tmpZ && camera.posZ < 0 + tmpZ)
		{
			camera.posX += rad;
			camera.posZ += rad;
		}
		else if (camera.posX <= 15.0f && camera.posX > 0 &&
				 camera.posZ >= 0 + tmpZ && camera.posZ < 15.0f + tmpZ)
		{
			camera.posX -= rad;
			camera.posZ += rad;
		}
		else if (camera.posX <= 0 && camera.posX > -15.0f &&
				 camera.posZ <= 15.0f + tmpZ && camera.posZ > 0 + tmpZ)
		{
			camera.posX -= rad;
			camera.posZ -= rad;
		}
		else if (camera.posX >= -15.0f && camera.posX < 0 &&
				 camera.posZ <= 0 + tmpZ && camera.posZ > -15.0f + tmpZ)
		{
			camera.posX += rad;
			camera.posZ -= rad;
		}
	}
	nextFrame();
}

void init()
{
	glClearColor(0, 0, 0, 1);

	//camera position initialize
	camera.posX = 15.0f;
	camera.posY = -20.0f;
	camera.posZ = 0.0f;
	camera.atX = 0.0f;
	camera.atY = 0.0f;
	camera.atZ = 0.0f;
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

	mat4 V = lookAt(vec3(camera.posX, camera.posY, camera.posZ),
					vec3(camera.atX, camera.atY, camera.atZ),
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
	glutInitWindowPosition(300, 0);
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