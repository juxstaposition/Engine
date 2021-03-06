#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <Windows.h>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <opencv2/opencv.hpp>

#include "OrtographicCamera.hpp"
#include "gui.hpp"
#include "Model.hpp"
#include "Shader.hpp"
#include "settings.hpp"
#include "WindowHandler.hpp"
#include "GraphicObject.hpp"
#include "Materials.hpp"
#include "VideoLoader/Videoloader.hpp"



class Application
{
public:
	Application();
	~Application();
	void start();

	WindowHandler* windowHandler = nullptr;

private:
	struct UniformLocations {
		GLuint viewMat;
		GLuint projMat;
		GLuint modelMat;
		GLuint lightVec;
		GLuint cameraVec;
		GLuint colorVec;
		GLuint ambient;
		GLuint shininess;
		GLuint specularColor;
		GLuint lightSpaceMat;
	};


	void initGLFW();
	void initGLEW();
	void initOpenGL(int w, int h);
	void initDepthMap();

	void CreateObjectShader(std::string vname, std::string fname);
	void CreateDepthShader(std::string vname, std::string fname);

	static void ErrorCallback(int error, const char* description);
	static void ResizeWindow(GLFWwindow* window, int width, int height);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseButtonnCallback(GLFWwindow* window, int button, int action, int mods);
	
	void LockFrameRate(double frame_rate);


	void SetObjectMaterialUniforms(GraphicObject* obj, uint8_t instance);
	void SetObjectModelMatUniform(GraphicObject* obj, uint8_t instance);
	
	VideoLoader* _video = nullptr;
	Shader* _objectShader = nullptr;
	Shader* _depthShader = nullptr;
	GraphicObject* _circle = nullptr;
	GraphicObject* _arrow = nullptr;

	UniformLocations _uniform;

	glm::vec3 _light = glm::vec3(0.f, 0.f, 0.f);
	

	unsigned int depthMapFBO = 0;
	unsigned int depthMap = 0;
	// Frame counting and limiting
	int    _frameCount = 0;
	double _frameStartTime, _frameEndTime, _frameDrawTime;
};
