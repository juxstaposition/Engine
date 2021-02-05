#include "Model.hpp"

// Object loader created by Rory Clark, https://rory.games/p/obj-loader, 2016
// Might be replaced by assimp loader


Model::Model()
{
}

Model::~Model()
{
	// Delete our vertex array object and wipe all the data in the vectors
	glDeleteVertexArrays(1, &_vao);
	_vertices.clear();
	_normals.clear();
	_uvs.clear();
}

void Model::loadObjFile(std::string _fileName)
{
	// Check the file name to make sure it has the OBJ extension
	std::string ext = _fileName;
	for (int i = 0; i < ext.length(); i++)
	{
		ext.at(i) = tolower(ext.at(i));
	}
	if (ext.substr(ext.find_last_of(".") + 1) != "obj")
	{
		throw("Filename is not a valid .obj file.\nPlease change your settings and try again.");
	}

	std::ifstream objFile(_fileName);
	std::string line, errorInfo;
	std::istringstream sstream;
	// Temporary vectors used for storing the raw read data
	std::vector<glm::vec3> tempVerts, tempNormals;
	std::vector<glm::vec2> tempUv;
	std::vector<unsigned int> tempIV, tempIN, tempIT;
	int lineCount = 0;
	// If we can open the file then all clear
	if (objFile)
	{
		// Read the line, works until end of file
		while (std::getline(objFile, line))
		{
			lineCount++;
			// Ignore blank lines
			if (line.length() > 0)
			{
				// Clear out stringstream so that we can add new text to it
				sstream.clear();
				if (line.find("#") == 0)
				{
					// do nothing, it's a comment
					// means we can skip over the rest of the if/else statements quickly
				}
				// vertices
				else if (line.find("v ") == 0)
				{
					glm::vec3 vertex;
					sstream.str(line.substr(2));
					// Double check that it has the correct number of items by auto splitting by spaces (3 parts)
					std::istream_iterator<std::string> begV(sstream), endV;
					std::vector<std::string> vParts(begV, endV);
					if (vParts.size() != 3)
					{
						errorInfo = "Obj file has an invalid number of vertices at line: " + std::to_string(lineCount) + ".\nPlease either use a different model or fix to use 3 vertices (x y z).";
						throw(errorInfo);
					}
					sstream.clear();
					sstream.str(line.substr(2));
					// Use our stringstream to output the required float values to the vec3
					sstream >> vertex.x >> vertex.y >> vertex.z;
					tempVerts.push_back(vertex);
				}
				// uvs
				else if (line.find("vt ") == 0)
				{
					// Same principle for UVs but with 2 items instead of 3
					glm::vec2 uv;
					sstream.str(line.substr(3));
					std::istream_iterator<std::string> begU(sstream), endU;
					std::vector<std::string> uParts(begU, endU);
					if (uParts.size() != 2)
					{
						errorInfo = "Obj file has an invalid number of UVs at line: " + std::to_string(lineCount) + ".\nPlease either use a different model or fix to use 2 UV floats.";
						throw(errorInfo);
					}
					sstream.clear();
					sstream.str(line.substr(3));
					sstream >> uv.x >> uv.y;
					tempUv.push_back(uv);
				}
				// normals
				else if (line.find("vn ") == 0)
				{
					// Same principle as vertices
					glm::vec3 normal;
					sstream.str(line.substr(3));
					std::istream_iterator<std::string> begN(sstream), endN;
					std::vector<std::string> nParts(begN, endN);
					if (nParts.size() != 3)
					{
						errorInfo = "Obj file has an invalid number of normals at line: " + std::to_string(lineCount) + ".\nPlease either use a different model or fix to use 3 normal floats.";
						throw(errorInfo);
					}
					sstream.clear();
					sstream.str(line.substr(3));
					sstream >> normal.x >> normal.y >> normal.z;
					tempNormals.push_back(normal);
				}
				// faces
				else if (line.find("f ") == 0)
				{
					sstream.str(line.substr(2));
					// Split our faces down into their single chuncks of data so that we can then split based on the /'s
					std::istream_iterator<std::string> beg(sstream), end;
					std::vector<std::string> fParts(beg, end);
					sstream.clear();

					for (auto fStr : fParts)
					{
						sstream.str(fStr);
						int c = 0;
						std::string token;
						// Read the stringstream up until the first instance of slash
						// Next loop will read from after that slash
						while (std::getline(sstream, token, '/'))
						{
							// Always want to increase the counter so that we correctly assign vertices, uvs or normals
							// #// = vertices only
							// #//# = vertices and normals
							// #/#/# = vertices, uvs and normals
							if (token.length() > 0)
							{
								switch (c)
								{
								case 0:
									tempIV.push_back(atoi(token.c_str()));
									break;
								case 1:
									tempIN.push_back(atoi(token.c_str()));
									break;
								case 2:
									tempIT.push_back(atoi(token.c_str()));
									break;
								}
							}
							c++;
						}
						// Make sure we looped through 3 times no matter what (accounts for the 2 slashes or 3 numbers)
						if (c < 3)
						{
							errorInfo = "Obj has incorrect face parameters at line: " + std::to_string(lineCount) + ".\nPlease either use a different model or fix to use 3 split numbers (#/#/#).";
							throw(errorInfo);
						}
						sstream.clear();
					}

				}
			}
		}
		objFile.close();
		for (int i = 0; i < tempIV.size(); i++)
		{
			// Assign our actual vertices vector to the indexes we received from the faces (temp vectors)
			unsigned int vertexI = tempIV.at(i);
			glm::vec3 vert = tempVerts[vertexI - 1];
			_vertices.push_back(vert);
		}
		for (int i = 0; i < tempIN.size(); i++)
		{
			// Same for uvs
			unsigned int uvI = tempIN.at(i);
			glm::vec2 uv = tempUv[uvI - 1];
			_uvs.push_back(uv);
		}
		for (int i = 0; i < tempIT.size(); i++)
		{
			// Same for normals
			unsigned int normalI = tempIT.at(i);
			glm::vec3 norm = tempNormals[normalI - 1];
			_normals.push_back(norm);
		}
		// Clear out all those temporary vectors to save memory
		tempIV.clear();
		tempIN.clear();
		tempIT.clear();
		tempVerts.clear();
		tempNormals.clear();
		tempUv.clear();
	}
	else
	{
		throw("Unable to find the specified .obj file.\nHave you incorrectly spelled the file name?");
	}
}


