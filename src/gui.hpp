#pragma once


#include <GLFW/glfw3.h>
#include <cstdlib>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "settings.hpp"


class Gui
{
public:
	Gui();
	~Gui();
	void Init(GLFWwindow* window);
	void NewFrame();
	void CleanUp();
	void Render();

	int GetValSlider1() { return _slider1; }
	int GetValSlider2() { return _slider2; }
	int GetValSlider3() { return _slider3; }
	int GetViewType() { return _imgView; }
	int GetBlockSize() { return _blockSize; }
	void SetWindowSize(float w, float h) { _windowSize.x = w; _windowSize.y = h; }
	bool GetVideoState() { return _running; }
	bool GetPlayerHighliting() { return _playerHighlighting; }
	bool GetArrow() { return _arrowDrawing; }
	static const int ORIGINALVIDEO = 0;
	static const int THRESHOLDVIDEO = 1;
	static const int MASKINGVIDEO = 2;

private:
	void DebugWindowDraw();
	void VideoStateHandling();

	ImVec2 _windowSize;
	bool _debug = DEBUG;
	bool _firstFrame = true;


	// Adjustable values
	int _slider1 = 25;
	int _slider2 = 70;
	int _slider3 = 700;
	int _imgView = THRESHOLDVIDEO;
	int _blockSize = 11;

	bool _running = true;
	bool _playerHighlighting = true;
	bool _arrowDrawing = true;
};

