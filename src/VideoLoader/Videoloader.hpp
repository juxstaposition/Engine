#pragma once

#include <iostream>
#include <vector>


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#include <conio.h>  

#include "gui.hpp"
#include "settings.hpp"
#include "Shader.hpp"

class VideoLoader
{
public:
	VideoLoader();
	~VideoLoader();

	int getVideoWidth();
	int getVideoHeight();
	double getFPS();
	bool loadFrame(Gui& gui);
	void enableMasking();
	void drawVideoBackgroundFrame(int& windowWidth, int& windowHeight);
	void drawMaskFrame(Gui& gui, int &windowWidth, int &windowHeight, int& lowTh, int& hightTh);

	void DrawTexture(int& windowWidth, int& windowHeight, GLuint textureId);
	void DeleteTexture(GLuint& textureId);



private:	
	
	cv::VideoCapture* _capture = nullptr;
	cv::Mat _frame, _fieldArea;
	
	void matToTexture(const cv::Mat& mat, GLuint& textureID, 
		GLenum minFilter, GLenum magFilter, GLenum wrapFilter);

	cv::Ptr<cv::BackgroundSubtractor> mog2BackgSubs = cv::createBackgroundSubtractorMOG2(1000, 16, true);
	cv::Mat _kernelElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(11, 11));
	void MOG2(cv::Mat& img, cv::Mat& mog2foregroundMask);
	void ChromaKey(cv::Mat& img, cv::Mat& result, int thLow, int thHigh);
	void FloodFill(cv::Mat& img, cv::Mat& result, int thLow, int thHigh);

	Shader* _shader = nullptr;
	GLuint _backgroundTextureLoc, _maskTextureLoc, _foregroundTextureLoc;
	GLuint VBO = 0, VAO = 1, EBO = 2;
	
	bool _pauseDetected = false;
	bool _isPaused = false;
	bool _isResumed = false;
	bool _maskCalculated = false;

	GLuint _backgroundTexture = 0;
	GLuint _thresholdTexture = 1;
	double _fps = 0.0;

};