void Model::initialzeModel()
{
	_vao = 0;
	// Creates one VAO for our object
	glGenVertexArrays(1, &_vao);

	// Bind out VAO in memory and "turning it on"
	glBindVertexArray(_vao);

	// Vertex buffer
	GLuint bufferV = 0;
	glGenBuffers(1, &bufferV);
	glBindBuffer(GL_ARRAY_BUFFER, bufferV);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Assign the data with the size of our vertices
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * _vertices.size(), &_vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized
		0,                  // stride
		0					// array buffer offset
	);
	// Activate it
	glEnableVertexAttribArray(0);

	// UV
	if (_uvs.size() > 0)
	{
		// Same principle as our vertices but don't activate unless we actually have loaded UVs
		// OpenGL will error if you don't have any data to buffer
		// Our attribute this time is 1 which changes our layout input location in the shader
		GLuint bufferU = 0;
		glGenBuffers(1, &bufferU);
		glBindBuffer(GL_ARRAY_BUFFER, bufferU);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * _uvs.size(), &_uvs.front(), GL_STATIC_DRAW);
		glVertexAttribPointer(
			1,                  // attribute
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			0            // array buffer offset
		);
		glEnableVertexAttribArray(1);
	}


	// Normals
	if (_normals.size() > 0)
	{
		// Same once again
		// Attribute this time is 2
		GLuint bufferN = 0;
		glGenBuffers(1, &bufferN);
		glBindBuffer(GL_ARRAY_BUFFER, bufferN);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * _normals.size(), &_normals.front(), GL_STATIC_DRAW);
		glVertexAttribPointer(
			2,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			0            // array buffer offset
		);
		glEnableVertexAttribArray(2);
	}

	// Bind our array buffer and vertex array and send them into GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Disable all our cached vertex arrays
	glDisableVertexAttribArray(0);
	if (_uvs.size() > 0)
	{
		glDisableVertexAttribArray(1);
	}
	if (_normals.size() > 0)
	{
		glDisableVertexAttribArray(2);
	}
}

void Model::drawModel()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GREATER);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	glEnable(GL_LINE_SMOOTH);
	// Activate the VAO
	glBindVertexArray(_vao);

	// Tell OpenGL to draw it
	// Triangles are drawn based on the size of our vertices
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei) _vertices.size());

	// Unbind VAO
	glBindVertexArray(0);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
}