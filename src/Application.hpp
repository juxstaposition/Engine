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
#include "VideoLoader/Videoloader.hpp"

class Application
{
public:
	Application();
	~Application();
	void start();

	WindowHandler* windowHandler = nullptr;

private:
	void initGLFW();
	void initGLEW();
	void initOpenGL(int w, int h);

	void CreateShader();

	static void ErrorCallback(int error, const char* description);
	static void resizeWindow(GLFWwindow* window, int width, int height);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseButtonnCallback(GLFWwindow* window, int button, int action, int mods);
	
	void LockFrameRate(double frame_rate);

	//GLFWwindow* _window = nullptr;
	VideoLoader* _video = nullptr;
	Shader* _shader = nullptr;
	Model _circle, _arrow;


	glm::vec3 _light = glm::vec3(0.f, 3000.f, 3500.f);
	glm::vec3 _arrowColor = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 _circleColor = glm::vec3(0.83f, 0.83f, 0.83f);

	float cameraRotX, cameraRotY, cameraZoom, sceneLight, rColour, gColour, bColour;

	GLuint _viewMatLoc, _projMatLoc, _modelMatLoc, _lightVecLoc, _cameraVecLoc, _colorVecLoc;

	glm::mat4 _modelMatrix, _viewMatrix, _projMatrix;

	// Frame counting and limiting
	int    _frameCount = 0;
	double _frameStartTime, _frameEndTime, _frameDrawTime;
};

