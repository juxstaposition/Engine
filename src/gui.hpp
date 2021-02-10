#pragma once


#include <cstdlib>
#include <string>
#include <imgui.h>
#include <vector>

#include <GLFW/glfw3.h>
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

	int GetSliderVal(int index);
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
	int sliderSize = 8;
	std::vector<int> _slider;
	int _imgView = ORIGINALVIDEO;
	int _blockSize = 11;

	bool _running = true;
	bool _playerHighlighting = true;
	bool _arrowDrawing = true;
};

