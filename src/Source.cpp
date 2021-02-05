#include "Videoloader.hpp"

/**
* @brief VideoLoader Constructor
*/
VideoLoader::VideoLoader()
{
    _capture = new cv::VideoCapture(videoPath);
    if (!_capture->isOpened()) {
        std::cerr << "Cannot open video: " << videoPath << std::endl;

    }

    _fps = _capture->get(cv::CAP_PROP_FPS);
    if (_fps > 0 && _fps < 1000)
    {
        _fps = 25.0;
    }

    _shader = new Shader();
    _shader->addFileNames("./src/VideoLoader/vsBackgroundTextures.glsl", "./src/VideoLoader/fsBackgroundTextures.glsl");
    _shader->compileShaders();
    glUseProgram(_shader->getShaderProg());
    _maskTextureLoc = _shader->getULocation("Texture0");
    _foregroundTextureLoc = _shader->getULocation("Texture1");
    //_backgroundTextureLoc = _shader->getULocation("Texture1");
    glUseProgram(0);


    float vertices[] = {
        // positions                                      // texture coords
         0.0f,  0.0f, 0.0f,                               0.0f, 0.0f, // bottom left
         (float)getVideoWidth(), 0.0f, 0.0f,              1.0f, 0.0f, // bottom right
         (float)getVideoWidth(), (float)getVideoHeight(), 0.0f,  1.0f, 1.0f, // top right
         0.0f,(float)getVideoHeight(), 0.0f,              0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

}

/**
* @brief VideoLoader Destructor
*/
VideoLoader::~VideoLoader()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    delete _shader;
    delete _capture;
}

/**
* @brief Return video width
*/
int VideoLoader::getVideoWidth()
{
    return (int)_capture->get(cv::CAP_PROP_FRAME_WIDTH);
}

/**
* @brief Return video height
*/
int VideoLoader::getVideoHeight()
{
    return (int)_capture->get(cv::CAP_PROP_FRAME_HEIGHT);
}

/**
* @brief Return video fps value
*/
double VideoLoader::getFPS()
{
    return _fps;
}


/**
* @brief Loads a new frame
*/
bool VideoLoader::loadFrame(Gui& gui)
{

    // if user enables video play, set video being continues
    // for last texture deleting and calculate new texture
    if (gui.GetVideoState() && _isPaused)
    {
        _isPaused = false;
        _isResumed = true;
    }
    // if video running calculate new texture
    else if (gui.GetVideoState() && !_isPaused)
    {
        _isResumed = false;
    }
    // if user stops video, load a new texture but disable deleting it
    else if (!gui.GetVideoState() && !_isPaused)
    {
        _isPaused = true;
        _pauseDetected = true;
    }
    // if video is stopped
    else if (!gui.GetVideoState() && _isPaused)
    {
        _pauseDetected = false;
    }

    // if video is running or pause was detected load a new frame
    if (!_isPaused || _pauseDetected)
    {
        if (!_capture->read(_frame))
        {
            std::cerr << "Cannot grab a frame." << std::endl;
            return false;
        }
    }

    return true;
}

/**
* @brief Converts OpenCv Mat to OpenGL texture
*/
void VideoLoader::matToTexture(const cv::Mat& mat, GLuint& textureID, GLenum minFilter, GLenum magFilter, GLenum wrapFilter)
{
    // TODO add OpenCV mat inversion, so it can be enabled in higher
    // opengl versions

    glEnable(GL_TEXTURE_2D);
    // Create Texture ID in Opengl
    glGenTextures(1, &textureID);

    // Bind to texture handle
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Texture interpolation method for magnification
    if (magFilter == GL_LINEAR_MIPMAP_LINEAR ||
        magFilter == GL_LINEAR_MIPMAP_NEAREST ||
        magFilter == GL_NEAREST_MIPMAP_LINEAR ||
        magFilter == GL_NEAREST_MIPMAP_NEAREST)
    {
        std::cerr << "MIPMAPs cannot be used for magnification" << std::endl;
        magFilter = GL_LINEAR;
    }

    // Set texture interpolation methods for minification and magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

    // Set texture clamping method
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapFilter);

    // Set input texture format
    GLenum inputColourFormat = GL_BGR;
    if (mat.channels() == 1)
    {
        inputColourFormat = GL_LUMINANCE;
    }
    // Create the texture
    glTexImage2D(GL_TEXTURE_2D,     // Texture type
        0,                 // Pyramid level
        GL_RGB,            // Output color format
        mat.cols,          // Image width
        mat.rows,          // Image height
        0,                 // Width of border 0/1
        inputColourFormat, // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
        GL_UNSIGNED_BYTE,  // Image data type
        mat.ptr());        // Image data itself

    // Generate mipmaps
    if (minFilter == GL_LINEAR_MIPMAP_LINEAR ||
        minFilter == GL_LINEAR_MIPMAP_NEAREST ||
        minFilter == GL_NEAREST_MIPMAP_LINEAR ||
        minFilter == GL_NEAREST_MIPMAP_NEAREST)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glDisable(GL_TEXTURE_2D);

}


