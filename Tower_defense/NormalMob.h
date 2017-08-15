#ifndef NORMALMOB_H
#define NORMALMOB_H

#include "GL\glew.h"
#include "GL\glut.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include <iostream>
#include <string>
//#include "MobCube.h"

class NormalMob
{
	enum Directions { LEFT, RIGHT, UP, DOWN, NO_DIRECTION };
	Directions direction;
	//glm::mat4 M = glm::mat4(1.0f);
	float posX, posY, posZ;
	float nextPosX, nextPosZ, prevPosX, prevPosZ;
	float changePos = 0.1f;
	int tabPosX, tabPosZ;
	void getStartingPos(int fieldTab[21][21]);
	void startDrawMob(glm::mat4 V, glm::mat4 M);
	void checkRoute(int fieldTab[21][21]);
	//glm::vec3 move(int fieldTab[][21]);
	void move(int fieldTab[21][21]);
	int countX = 0, countZ = (tabPosZ + 1 * 20);
	int count = 0;
	

	public:
		void drawMob(glm::mat4 V, glm::mat4 M, int fieldTab[21][21]);
		NormalMob(glm::mat4 V, glm::mat4 M, int fieldTab[21][21]);
		~NormalMob();
};

#endif