#include "Shader.hpp"

/**
 * @brief Constructor
 */
Shader::Shader()
{
}
/**
 * @brief Destructor
 */
Shader::~Shader()
{
	_vertexCode.clear();
	_fragmentCode.clear();
}

/**
 * @brief Function return shader ID
 */
GLuint Shader::getShaderProg()
{
	return _shaderProgram;
};

/**
 *  Get file names for shader
 */
void Shader::addFileNames(std::string vname, std::string fname)
{
	_sPFragment = fname;
	_sPVertex = vname;
}


/// This section is based on
/// Opengl-tutorials (2011). Tutorial 2 : The first triangle [online]. [Accessed 2020]. 
/// Available from: <http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/>.
/**
 * @brief Function loads shader from a file
 */
void Shader::loadShaders()
{
	// Create the shaders in opengl
	_vertexID = glCreateShader(GL_VERTEX_SHADER);
	_fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

	_vertexCode = "";

	// Read from vertex shader file into vertex code string
	std::ifstream vertexShaderStream(_sPVertex, std::ios::in);
	if (vertexShaderStream.is_open())
	{
		std::string line = "";
		while (std::getline(vertexShaderStream, line))
			_vertexCode += "\n" + line;
		vertexShaderStream.close();
	}
	else
	{
		// Error if its not possible can't read the file
		throw("Cannot read read vertex shader file.\n");
	}

	// Clear the stream
	vertexShaderStream.clear();

	_fragmentCode = "";

	// Read from fragment shader file into vertex code string
	std::ifstream fragmentShaderStream(_sPFragment, std::ios::in);
	if (fragmentShaderStream.is_open())
	{
		std::string line = "";
		while (std::getline(fragmentShaderStream, line))
			_fragmentCode += "\n" + line;
		fragmentShaderStream.close();
	}
	else
	{
		// Error if its not possible can't read the file
		throw("Cannot read read fragment shader file.\n");
	}
}

/**
 * @brief Shader compilation
 */
void Shader::compileShaders()
{
	// Load the files holding the shaders
	this->loadShaders();

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader from the loaded vertex code
	char const* vPointer = _vertexCode.c_str();
	glShaderSource(_vertexID, 1, &vPointer, NULL);
	// Tell OpenGL to compile the shader at the memory location
	glCompileShader(_vertexID);

	// Check Vertex Shader
	if (!checkShaderCompiled(_vertexID))
	{
		throw("Vertex shader compiled incorrectly.\nCheck console for more information.");
	}

	// Compile Fragment Shader
	char const* fPointer = _fragmentCode.c_str();
	glShaderSource(_fragmentID, 1, &fPointer, NULL);
	glCompileShader(_fragmentID);

	// Check Vertex Shader
	if (!checkShaderCompiled(_fragmentID))
	{
		throw("Fragment shader compiled incorrectly.\nCheck console for more information.");
	}


	// Link the program
	_shaderProgram = glCreateProgram();
	// Attach the shaders we just made 
	glAttachShader(_shaderProgram, _vertexID);
	glAttachShader(_shaderProgram, _fragmentID);
	// Link the program we just made with the attached shaders
	glLinkProgram(_shaderProgram);

	// Check the program
	glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &Result);
	glGetProgramiv(_shaderProgram, GL_INFO_LOG_LENGTH, &InfoLogLength);

	// Certain graphics cards report a "0" as the info log length while others return blank
	if (InfoLogLength > 1)
	{
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(_shaderProgram, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("Program Error: %s\n", &ProgramErrorMessage[0]);
	}

	// No need to keep hold of the shader connection as it's loaded into the memory
	glDetachShader(_shaderProgram, _vertexID);
	glDetachShader(_shaderProgram, _fragmentID);

	// Delete the temporary data of the shaders
	glDeleteShader(_fragmentID);
	glDeleteShader(_vertexID);

}


/**
 * @brief Check if shaders were compiled successfully
 */
bool Shader::checkShaderCompiled(GLint shader)
{
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		int len;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

		// OpenGL will store an error message as a string that we can retrieve and print
		std::vector<char> VertexShaderErrorMessage(static_cast<unsigned __int64>(len) + 1 );
		glGetShaderInfoLog(_vertexShaderID, len, &len, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);

		return false;
	}
	return true;
}
/// End of citation

/**
 * @brief Function returns Location id of a shader uniform value
 */
int Shader::getULocation(std::string name)
{
	// Get the location of uniform in the shader
	return glGetUniformLocation(_shaderProgram, name.c_str());
}