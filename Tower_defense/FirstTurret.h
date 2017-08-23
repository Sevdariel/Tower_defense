#ifndef FIRSTTURRET_H
#define FIRSTTURRET_H

#include "GL\glew.h"
#include "GL\glut.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include "NormalMob.h"
#include "Arrow.h"


using namespace glm;

class FirstTurret
{
	private:
		float posX, posY = 0.0f, posZ;
		float range = 3.0f;
		int damage = 10, attackSpeed = 100;
		int attackedNumber;
		int attackCount = 100;
		std::vector<Arrow> arrow;

	public:
		bool isGhost;
		float attackMobPosX, attackMobPosZ, attackMobPosY;
		
		FirstTurret(mat4 V, mat4 M);
		~FirstTurret();	
		void drawGhostTurret(mat4 V, mat4 M);
		void drawSolidTurret(mat4 V, mat4 M, std::vector<NormalMob> mobAlive);
		void setPosX(int x);
		void setPosY(int y);
		void setPosZ(int z);
		float getPosX();
		float getPosY();
		float getPosZ();
		bool canAttack(std::vector<NormalMob> mobAlive);
		float mobDistance(std::vector<NormalMob> mobAlive, int i);
		float getAttackMobPosX();
		float getAttackMobPosY();
		float getAttackMobPosZ();
		int getAttackedNumber();
		int getDamage();
		bool canCreateArrow();
};

#endif