#define _CRT_SECURE_NO_WARNINGS
#include "FirstTurret.h"
#include "MobVertices.h"

FirstTurret::FirstTurret(mat4 V, mat4 M,GLuint tex)
{
	drawGhostTurret(V, M,tex);
}

FirstTurret::~FirstTurret() { }

void FirstTurret::setPosX(int x)
{
	posX = static_cast<float> (x);
}

void FirstTurret::setPosY(int y)
{
	posY = static_cast<float> (y);
}

void FirstTurret::setPosZ(int z)
{
	posZ = static_cast<float> (z);
}

float FirstTurret::getPosX()
{
	return posX;
}

float FirstTurret::getPosY()
{
	return posY;
}

float FirstTurret::getPosZ()
{
	return posZ;
}

bool FirstTurret::canAttack(std::vector<NormalMob> mobAlive)
{
	for (int i = 0; i < mobAlive.size(); i++)
		if (mobDistance(mobAlive, i) < range)
		{
			attackedNumber = i;
			return true;
		}
	return false;
}

float FirstTurret::mobDistance(std::vector<NormalMob> mobAlive, int i)
{
	float distanceSQR = pow(mobAlive[i].getPosX() - posX, 2) +
		pow(mobAlive[i].getPosZ() - posZ, 2);

	float distance = sqrt(distanceSQR);

	return distance;
}

int FirstTurret::getAttackedNumber()
{
	return attackedNumber;
}

int FirstTurret::getDamage()
{
	return damage;
}

bool FirstTurret::canCreateArrow()
{

	if (attackCount % attackSpeed == 0)
	{
		attackCount++;
		return true;
	}
	else
	{
		attackCount++;
		return false;
	}
}

void FirstTurret::upgrade()
{
	damage *= 1.1;
	attackSpeed--;
	upgradeCost *= 1.3;
}

int FirstTurret::getUpgradeCost()
{
	return upgradeCost;
}

void FirstTurret::drawGhostTurret(mat4 V, mat4 M,GLuint tex)
{
	glMatrixMode(GL_MODELVIEW);
	M = translate(M, vec3(posX, posY, posZ));
	M = scale(M, vec3(0.5f, 1, 0.5f));
	glLoadMatrixf(value_ptr(V*M));

	glBindTexture(GL_TEXTURE_2D, tex);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, mobCubeVertices);
	glTexCoordPointer(2, GL_FLOAT, 0, mobCubeTex);
	glEnable(GL_NORMALIZE);
	glDrawArrays(GL_QUADS, 0, mobCubeVertexCount);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void FirstTurret::drawSolidTurret(mat4 V, mat4 M, std::vector<NormalMob> mobAlive, GLuint tex)
{
	glMatrixMode(GL_MODELVIEW);
	M = translate(M, vec3(posX, posY, posZ));
	M = scale(M, vec3(0.5f, 1, 0.5f));
	glLoadMatrixf(value_ptr(V*M));

	
	glBindTexture(GL_TEXTURE_2D, tex);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnable(GL_NORMALIZE);
	glVertexPointer(3, GL_FLOAT, 0, mobCubeVertices);
	glTexCoordPointer(2, GL_FLOAT, 0, mobCubeTex);
	glNormalPointer(GL_FLOAT, 0, mobCubeVertices);
	glDrawArrays(GL_QUADS, 0, mobCubeVertexCount);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}
