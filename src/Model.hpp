#pragma once

#include <iostream>

#include <fstream>
#include <sstream>
#include <vector> 
#include <algorithm>
#include <cstdlib>


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Model
{
public:

	Model();
	~Model();

	void loadObjFile(std::string _fileName);
	void LoadTexture(std::string _fileName);
	void initialzeModel();

	void drawModel();

	std::vector<GLushort> getElements() { return _elements; };
	std::vector<glm::vec3> getVertices() { return _vertices; };
	std::vector<glm::vec2> getUvs() { return _uvs; };
	std::vector<glm::vec3> getNormals() { return _normals; };
	GLuint getTextureID() { return _textureID; };

	int getVerticesCount() { return (int) _vertices.size(); };
	int getUVCount() { return (int) _uvs.size(); };
	int getNormalCount() { return (int) _normals.size(); };

private:

	std::vector<GLushort> _elements;
	std::vector<glm::vec3> _vertices;
	std::vector<glm::vec2> _uvs;
	std::vector<glm::vec3> _normals;
	GLuint _textureID;

	std::vector<unsigned int> _vertexIndex, _normalIndex, _textureIndex;

	GLuint _vao;

};

