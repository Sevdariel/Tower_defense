#ifndef NORMALMOB_H
#define NORMALMOB_H

#include "GL\glew.h"
#include "GL\glut.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include <iostream>
#include <string>

class NormalMob
{
	int speed, healthPoints;
	enum Directions { LEFT, RIGHT, UP, DOWN, NO_DIRECTION, STOP };
	Directions direction = NO_DIRECTION;
	Directions prevDirection;
	float posX, posY, posZ;
	float nextPosX, nextPosZ, prevPosX, prevPosZ;
	float changePos = 0.025f;
	int tabPosX, tabPosZ;
	void getStartingPos(int fieldTab[21][21]);
	void startDrawMob(glm::mat4 V, glm::mat4 M, int fieldTab[21][21]);
	void checkRoute(int fieldTab[21][21]);
	void move(int fieldTab[21][21]);
	int countX, countZ;
	int count = 0;
	

	public:
		void drawMob(glm::mat4 V, glm::mat4 M, int fieldTab[21][21]);
		NormalMob(glm::mat4 V, glm::mat4 M, int fieldTab[21][21]);
		int getTabPosX();
		int getTabPosZ();
		~NormalMob();

		bool buildPhase = false;
};

#endif