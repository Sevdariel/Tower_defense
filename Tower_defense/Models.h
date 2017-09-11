#include <string>
#include <vector>
#include "GL\glew.h"
#include "GL\glut.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

#ifndef MODELS_H
#define MODELS_H

class BlenderModel {
	std::string path;
	std::vector < glm::vec3 > vertices;
	std::vector < glm::vec2 > uvArray;
	std::vector < glm::vec3 > normals;
	bool czyTekstura;
	float posX, posY, posZ;

public:
	BlenderModel();
	BlenderModel(std::string path, bool czyTekstura);

	std::string toString();
	void clear();
	void draw();
	void draw(GLuint& tekstura, glm::mat4 V, glm::mat4 M);
	static glm::vec3 count_normal(glm::vec3 coord1, glm::vec3 coord2, glm::vec3 coord3);
	static bool loadBlenderModerl(std::string path, BlenderModel* model, bool czyTekstura);
};

GLuint readTexture(char* filename);

#endif // !MODELS_H
