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
		float range = 3.0f, dangerousRange = 2.5f;
		int damage = 10, attackSpeed = 100;
		int attackedNumber = -1, dangerousAttackedNumber = -1;
		int killNumber, dangerousKillNumber;
		int attackCount = 100;

	public:
		bool isGhost;
		float attackMobPosX, attackMobPosZ, attackMobPosY;
		float dangerousAttackMobPosX, dangerousAttackMobPosY, dangerousAttackMobPosZ;
		
		FirstTurret(mat4 V, mat4 M,GLuint tex);
		~FirstTurret();	
		void drawGhostTurret(mat4 V, mat4 M,GLuint tex);
		void drawSolidTurret(mat4 V, mat4 M, std::vector<NormalMob> mobAlive,GLuint tex);
		void setPosX(int x);
		void setPosY(int y);
		void setPosZ(int z);
		float getPosX();
		float getPosY();
		float getPosZ();
		bool canAttack(std::vector<NormalMob> mobAlive);
		float mobDistance(std::vector<NormalMob> mobAlive, int i);
		int getAttackedNumber();
		int getDamage();
		bool canCreateArrow();
};

#endif