#include "NormalMob.h"
#include "MobCube.h"

#include <iostream>

using namespace glm;

void NormalMob::startDrawMob(glm::mat4 V, glm::mat4 M)
{
	glMatrixMode(GL_MODELVIEW);
	M = translate(M, vec3(posX, posY, posZ));
	M = scale(M, vec3(0.5f, 0.5f, 0.5f));
	glLoadMatrixf(value_ptr(V*M));

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, mobCubeVertices);
	glColorPointer(3, GL_FLOAT, 0, mobCubeColors);
	glDrawArrays(GL_QUADS, 0, mobCubeVertexCount);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void NormalMob::drawMob(glm::mat4 V, glm::mat4 M, int fieldTab[21][21])
{
	//move(fieldTab);
	if (direction = NO_DIRECTION)
	{
		glMatrixMode(GL_MODELVIEW);
		M = translate(M, vec3(posX, posY, posZ));
		M = scale(M, vec3(0.5f, 0.5f, 0.5f));
		glLoadMatrixf(value_ptr(V*M));
		checkRoute(fieldTab);
	}
	else if (direction != NO_DIRECTION)
	{
		std::cout << "drawMob cout" << std::endl;
		move(fieldTab);
		glMatrixMode(GL_MODELVIEW);
		
		
		M = translate(M, vec3(posX, posY, posZ) + vec3(nextPosX, 0.0f, nextPosZ));
		M = scale(M, vec3(0.5f, 0.5f, 0.5f));
		glLoadMatrixf(value_ptr(V*M));
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, mobCubeVertices);
	glColorPointer(3, GL_FLOAT, 0, mobCubeColors);
	glDrawArrays(GL_QUADS, 0, mobCubeVertexCount);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	if (count < 150)
	{
		std::cout << "posX = " << posX << std::endl;
		std::cout << "posY = " << posY << std::endl;
		std::cout << "posZ = " << posZ << std::endl;
		std::cout << "tabPosX = " << tabPosX << std::endl;
		std::cout << "tabPosZ = " << tabPosZ << std::endl;
		std::cout << "prevPosX = " << prevPosX << std::endl;
		std::cout << "prevPosZ = " << prevPosZ << std::endl;
		std::cout << "direction = " << direction << std::endl << std::endl;
	}
	count++;
}

NormalMob::NormalMob(glm::mat4 V, glm::mat4 M, int fieldTab[21][21])
{
	getStartingPos(fieldTab);
	drawMob(V, M, fieldTab);

	for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 21; j++)
		{
			std::cout << fieldTab[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

NormalMob::~NormalMob()
{

}

void NormalMob::getStartingPos(int fieldTab[21][21])
{
	for (int i = 0; i < 21; i++)
		if (fieldTab[i][0] == 2)
		{
			posX = static_cast<float> (20 - 0);
			posY = 0.0f;
			posZ = static_cast<float> (i * 2 - 20);
			tabPosX = 0;
			tabPosZ = i;
			break;
		}
	
	prevPosX = posX;
	prevPosZ = posZ;
	direction = NO_DIRECTION;

	std::cout << "posX = " << posX << std::endl;
	std::cout << "posY = " << posY << std::endl;
	std::cout << "posZ = " << posZ << std::endl;
	std::cout << "tabPosX = " << tabPosX << std::endl;
	std::cout << "tabPosZ = " << tabPosZ << std::endl;
	std::cout << "prevPosX = " << prevPosX << std::endl;
	std::cout << "prevPosZ = " << prevPosZ << std::endl;
	std::cout << "direction = " << direction << std::endl;

}

void NormalMob::checkRoute(int fieldTab[21][21])
{
	if (posX == 20 || tabPosX == 0)
	{
		if (fieldTab[tabPosZ][tabPosX + 1] == 2 && prevPosX <= posX)
			direction = RIGHT;
		else if (fieldTab[tabPosZ - 1][tabPosX] == 2 && prevPosZ <= posZ )
			direction = UP;
		else if (fieldTab[tabPosZ + 1][tabPosX] == 2 && prevPosZ >= posZ)
			direction = DOWN;
	}
	else if (posX != 20 && tabPosX != 0)
	{
		if (fieldTab[tabPosZ][tabPosX + 1] == 2 && prevPosX <= posX)
			direction = RIGHT;
		else if (fieldTab[tabPosZ][tabPosX - 1] == 2 && prevPosX >= posX)
			direction = LEFT;
		else if (fieldTab[tabPosZ - 1][tabPosX] == 2 && prevPosZ <= posZ)
			direction = UP;
		else if (fieldTab[tabPosZ + 1][tabPosX] == 2 && prevPosZ >= posZ)
			direction = DOWN;
	}
	if (count < 150)
	std::cout << "CHECKROUTE direction = " << direction << std::endl;
}

//glm::vec3 NormalMob::move(int fieldTab[][21])
//{
//	checkRoute(fieldTab);
//	switch (direction)
//	{
//		case LEFT:
//		{
//			countX--;
//			if (countX % 20 == 0)
//				tabPosX++;
//			return vec3(changePos, 0.0f, 0.0f);
//		}
//		case RIGHT:
//		{
//			countX++;
//			if (countX % 20 == 0)
//				tabPosX--;
//			return vec3(-changePos, 0.0f, 0.0f);
//		}
//		case UP:
//		{
//			countZ--;
//			if (countZ % 20 == 0)
//				tabPosZ--;
//			return vec3(0.0f, 0.0f, -changePos);
//		}
//		case DOWN:
//		{
//			countZ++;
//			if (countZ % 20 == 0)
//				tabPosZ++;
//			return vec3(0.0f, 0.0f, changePos);
//		}
//	}
//	if (count < 150)
//	std::cout << "MOVE direction = " << direction << std::endl;
//}

void NormalMob::move(int fieldTab[21][21])
{
	if (count < 150)
		std::cout << "MOVE direction = " << direction << std::endl;
	checkRoute(fieldTab);
	switch (direction)
	{
		case LEFT:
		{
			nextPosZ = 0.0f;
			countX--;
			if (countX % 20 == 0)
				tabPosX++;
			if (count < 150)
				std::cout << "MOVE direction = " << direction << std::endl;
			nextPosX += changePos;
			break;
		}
		case RIGHT:
		{
			nextPosZ = 0.0f;
			countX++;
			if (countX % 20 == 0)
				tabPosX--;
			if (count < 150)
				std::cout << "MOVE direction = " << direction << std::endl;
			nextPosX -= changePos;
			break;
		}
		case UP:
		{
			nextPosX = 0.0f;
			countZ--;
			if (countZ % 20 == 0)
				tabPosZ--;
			if (count < 150)
				std::cout << "MOVE direction = " << direction << std::endl;
			nextPosZ -= changePos;
			break;
		}
		case DOWN:
		{
			nextPosX = 0.0f;
			countZ++;
			if (countZ % 20 == 0)
				tabPosZ++;
			if (count < 150)
				std::cout << "MOVE direction = " << direction << std::endl;
			nextPosZ += changePos;
			break;
		}
	}
	if (count < 150)
		std::cout << "MOVE direction = " << direction << std::endl;
}