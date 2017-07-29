#include <iostream>
#include "Camera.h"

Camera::Camera()
{
	posX = 0.0f;
	posY = 20.0f;
	posZ = 15.0f;
	atX = 0.0f;
	atY = 0.0f;
	atZ = 0.0f;
}

void Camera::leftCameraRotation()
{
	if (posX <= 0 + tmpX && posX > -15.0f + tmpX &&
		posZ >= -15.0f + tmpZ && posZ < 0 + tmpZ)
	{
		posX -= rad;
		posZ += rad;
	}
	else if (posX >= -15.0f + tmpX && posX < 0 + tmpX &&
		posZ >= 0 + tmpZ && posZ < 15.0f + tmpZ)
	{
		posX += rad;
		posZ += rad;
	}
	else if (posX >= 0 + tmpX && posX < 15.0f + tmpX &&
		posZ <= 15.0f + tmpZ && posZ > 0 + tmpZ)
	{
		posX += rad;
		posZ -= rad;
	}
	else if (posX <= 15.0f + tmpX && posX > 0 + tmpX &&
		posZ <= 0 + tmpZ && posZ > -15.0f + tmpZ)
	{
		posX -= rad;
		posZ -= rad;
	}

	std::cout << "camera.posX = " << posX << std::endl;
	std::cout << "camera.posZ = " << posZ << std::endl;
}

void Camera::rightCameraRotation()
{
	if (posX >= 0 + tmpX && posX < 15.0f + tmpX &&
		posZ >= -15.0f + tmpZ && posZ < 0 + tmpZ)
	{
		posX += rad;
		posZ += rad;
	}
	else if (posX <= 15.0f + tmpX && posX > 0 + tmpX &&
		posZ >= 0 + tmpZ && posZ < 15.0f + tmpZ)
	{
		posX -= rad;
		posZ += rad;
	}
	else if (posX <= 0 + tmpX && posX > -15.0f + tmpX &&
		posZ <= 15.0f + tmpZ && posZ > 0 + tmpZ)
	{
		posX -= rad;
		posZ -= rad;
	}
	else if (posX >= -15.0f + tmpX && posX < 0 + tmpX &&
		posZ <= 0 + tmpZ && posZ > -15.0f + tmpZ)
	{
		posX += rad;
		posZ -= rad;
	}

	std::cout << "camera.posX = " << posX << std::endl;
	std::cout << "camera.posZ = " <<posZ << std::endl;
}

void Camera::cameraDistanceLonger()
{
	if (posX == 0 && posZ - atZ < 0)
	{
		posY += to;
		posZ -= to;
		tmpZ -= to;
	}
	else if (posX == 0 && posZ - atZ > 0)
	{
		posY += to;
		posZ += to;
		tmpZ += to;
	}
	else if (posZ == 0 && posX - atX < 0)
	{
		posY += to;
		posX -= to;
		tmpX -= to;
	}
	else if (posZ == 0 && posX - atX > 0)
	{
		posY += to;
		posX += to;
		tmpX += to;
	}
	else if (posZ - atZ < 0 && posX - atX < 0)
	{
		posX -= to;
		posY += to;
		posZ -= to;
		tmpZ -= to;
		tmpX -= to;
	}
	else if (posZ - atZ > 0 && posX - atX < 0)
	{
		posX -= to;
		posY += to;
		posZ += to;
		tmpZ += to;
		tmpX -= to;
	}
	else if (posZ - atZ < 0 && posX - atX > 0)
	{
		posX += to;
		posY += to;
		posZ -= to;
		tmpZ -= to;
		tmpX += to;
	}
	else if (posZ - atZ > 0 && posX - atX > 0)
	{
		posX += to;
		posY += to;
		posZ += to;
		tmpZ += to;
		tmpX += to;
	}

	std::cout << "camera.posX = " << posX << std::endl;
	std::cout << "camera.posZ = " << posZ << std::endl;
	std::cout << "tmpX = " << tmpX << std::endl;
	std::cout << "tmpZ = " << tmpZ << std::endl;
}

void Camera::cameraDistanceShorter()
{
	if (posX == 0 && posZ - atZ < 0)
	{
		posY -= to;
		posZ += to;
		tmpZ += to;
	}
	else if (posX == 0 && posZ - atZ > 0)
	{
		posY -= to;
		posZ -= to;
		tmpZ -= to;
	}
	else if (posZ == 0 && posX - atX < 0)
	{
		posY -= to;
		posX += to;
		tmpX += to;
	}
	else if (posZ == 0 && posX - atX > 0)
	{
		posY -= to;
		posX -= to;
		tmpX -= to;
	}
	else if (posZ - atZ < 0 && posX - atX < 0)
	{
		posX += to;
		posY -= to;
		posZ += to;
		tmpZ += to;
		tmpX += to;
	}
	else if (posZ - atZ > 0 && posX - atX < 0)
	{
		posX += to;
		posY -= to;
		posZ -= to;
		tmpZ -= to;
		tmpX += to;
	}
	else if (posZ - atZ < 0 && posX - atX > 0)
	{
		posX -= to;
		posY -= to;
		posZ += to;
		tmpZ += to;
		tmpX -= to;
	}
	else if (posZ - atZ > 0 && posX - atX > 0)
	{
		posX -= to;
		posY -= to;
		posZ -= to;
		tmpZ -= to;
		tmpX -= to;
	}
	std::cout << "camera.posX = " << posX << std::endl;
	std::cout << "camera.posZ = " << posZ << std::endl;
	std::cout << "tmpX = " << tmpX << std::endl;
	std::cout << "tmpZ = " << tmpZ << std::endl;
}

void Camera::cameraUp()
{
	posY += to;
	atY += to;
	tmpY -= to;
}

void Camera::cameraDown()
{
	posY -= to;
	atY -= to;
	tmpY += to;
}

void Camera::cameraLeft()
{
	posX -= to;
	atX -= to;
	tmpX -= to;
}

void Camera::cameraRight()
{
	posX += to;
	atX += to;
	tmpX += to;
}

float Camera::getAtX()
{
	return atX;
}

float Camera::getAtY()
{
	return atY;
}

float Camera::getAtZ()
{
	return atZ;
}

float Camera::getPosX()
{
	return posX;
}

float Camera::getPosY()
{
	return posY;
}

float Camera::getPosZ()
{
	return posZ;
}