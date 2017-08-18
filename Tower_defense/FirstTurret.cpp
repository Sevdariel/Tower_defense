#include "FirstTurret.h"
#include "MobVertices.h"

FirstTurret::FirstTurret(mat4 V, mat4 M, int fieldTab[21][21])
{
	drawGhostTurret(V, M, fieldTab);
}

FirstTurret::~FirstTurret() { }

void FirstTurret::changePosX(float x)
{
	posX = x;
}

void FirstTurret::changePosY(float y)
{
	posY = y;
}

void FirstTurret::changePosZ(float z)
{
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

void FirstTurret::drawGhostTurret(mat4 V, mat4 M, int fieldTab[21][21])
{
	glMatrixMode(GL_MODELVIEW);
	M = translate(M, vec3(posX, posY, posZ));
	M = scale(M, vec3(0.5f, 1, 0.5f));
	glLoadMatrixf(value_ptr(V*M));

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, mobCubeVertices);
	glColorPointer(3, GL_FLOAT, 0, mobCubeColorsAlpha);
	glDrawArrays(GL_QUADS, 0, mobCubeVertexCount);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}