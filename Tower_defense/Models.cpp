//#include "stdafx.h"

#define _CRT_SECURE_NO_WARNINGS
#include "Models.h"
#include <string>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <lodepng.h>
#include <stdio.h>
BlenderModel::BlenderModel() {};



BlenderModel::BlenderModel(std::string path, bool czyTekstura) : path(path), czyTekstura(czyTekstura) {};

std::string BlenderModel::toString() {
	std::ostringstream stream;
	stream << path << " " << czyTekstura << " " << vertices.size() << " " << uvArray.size() << " " << normals.size();
	return stream.str();
}

void BlenderModel::clear() {
	vertices.clear();
	uvArray.clear();
	normals.clear();
}

void BlenderModel::draw() {
	if (vertices.size() > 0) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
		glNormalPointer(GL_FLOAT, 0, &normals[0]);

		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}
}

void BlenderModel::draw(GLuint& tekstura, glm::mat4 V, glm::mat4 M) {
	if (vertices.size() > 0) {
		//glm::mat4 M = glm::mat4(1.0);
		M = glm::translate(M, glm::vec3(-20.0f, -0.8f, -7.4f));
		M = glm::rotate(M, 90.0f*3.14f / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		M = glm::rotate(M, 90.0f*3.14f / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		//M = glm::scale(M, glm::vec3(0.5f, 0.5f, 0.5f));
		glLoadMatrixf(glm::value_ptr(V*M));

		if (czyTekstura)
			glBindTexture(GL_TEXTURE_2D, tekstura);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		if (czyTekstura)
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, vertices.data());
		glNormalPointer(GL_FLOAT, 0, normals.data());

		if (czyTekstura)
			glTexCoordPointer(2, GL_FLOAT, 0, uvArray.data());

		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		if (czyTekstura)
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}

glm::vec3 BlenderModel::count_normal(glm::vec3 coord1, glm::vec3 coord2, glm::vec3 coord3) {
	glm::vec3 va, vb, vr;

	va.x = coord1.x - coord2.x;
	va.y = coord1.y - coord2.y;
	va.z = coord1.z - coord2.z;

	vb.x = coord1.x - coord3.x;
	vb.y = coord1.y - coord3.y;
	vb.z = coord1.z - coord3.z;

	/* cross product */
	vr.x = va.y * vb.z - vb.y * va.z;
	vr.y = vb.x * va.z - va.x * vb.z;
	vr.z = va.x * vb.y - vb.x * va.y;

	/* normalization factor */
	float val = sqrt(vr.x * vr.x + vr.y * vr.y + vr.z * vr.z);

	glm::vec3 norm;
	norm.x = vr.x / val;
	norm.y = vr.y / val;
	norm.z = vr.z / val;

	return norm;
}

bool BlenderModel::loadBlenderModerl(std::string path, BlenderModel* model, bool czyTekstura) {
	model->czyTekstura = czyTekstura;
	model->path = path;
	printf("Loading OBJ file %s...\n", path.c_str());
	model->clear();

	std::vector<int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
	std::vector<glm::vec3> temp_normals_count;


	FILE * file = fopen(path.c_str(), "r");
	if (file == NULL) {
		printf("Impossible to open the file\n");
		return false;
	}

	while (1) {
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y;
			// Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			int vertexIndex[3], uvIndex[3] = { 0, 0, 0 }, normalIndex[3] = { 0, 0, 0 };
			char line[100];
			fgets(line, 80, file);
			int matches = sscanf(line, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1],
				&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				int matches6 = sscanf(line, "%d//%d %d//%d %d//%d\n",
					&vertexIndex[0], &normalIndex[0],
					&vertexIndex[1], &normalIndex[1],
					&vertexIndex[2], &normalIndex[2]);
				if (matches6 != 6) {
					int matches3 = sscanf(line, "%d %d %d\n",
						&vertexIndex[0],
						&vertexIndex[1],
						&vertexIndex[2]);
					if (matches3 != 3) {
						printf("File can't be read by our simple parser :-( Try exporting with other options\n");
						printf("%s\n%d %d %d\n", line, matches, matches6, matches3);
						fclose(file);
						return false;
					}
				}
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			if (normalIndex[0] == 0 && normalIndex[1] == 0 && normalIndex[2] == 0) {
				glm::vec3 normal;
				normal.x = vertexIndex[0];
				normal.y = vertexIndex[1];
				normal.z = vertexIndex[2];
				temp_normals_count.push_back(normal);
				normalIndices.push_back(-1 * temp_normals_count.size());
				normalIndices.push_back(-1 * temp_normals_count.size());
				normalIndices.push_back(-1 * temp_normals_count.size());
			}
			else {
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
		}
		else {
			char buffer[1000];
			fgets(buffer, 1000, file);
		}
	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		// Get the indices of its attributes
		int vertexIndex = vertexIndices[i];
		int uvIndex = uvIndices[i];
		int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv;
		if (uvIndex > 0)
			uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal;
		if (normalIndex > 0)
			normal = temp_normals[normalIndex - 1];
		else if (normalIndex < 0) {
			glm::vec3 normals_vertex_to_count = temp_normals_count[-1 * normalIndex - 1];
			normal = count_normal(temp_vertices[normals_vertex_to_count.z - 1],
				temp_vertices[normals_vertex_to_count.y - 1],
				temp_vertices[normals_vertex_to_count.z - 1]);
		}

		// Put the attributes in buffers
		model->vertices.push_back(vertex);
		model->uvArray.push_back(uv);
		model->normals.push_back(normal);
	}
	fclose(file);
	return true;
}


GLuint readTexture(char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pamiêci komputera
	std::vector<unsigned char> image;  //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
							  //Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);
	if (error == 0) {
		printf("Load texture: %s\n", filename);
		//Import do pamiêci karty graficznej
		glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
		glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
										   //Wczytaj obrazek do pamiêci KG skojarzonej z uchwytem
		glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		printf("Not oppened texture: %s\n", filename);
	}

	return tex;
}
