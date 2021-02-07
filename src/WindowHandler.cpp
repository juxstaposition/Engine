#include "WindowHandler.hpp"

WindowHandler::WindowHandler(GLFWwindow* window)
{
	_window = window;

	glfwSetWindowUserPointer(_window, reinterpret_cast<void*>(this));

	_ortographicCamera = new OrtographicCamera(-1.0f, 1.0f, -1.0f, 1.0f);
}
WindowHandler::~WindowHandler()
{
	if (_window != nullptr)
		glfwDestroyWindow(_window);

	gui.CleanUp();
}