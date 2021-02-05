#include "WindowHandler.hpp"

WindowHandler::WindowHandler(GLFWwindow* window)
{
	_window = window;

	glfwSetWindowUserPointer(_window, reinterpret_cast<void*>(this));
}
WindowHandler::~WindowHandler()
{
	if (_window != nullptr)
		glfwDestroyWindow(_window);

	gui.CleanUp();
}