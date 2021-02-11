
#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>


class Shader
{
public:
	Shader();
	~Shader();

	void compileShaders(std::string vname, std::string fname);
	GLuint getShaderProg();
	int getULocation(std::string name);

private:
	void loadShaders();
	bool checkShaderCompiled(GLint shader);


	GLuint _vertexShaderID, _fragmentShaderID;
	GLuint _vertexID, _fragmentID;
	GLuint _shaderProgram;
	std::string _sPVertex, _sPFragment;
	std::string _vertexCode, _fragmentCode;

};