/**
* @brief Enables Opengl masking
*/
void VideoLoader::enableMasking()
{
    // Use a simple blendfunc for drawing the background
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ZERO);
}

/**
* @brief Draws background frame
*/
void VideoLoader::drawVideoBackgroundFrame(int& windowWidth, int& windowHeight)
{

    glMatrixMode(GL_MODELVIEW);

    // Stop generating new background if video is stopped
    if (!_isPaused || _pauseDetected)
    {
        matToTexture(_frame, _backgroundTexture, GL_NEAREST, GL_LINEAR, GL_CLAMP);
    }

    glEnable(GL_TEXTURE_2D);
    // Set texture as background
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_ONE, GL_ZERO);

    DrawTexture(windowWidth, windowHeight, _backgroundTexture);

    //glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

    //// Does not yet work
    //// drawing background with shader
    //glUseProgram(videoShader->getShaderProg());

    //glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, glm::value_ptr(view));
    //glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, glm::value_ptr(proj));

    //// Draw the background

    //// Activate the texture
    //glBindTexture(GL_TEXTURE_2D, image_tex);
    //// Activate the VAO
    //glBindVertexArray(vao);

    //// Tell OpenGL to draw it
    //// Triangles are drawn based on the size of our vertices
    //glDrawArrays(GL_TRIANGLES, 0, (GLsizei)backgroundVerticesSize);

    //// Unbind VAO
    //glBindVertexArray(0);

    //// Stop using the program and object drawing
    //glUseProgram(0);
}


/**
* @brief Draws texture in a video dimensions
* @param textureId Gluint ID of a texture
*/
void VideoLoader::DrawTexture(int& windowWidth, int& windowHeight, GLuint textureId)
{
    // Bind to  texture handle
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Draw a quad 
    glBegin(GL_QUADS);
    glTexCoord2i(0, 0); glVertex2i(0, 0);
    glTexCoord2i(1, 0); glVertex2i(windowWidth, 0);
    glTexCoord2i(1, 1); glVertex2i(windowWidth, windowHeight);
    glTexCoord2i(0, 1); glVertex2i(0, windowHeight);
    glEnd();

}

/**
* @brief Deletes textures if it wont be used anymore
*/
void VideoLoader::DeleteTexture(GLuint& textureId)
{
    // Dont delete texture if video is stopped
    if (_isPaused && !_isResumed)
    {
        return;
    }
    glDeleteTextures(1, &textureId);
}

/**
* @brief Draws mask frame into alpha buffer or shows into the screen for debbuing
*/
void VideoLoader::drawMaskFrame(Gui& gui, int& windowWidth, int& windowHeight, int& lowTh, int& hightTh)
{
    // disable extra operations if no objects to be drawn
    if (gui.GetViewType() != gui.ORIGINALVIDEO)
    {
        if (!_maskCalculated)
        {
            // create mask
            cv::Mat fieldAreaMask, imgFieldMasked, mog2Mask, result;

            FloodFill(_frame, fieldAreaMask, 25, 70);
            //cv::bitwise_and(_frame, _frame, imgFieldMasked, fieldAreaMask);

            //MOG2(imgFieldMasked, mog2Mask);
            //cv::bitwise_and(_frame, _frame, result, mog2Mask);

            // Create mask texture is video is playing, this doesn't produce a visible result, it just
            // changes the alpha values in the framebuffer
            if ((!_isPaused || _pauseDetected))
            {
                matToTexture(fieldAreaMask, _thresholdTexture, GL_NEAREST, GL_LINEAR, GL_CLAMP);
                _maskCalculated = true;
            }
        }

        if (gui.GetViewType() == gui.THRESHOLDVIDEO)
        {
            glEnable(GL_TEXTURE_2D);
            DrawTexture(windowWidth, windowHeight, _thresholdTexture);
            glDisable(GL_TEXTURE_2D);
        }

        if (gui.GetViewType() == gui.MASKINGVIDEO)
        {
            // Finally, draw foreground texture visible only in areas with high alpha.
            glEnable(GL_TEXTURE_2D);
            glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);

            DrawTexture(windowWidth, windowHeight, _backgroundTexture);

            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);


            // Drawing textures with shader
            //glEnable(GL_TEXTURE_2D);
            //glEnable(GL_BLEND);
            //glUseProgram(_shader->getShaderProg());

            //glActiveTexture(GL_TEXTURE0);
            //glBindTexture(GL_TEXTURE_2D, _thresholdTexture);
            //glActiveTexture(GL_TEXTURE1);
            //glBindTexture(GL_TEXTURE_2D, _backgroundTexture);

            //glBindVertexArray(VAO);
            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            //glUseProgram(0);

            //glDisable(GL_BLEND);
            //glDisable(GL_TEXTURE_2D);

        }

        DeleteTexture(_thresholdTexture);
    }

    DeleteTexture(_backgroundTexture);

    if (_isResumed || !_isPaused)
    {
        _maskCalculated = false;
    }
}



