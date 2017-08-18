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
		float posX, posY, posZ;

	public:
		FirstTurret(mat4 V, mat4 M, int fieldTab[21][21]);
		~FirstTurret();
		bool isGhost;	
		
		void drawGhostTurret(mat4 V, mat4 M, int fieldTab[21][21]);
		void changePosX(float x);
		void changePosY(float y);
		void changePosZ(float z);
		float getPosX();
		float getPosY();
		float getPosZ();
};

#endif