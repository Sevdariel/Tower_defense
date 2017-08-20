#ifndef FIRSTTURRET_H
#define FIRSTTURRET_H

#include "GL\glew.h"
#include "GL\glut.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include <iostream>


using namespace glm;

class FirstTurret
{
	private:
		float posX, posY = 0.0f, posZ;
		float range, attackSpeed;

	public:
		FirstTurret(mat4 V, mat4 M, int fieldTab[21][21]);
		~FirstTurret();
		bool isGhost;	
		
		void drawGhostTurret(mat4 V, mat4 M, int fieldTab[21][21]);
		void drawSolidTurret(mat4 V, mat4 M, int fieldTab[21][21]);
		void changePosX(int x);
		void changePosY(int y);
		void changePosZ(int z);
		float getPosX();
		float getPosY();
		float getPosZ();
};

#endif