void VideoLoader::MOG2(cv::Mat& img, cv::Mat& mog2foregroundMask)
{
    mog2BackgSubs->apply(img, mog2foregroundMask);
}



void VideoLoader::ChromaKey(cv::Mat& img, cv::Mat& result, int thLow, int thHigh)
{
    cv::Mat hsv, thresh, chromaMask, chromaMaskDilate, chromaMaskINV;
    cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);

    cv::Vec3b hsvPixel(hsv.at<cv::Vec3b>(0, 0));

    cv::inRange(hsv, cv::Scalar(25, 20, 20), cv::Scalar(80, 255, 255), chromaMask);


    cv::threshold(chromaMask, thresh, 127, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);

    int kernelSize = 11;
    cv::Mat kernelElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelSize, kernelSize));
    // dilate to get almost proper player mask joined with background outside field
    cv::dilate(thresh, chromaMaskDilate, kernelElement);

    //inverte for field mask
    //cv::bitwise_not(chromaMaskDilate, chromaMaskINV);

    // dilate again for field mask
    //cv::dilate(chromaMaskINV, chromaMaskINV, kernelElement);

    // find biggest contour
    //std::vector<std::vector<cv::Point>> contours;
    //std::vector<cv::Vec4i> hierarchy;
    //findContours(chromaMaskCannyINV, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    ////find biggest contour area
    //double maxArea = 0;
    //int maxAreaId = 0;
    //for (int i = 0; i < contours.size(); i++)
    //{
    //    double area = contourArea(contours[i], false); 
    //    if (area > maxArea)
    //    {
    //        maxArea = area;
    //        maxAreaId = i;
    //    }
    //}

    //cv::drawContours(result, contours, maxAreaId, cv::Scalar(255, 255, 255), cv::FILLED, 8, hierarchy);
    //cv::bitwise_and(img, img, result, contourMask);


    result = chromaMaskDilate.clone();

}


void VideoLoader::FloodFill(cv::Mat& img, cv::Mat& result, int thLow, int thHigh)
{
    cv::Mat imgCopy = img.clone();
    cv::Mat mask = cv::Mat::zeros(img.rows + 2, img.cols + 2, CV_8UC3);
    cv::Mat resizedMask;

    int floodFlags = 4;
    floodFlags |= (255 << 8);
    int diff = 11;
    cv::Point point1 = cv::Point((int)getVideoWidth() / 2 + 10, (int)getVideoHeight() / 2);
    cv::Point point2 = cv::Point((int)getVideoWidth() / 2 - 10, (int)getVideoHeight() / 2);

    int filling;
    filling = cv::floodFill(imgCopy, mask, point1,
        cv::Scalar(255, 255, 255), (cv::Rect*)0, cv::Scalar(diff, diff, diff, diff), cv::Scalar(diff, diff, diff, diff), floodFlags);
    filling = cv::floodFill(imgCopy, mask, point2,
        cv::Scalar(255, 255, 255), (cv::Rect*)0, cv::Scalar(diff, diff, diff, diff), cv::Scalar(diff, diff, diff, diff), floodFlags);

    cv::resize(mask, resizedMask, cv::Size(img.rows, img.cols));


    // dilate to get almost proper player mask joined with background outside field
    cv::dilate(resizedMask, result, _kernelElement);

    //cv::bitwise_not(resizedMask, result);

    // result = resizedMask.clone();

}