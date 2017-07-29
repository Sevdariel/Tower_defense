#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
	private:
		float posX, posY, posZ;							//camera position
		float atX, atY, atZ;							//camera looking at
		const float rad = 0.75f / 3;					//rotation degree
		const float to = 0.01f;							//translate distance
		float tmpX = 0, tmpY = 0, tmpZ = 0;

	public:
		Camera();
		void rightCameraRotation();						//Rotate camera right
		void leftCameraRotation();						//Rotate camera left
		void cameraDistanceLonger();					//camera push away
		void cameraDistanceShorter();					//camera approach
		void cameraUp();								//camera move up
		void cameraDown();								//camera move down
		void cameraLeft();								//camera move left
		void cameraRight();								//camera move right
		float getPosX();								//returning posX
		float getPosY();								//returning posY
		float getPosZ();								//returning posZ
		float getAtX();									//returning atX
		float getAtZ();									//returning atZ
		float getAtY();									//returning atY
};

#endif