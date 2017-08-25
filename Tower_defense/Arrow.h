#ifndef ARROW_H
#define ARROW_H

#include "GL\glew.h"
#include "GL\glut.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include <iostream>

using namespace glm;

class Arrow
{
	float posX, posY, posZ;
	float startPosX, startPosY, startPosZ;
	float flySpeed = 0.05f;
	float mobPosX, mobPosY, mobPosZ;
	float distanceX, distanceZ;
	void move();
	void setStartingPosition(float x, float y, float z);
	int attackedMob;
	int turretNumber;

	public:
		bool buildPhase = false;

		Arrow(mat4 V, mat4 M, float x, float y, float z);
		~Arrow();
		void startDrawArrow(mat4 V, mat4 M);
		void drawArrow(mat4 V, mat4 M);
		float getPosX();
		float getPosY();
		float getPosZ();
		void setPosX(float x);
		void setPosY(float y);
		void setPosZ(float z);
		void setMobPosition(float mobPosX, float mobPosY, float mobPosZ);
		void setDistance();
		float getDistanceX();
		float getDistanceZ();
		void setAttackedMob(int x);
		int getAttackedMob();
		void setTurretNumber(int x);
		int getTurretNumber();
};
#endif