#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
	private:
		//const float startX = 0.0f, startY = -20.0f, startZ = 25.0f;			//camera starting position
		const float startX = 0.0f, startY = -20.0f, startZ = 25.0f;			//camera starting position
		const float startAtX = 0.0f, startAtY = 0.0f , startAtZ = 0.0f;		//camera looking at starting position
		float posX, posY, posZ;												//camera position
		float atX, atY, atZ;												//camera looking at
		const float rad = 0.25f;											//rotation degree and translation distance
		float tmpX = 0.0f, tmpY = 0.0f, tmpZ = 0.0f;						//temporary values needed to correct rotation

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
		void startPos();								//assign starting camera position values
		void check();									//checking position values
};

#endif