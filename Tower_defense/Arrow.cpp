#define _CRT_SECURE_NO_WARNINGS
#include "Arrow.h"
#include "ArrowVertices.h"
#include "MobVertices.h"

void Arrow::move()
{
	setDistance();
	if (mobPosX < posX && mobPosZ < posZ)
	{
		posX -= flySpeed*distanceX;
		posY += flySpeed*distanceY;
		posZ -= flySpeed*distanceZ;
	}
	else if (mobPosX > posX && mobPosZ < posZ)
	{
		posX += flySpeed*distanceX;
		posY += flySpeed*distanceY;
		posZ -= flySpeed*distanceZ;
	}
	else if (mobPosX > posX && mobPosZ > posZ)
	{
		posX += flySpeed*distanceX;
		posY += flySpeed*distanceY;
		posZ += flySpeed*distanceZ;
	}
	else if (mobPosX < posX && mobPosZ > posZ)
	{
		posX -= flySpeed*distanceX;
		posY += flySpeed*distanceY;
		posZ += flySpeed*distanceZ;
	}
}

void Arrow::setDistance()
{
	distanceX = abs(mobPosX - posX);
	distanceZ = abs(mobPosZ - posZ);
	distanceY = abs(mobPosY - posY);
}

float Arrow::getDistanceX()
{
	return distanceX;
}

float Arrow::getDistanceZ()
{
	return distanceZ;
}

float Arrow::getDistanceY()
{
	return distanceY;
}

void Arrow::setAttackedMob(int x)
{
	attackedMob = x;
}

int Arrow::getAttackedMob()
{
	return attackedMob;
}

void Arrow::setTurretNumber(int x)
{
	turretNumber = x;
}

int Arrow::getTurretNumber()
{
	return turretNumber;
}

void Arrow::setStartingPosition(float x, float y, float z)
{
	posX = x;
	startPosX = x;
	posY = y;
	startPosY = y;
	posZ = z;
	startPosZ = z;
}

Arrow::Arrow(mat4 V, mat4 M, float x, float y, float z)
{
	setStartingPosition(x, y, z);
	startDrawArrow(V, M);
}

Arrow::~Arrow() {	}

void Arrow::startDrawArrow(mat4 V, mat4 M)
{
	
	glMatrixMode(GL_MODELVIEW);
	M = translate(M, vec3(startPosX, startPosY, startPosZ));
	M = scale(M, vec3(0.05f, 0.05f, 0.05f));
	glLoadMatrixf(value_ptr(V*M));


	///TEST
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnable(GL_NORMALIZE);
	glVertexPointer(3, GL_FLOAT, 0, mobCubeVertices);
	glTexCoordPointer(2, GL_FLOAT, 0, mobCubeTex);
	glNormalPointer(GL_FLOAT, 0, mobCubeVertices);
	glDrawArrays(GL_QUADS, 0, mobCubeVertexCount);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void Arrow::drawArrow(mat4 V, mat4 M)
{
	if (buildPhase == false)
		move();
	glMatrixMode(GL_MODELVIEW);
	M = translate(M, vec3(posX, posY, posZ));
	M = scale(M, vec3(0.05f, 0.05f, 0.05f));
	glLoadMatrixf(value_ptr(V*M));
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnable(GL_NORMALIZE);
	glVertexPointer(3, GL_FLOAT, 0, mobCubeVertices);
	glTexCoordPointer(2, GL_FLOAT, 0, mobCubeTex);
	glNormalPointer(GL_FLOAT, 0, mobCubeVertices);
	glDrawArrays(GL_QUADS, 0, mobCubeVertexCount);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

float Arrow::getPosX()
{
	return posX;
}

float Arrow::getPosY()
{
	return posY;
}

float Arrow::getPosZ()
{
	return posZ;
}

void Arrow::setPosX(float x)
{
	posX = x;
}

void Arrow::setPosY(float y)
{
	posY = y;
}

void Arrow::setPosZ(float z)
{
	posZ = z;
}

void Arrow::setMobPosition(float x, float y, float z)
{
	mobPosX = x;
	mobPosY = y;
	mobPosZ = z;
}
