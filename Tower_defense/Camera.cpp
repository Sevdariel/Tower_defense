#include <iostream>
#include "Camera.h"

Camera::Camera()
{
	startPos();
}

Camera::~Camera()
{
	std::cout << "Deleting camera." << std::endl;
}

void Camera::rightCameraRotation()
{
	//I quarter
	if (posX >= 0 + tmpX && posX < 25.0f + tmpX &&
		posZ >= -25.0f + tmpZ && posZ < 0 + tmpZ )
	{
		posX += rad;
		posZ += rad;
	}
	//II quarter
	else if (posX >= -25.0f + tmpX && posX < 0 + tmpX &&
		posZ > -25.0f + tmpZ && posZ <= 0 + tmpZ )
	{
		posX += rad;
		posZ -= rad;
	}
	//III quarter
	else if (posX > -25.0f + tmpX && posX <= 0 + tmpX &&
		posZ > 0 + tmpZ && posZ <= 25.0f + tmpZ)
	{
		posX -= rad;
		posZ -= rad;
	}
	//IV quarter
	else if (posX > 0 + tmpX && posX <= 25.0f + tmpX &&
		posZ >= 0 + tmpZ && posZ < 25.0f + tmpZ)
	{
		posX -= rad;
		posZ += rad;
	}
}

void Camera::leftCameraRotation()
{
	//I quarter
	if (posX > 0 + tmpX && posX <= 25.0f + tmpX &&
		posZ > -25.0f + tmpZ && posZ <= 0 + tmpZ)
	{
		posX -= rad;
		posZ -= rad;
	}
	//II quarter
	else if (posX > -25.0f + tmpX && posX <= 0 + tmpX &&
		posZ >= -25.0f + tmpZ && posZ < 0 + tmpZ)
	{
		posX -= rad;
		posZ += rad;
	}
	//III quarter
	else if (posX >= -25.0f + tmpX && posX < 0 + tmpX &&
		posZ >= 0 + tmpZ && posZ < 25.0f + tmpZ)
	{
		posX += rad;
		posZ += rad;
	}
	//IV quarter
	else if (posX >= 0 + tmpX && posX < 25.0f + tmpX &&
		posZ > 0 + tmpZ && posZ <= 25.0f + tmpZ)
	{
		posX += rad;
		posZ -= rad;
	}
}

void Camera::cameraDistanceLonger()
{
	if (posX == 0 && posZ - atZ < 0)
	{
		posY += rad;
		posZ -= rad;
		tmpZ -= rad;
	}
	else if (posX == 0 && posZ - atZ > 0)
	{
		posY += rad;
		posZ += rad;
		tmpZ += rad;
	}
	else if (posZ == 0 && posX - atX < 0)
	{
		posY += rad;
		posX -= rad;
		tmpX -= rad;
	}
	else if (posZ == 0 && posX - atX > 0)
	{
		posY += rad;
		posX += rad;
		tmpX += rad;
	}
	else if (posZ - atZ < 0 && posX - atX < 0)
	{
		posX -= rad;
		posY += rad;
		posZ -= rad;
		tmpZ -= rad;
		tmpX -= rad;
	}
	else if (posZ - atZ > 0 && posX - atX < 0)
	{
		posX -= rad;
		posY += rad;
		posZ += rad;
		tmpZ += rad;
		tmpX -= rad;
	}
	else if (posZ - atZ < 0 && posX - atX > 0)
	{
		posX += rad;
		posY += rad;
		posZ -= rad;
		tmpZ -= rad;
		tmpX += rad;
	}
	else if (posZ - atZ > 0 && posX - atX > 0)
	{
		posX += rad;
		posY += rad;
		posZ += rad;
		tmpZ += rad;
		tmpX += rad;
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
		posY -= rad;
		posZ += rad;
		tmpZ += rad;
	}
	else if (posX == 0 && posZ - atZ > 0)
	{
		posY -= rad;
		posZ -= rad;
		tmpZ -= rad;
	}
	else if (posZ == 0 && posX - atX < 0)
	{
		posY -= rad;
		posX += rad;
		tmpX += rad;
	}
	else if (posZ == 0 && posX - atX > 0)
	{
		posY -= rad;
		posX -= rad;
		tmpX -= rad;
	}
	else if (posZ - atZ < 0 && posX - atX < 0)
	{
		posX += rad;
		posY -= rad;
		posZ += rad;
		tmpZ += rad;
		tmpX += rad;
	}
	else if (posZ - atZ > 0 && posX - atX < 0)
	{
		posX += rad;
		posY -= rad;
		posZ -= rad;
		tmpZ -= rad;
		tmpX += rad;
	}
	else if (posZ - atZ < 0 && posX - atX > 0)
	{
		posX -= rad;
		posY -= rad;
		posZ += rad;
		tmpZ += rad;
		tmpX -= rad;
	}
	else if (posZ - atZ > 0 && posX - atX > 0)
	{
		posX -= rad;
		posY -= rad;
		posZ -= rad;
		tmpZ -= rad;
		tmpX -= rad;
	}
	std::cout << "camera.posX = " << posX << std::endl;
	std::cout << "camera.posZ = " << posZ << std::endl;
	std::cout << "tmpX = " << tmpX << std::endl;
	std::cout << "tmpZ = " << tmpZ << std::endl;
}

void Camera::cameraUp()
{
	posZ -= rad;
	atZ -= rad;
	tmpZ -= rad;
}

void Camera::cameraDown()
{
	posZ += rad;
	atZ += rad;
	tmpZ += rad;
}

void Camera::cameraLeft()
{
	posX += rad;
	atX += rad;
	tmpX += rad;
}

void Camera::cameraRight()
{
	posX -= rad;
	atX -= rad;
	tmpX -= rad;
}

float Camera::getAtX()
{
	return atX;
}

float Camera::getAtY()
{
	return atY;
}

float Camera::getNoseX()
{
	return noseX;
}

float Camera::getNoseY()
{
	return noseY;
}

float Camera::getNoseZ()
{
	return noseZ;
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

void Camera::startPos()
{
	posX = startX;
	posY = startY;
	posZ = startZ;
	atX = startAtX;
	atY = startAtY;
	atZ = startAtZ;
	noseX = startNoseX;
	noseY = startNoseY;
	noseZ = startNoseZ;
	tmpX = 0.0f;
	tmpY = 0.0f;
	tmpZ = 0.0f;
}

void Camera::changeCameraLookAt()
{
	if (buildPhase == true)
	{
		prevPosX = posX;
		prevPosY = posY;
		prevPosZ = posZ;
		prevAtX = atX;
		prevAtY = atY;
		prevAtZ = atZ;
		prevNoseX = noseX;
		prevNoseY = noseY;
		prevNoseZ = noseZ;
		posX = 0.0f;
		posY = -50.0f;
		posZ = 0.0f;
		atX = 0.0f;
		atY = 0.0f;
		atZ = 0.0f;
		noseX = 0.0f;
		noseY = 0.0f;
		noseZ = -1.0f;
	}
	else
	{
		posX = prevPosX;
		posY = prevPosY;
		posZ = prevPosZ;
		atX = prevAtX;
		atY = prevAtY;
		atZ = prevAtZ;
		noseX = prevNoseX;
		noseY = prevNoseY;
		noseZ = prevNoseZ;
	}
}