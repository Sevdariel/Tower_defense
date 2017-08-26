#include "NormalMob.h"
#include "MobVertices.h"

using namespace glm;

void NormalMob::startDrawMob(glm::mat4 V, glm::mat4 M, int fieldTab[21][21])
{
	checkRoute(fieldTab);
	glMatrixMode(GL_MODELVIEW);
	M = translate(M, vec3(posX, posY, posZ));
	M = scale(M, vec3(0.5f, 0.5f, 0.5f));
	glLoadMatrixf(value_ptr(V*M));

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, mobCubeVertices);
	glColorPointer(4, GL_FLOAT, 0, mobCubeColors);
	glDrawArrays(GL_QUADS, 0, mobCubeVertexCount);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void NormalMob::drawMob(glm::mat4 V, glm::mat4 M, int fieldTab[21][21],GLuint tex)
{
	if (direction == STOP)
	{
		glMatrixMode(GL_MODELVIEW);
		M = translate(M, vec3(posX, posY, posZ));
		M = scale(M, vec3(0.5f, 0.5f, 0.5f));
		checkRoute(fieldTab);
	}
	else 
	{
		move(fieldTab);
		glMatrixMode(GL_MODELVIEW);
		M = translate(M, vec3(posX, posY, posZ));
		M = scale(M, vec3(0.5f, 0.5f, 0.5f));
	}
	glLoadMatrixf(value_ptr(V*M));

	glBindTexture(GL_TEXTURE_2D, tex);
	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, mobCubeVertices);
	glTexCoordPointer(2, GL_FLOAT, 0, mobCubeTex);
	glColorPointer(4, GL_FLOAT, 0, mobCubeColors);
	glDrawArrays(GL_QUADS, 0, mobCubeVertexCount);
	//glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

NormalMob::NormalMob(glm::mat4 V, glm::mat4 M, int fieldTab[21][21])
{
	getStartingPos(fieldTab);
	startDrawMob(V,M, fieldTab);
}

int NormalMob::getTabPosX()
{
	return tabPosX;
}

int NormalMob::getTabPosZ()
{
	return tabPosZ;
}

float NormalMob::getPosX()
{
	return posX;
}

float NormalMob::getPosY()
{
	return posY;
}

float NormalMob::getPosZ()
{
	return posZ;
}

int NormalMob::getHealth()
{
	return healthPoints;
}

void NormalMob::setHealth(int level)
{
	healthPoints *= 1.1;
}

void NormalMob::decreaseHealth(int health)
{
	healthPoints -= health;
}

NormalMob::~NormalMob()	{	}

void NormalMob::getStartingPos(int fieldTab[21][21])
{
	for (int i = 0; i < 21; i++)
		if (fieldTab[i][0] == 2)
		{
			posX = static_cast<float> (20 - 0.5f);
			posY = 0.0f;
			posZ = static_cast<float> (i * 2 - 20);
			tabPosX = 0;
			tabPosZ = i;
			break;
		}
	
	prevPosX = posX;
	prevPosZ = posZ;
	countX = tabPosX;
	countZ = tabPosZ * 80;
}

void NormalMob::checkRoute(int fieldTab[21][21])
{
	if (tabPosX == 21 || buildPhase == true)
		direction = STOP;
	else if (tabPosX == 0)
	{
		if (fieldTab[tabPosZ][tabPosX + 1] == 2 && prevDirection != LEFT)
			direction = RIGHT;
		else if (fieldTab[tabPosZ - 1][tabPosX] == 2 && prevDirection != DOWN)
			direction = UP;
		else if (fieldTab[tabPosZ + 1][tabPosX] == 2 && prevDirection != UP)
			direction = DOWN;
	}
	else
	{
		if (fieldTab[tabPosZ][tabPosX + 1] == 2 && prevDirection != LEFT)
			direction = RIGHT;
		else if (fieldTab[tabPosZ][tabPosX - 1] == 2 && prevDirection != RIGHT)
			direction = LEFT;
		else if (fieldTab[tabPosZ - 1][tabPosX] == 2 && prevDirection != DOWN)
			direction = UP;
		else if (fieldTab[tabPosZ + 1][tabPosX] == 2 && prevDirection != UP)
			direction = DOWN;
	}
}

void NormalMob::move(int fieldTab[21][21])
{
	checkRoute(fieldTab);
	switch (direction)
	{
		case LEFT:
		{
			countX--;
			if (countX % 80 == 0)
			{
				prevDirection = direction;
				tabPosX--;
			}
			prevPosX = posX;
			posX += changePos * speed;
			break;
		}
		case RIGHT:
		{
			countX++;
			if (countX % 80 == 0)
			{
				prevDirection = direction;
				tabPosX++;
			}
			prevPosX = posX;
			posX -= changePos * speed;
			break;
		}
		case UP:
		{
			countZ--;
			if (countZ % 80 == 0)
			{
				prevDirection = direction;
				tabPosZ--;
			}
			prevPosZ = posZ;
			posZ -= changePos * speed;
			break;
		}
		case DOWN:
		{
			countZ++;
			if (countZ % 80 == 0)
			{
				prevDirection = direction;
				tabPosZ++;
			}
			prevPosZ = posZ;
			posZ += changePos * speed;
			break;
		}
		case STOP:
			break;
	}

	
}