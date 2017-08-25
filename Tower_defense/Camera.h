#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
	private:
		const float startX = 0.0f, startY = -20.0f, startZ = 25.0f;				//camera starting position
		const float startAtX = 0.0f, startAtY = 0.0f , startAtZ = 0.0f;			//camera looking at starting position
		const float startNoseX = 0.0f, startNoseY = 1.0f, startNoseZ = 0.0f;	//camera nose vector starting position
		float buildPosX = 0.0f, buildPosY = -50.0f, buildPosZ = 0.0f;
		float buildAtX = 0.0f, buildAtY = 0.0f, buildAtZ = 0.0f;
		float buildNoseX = 0.0f, buildNoseY = 0.0f, buildNoseZ = -1.0f;
		float posX, posY, posZ;													//camera position
		float atX, atY, atZ;													//camera looking at
		float noseX, noseY, noseZ;												//camera nose vector
		float prevPosX, prevPosY, prevPosZ;
		float prevAtX, prevAtY, prevAtZ;
		float prevNoseX, prevNoseY, prevNoseZ;
		const float rad = 0.25f;												//rotation degree and translation distance
		float tmpX = 0.0f, tmpY = 0.0f, tmpZ = 0.0f;							//temporary values needed to correct rotation
			
	public:
		Camera();
		~Camera();
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
		float getNoseX();								//returning noseX
		float getNoseY();								//returning noseY
		float getNoseZ();								//returning noseZ
		void startPos();								//assign starting camera position values
		void changeCameraLookAt();						//changing camera position between MINION and BUILD phases
		
		bool buildPhase;								//build phase flag
};

#endif