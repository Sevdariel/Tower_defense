#ifndef NORMALMOB_H
#define NORMALMOB_H

#include "GL\glew.h"
#include "GL\glut.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include <iostream>


class NormalMob
{
	int speed = 1, healthPoints = 100;										//Mob speed and health
	enum Directions { LEFT, RIGHT, UP, DOWN, NO_DIRECTION, STOP };			//mob moving directions
	Directions direction = NO_DIRECTION;									//direction variable
	Directions prevDirection;												//previous direction variable
	float posX, posY, posZ;													//mob position
	float prevPosX, prevPosZ;												//mob previous and next position
	float changePos = 0.025f;												//moving distance
	int tabPosX, tabPosZ;													//array coordinates

	void getStartingPos(int fieldTab[21][21]);								//set starting position
	void startDrawMob(glm::mat4 V, glm::mat4 M, int fieldTab[21][21]);		//first mob draw
	void checkRoute(int fieldTab[21][21]);									//checking where mob can go
	void move(int fieldTab[21][21]);										//move
	int countX, countZ;														//addition variable needed to change array coords

	public:
		void drawMob(glm::mat4 V, glm::mat4 M, int fieldTab[21][21],GLuint tex);		//standard mob draw
		NormalMob(glm::mat4 V, glm::mat4 M, int fieldTab[21][21]);			//mob constructor
		int getTabPosX();													//returning tabPosX
		int getTabPosZ();													//returning tabPosZ
		float getPosX();
		float getPosY();
		float getPosZ();
		int getHealth();
		void setHealth(int level);
		void decreaseHealth(int health);
		~NormalMob();

		bool buildPhase = false;											//build phase flag
};

#endif