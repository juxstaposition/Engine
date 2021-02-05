#pragma once

#include <GLFW/glfw3.h>
#include "OrtographicCamera.hpp"
#include "gui.hpp"

class WindowHandler
{
public:
	WindowHandler(GLFWwindow* window);
	~WindowHandler();

	GLFWwindow* GetWindow() { return _window; }

	int width;
	int height;
	Gui gui;

private:
	GLFWwindow* _window = nullptr;
};

