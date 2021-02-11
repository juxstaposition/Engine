#include "Application.hpp"

/**
* @brief Application constructor
*/
Application::Application()
{
    glfwSetErrorCallback(ErrorCallback);

    initGLFW();
    initGLEW();
    initOpenGL(windowHandler->width, windowHandler->height);
    windowHandler->gui.Init(windowHandler->GetWindow());

    // Create and compile shaders
    _objectShader = new Shader();
    CreateObjectShader("./src/shaderVertex.glsl", "./src/shaderFragment.glsl");
    //_depthShader = new Shader();
    //CreateDepthShader("./src/vsShadowMappingDepth.glsl", "./src/fsShadowMappingDepth.glsl");

    // Create objects
    _circle = new GraphicObject("./objects3D/circle2.obj");
    _arrow = new GraphicObject("./objects3D/arrow.obj");

    // create video handler
    _video = new VideoLoader();

    // resize window based on dimension ratio of the video
    int videoWidth = _video->getVideoWidth();
    int videoHeight = _video->getVideoHeight();
    float videoDim = (float) videoWidth / videoHeight;
    float windowDim = (float) windowHandler->width / windowHandler->height;
    
    // resize if aspect ratios are not the same
    if (fabs(videoDim + windowDim) > 0.0001)
    {
        windowHandler->height = static_cast<int>(windowHandler->width / videoDim);
    }

    glfwSetWindowSize(windowHandler->GetWindow(), windowHandler->width, windowHandler->height);

    windowHandler->gui.SetWindowSize((float)windowHandler->width, (float)windowHandler->height);
    // change project matrix based on new dimensions
    windowHandler->GetCamera()->resize(0.f, (float)windowHandler->width, 0.f, (float)windowHandler->height);
}

/**
* @brief Application destructor
*/
Application::~Application()
{

    if (_video != nullptr)
        delete _video;

    if (_circle != nullptr)
        delete _circle;

    if (_arrow != nullptr)
        delete _arrow;

    if (_objectShader != nullptr)
        delete _objectShader;
    
    if (windowHandler != nullptr)
        delete windowHandler;
    
    glfwTerminate();
}


/**
* @brief Resize Opengl Window Callback for GLFW
*/
void Application::ResizeWindow(GLFWwindow* window, int width, int height)
{
    WindowHandler* win = reinterpret_cast<WindowHandler*>(glfwGetWindowUserPointer(window));
    win->width = width;
    win->height = height;
    glMatrixMode(GL_PROJECTION);
    glViewport(0, 0, win->width, win->height);
    glLoadIdentity();
    glOrtho(0.0, win->width, win->height, 0.0, 0.0, 100.0);
    glMatrixMode(GL_MODELVIEW);

    win->gui.SetWindowSize((float)win->width, (float)win->height);
    //win->GetCamera()->resize(0, (float)win->width, 0, (float)win->height);
}


/**
* @brief GLFW mouse button click callback
*/
void Application::MouseButtonnCallback(GLFWwindow* window, int button, int action, int mods)
{
    WindowHandler* win = reinterpret_cast<WindowHandler*>(glfwGetWindowUserPointer(window));

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
    }
}

/**
* @brief GLFW error callback
*/
void Application::ErrorCallback(int error, const char* description)
{
    std::cerr << "Error: " << description << std::endl;
}


/**
* @brief Keyboard input handling
*/
void Application::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    WindowHandler* win = reinterpret_cast<WindowHandler*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(win->GetWindow(), GLFW_TRUE);
    }


    const float moveSensitivityOrto = 5.f;

    // move camera up
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        win->GetCamera()->addYPosition(-moveSensitivityOrto);
    }

    // move camera down
    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        win->GetCamera()->addYPosition(moveSensitivityOrto);
    }

    // move camera right
    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        win->GetCamera()->addXPosition(moveSensitivityOrto);
    }

    // move camera left
    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        win->GetCamera()->addXPosition(-moveSensitivityOrto);
    }

    // increase camera disctance
    if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        win->GetCamera()->setDistance(1.5f);
    }

    // decrease camera distance
    if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        win->GetCamera()->setDistance(0.1f);
    }
}


/**
* @brief GLFW Initialization
*/
void Application::initGLFW()
{
    if (!glfwInit()) {
        std::cerr << "GLFW initiation error" << std::endl;
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_SAMPLES, 8);
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Rendering Engine", NULL, NULL);
    windowHandler = new WindowHandler(window);
    if (!window) {
        std::cerr << "GLFW Window creation error" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwGetWindowSize(window, &windowHandler->width, &windowHandler->height);

    glfwSetWindowPos(window, 0, 35);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    //glfwSetWindowSizeCallback(window, resizeWindow);
    glfwSetFramebufferSizeCallback(window, ResizeWindow);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonnCallback);
}

/**
* @brief GLEW initialization
*/
void Application::initGLEW()
{
    //  Initialise glew (must occur AFTER window creation or glew will error)
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cerr << "GLEW initialisation error: " << glewGetErrorString(err) << std::endl;
        exit(-1);
    }

}

/**
* @brief OpenGL initialization
*/
void Application::initOpenGL(int w, int h)
{
    // use a screen size of WIDTH x HEIGHT
    glViewport(0, 0, w, h); 

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, w, h, 0.0, 0.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}




/**
* @brief Inicialize uniform location IDs
*/
void Application::CreateObjectShader(std::string vname, std::string fname)
{
    // Compile the shaders and get all the memory locations for our uniform values within the shaders
    _objectShader->compileShaders(vname, fname);
    glUseProgram(_objectShader->getShaderProg());
    _uniform.projMat = _objectShader->getULocation("projMat");
    _uniform.viewMat = _objectShader->getULocation("viewMat");
    _uniform.modelMat = _objectShader->getULocation("modelMat");
    _uniform.cameraVec = _objectShader->getULocation("cameraPos");

    _uniform.colorVec = _objectShader->getULocation("objectColor");
    _uniform.lightVec = _objectShader->getULocation("lightPos");
    _uniform.ambient = _objectShader->getULocation("ambient");
    _uniform.shininess = _objectShader->getULocation("shininess");
    _uniform.specularColor = _objectShader->getULocation("specularColor");
    glUseProgram(0);
}

/**
* @brief Inicialize depth uniform location IDs
*/
void Application::CreateDepthShader(std::string vname, std::string fname)
{
    // Compile the shaders and get all the memory locations for our uniform values within the shaders
    _depthShader->compileShaders(vname, fname);

    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    //glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);




    glUseProgram(_depthShader->getShaderProg());

    //simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    _uniform.lightSpaceMat = _objectShader->getULocation("lightSpaceMat");
    _uniform.modelMat = _objectShader->getULocation("modelMat");

    //glUniform1i(glGetUniformLocation(_depthShader->getShaderProg(),  ), 0);
    



    glUseProgram(0);
}

/**
* @brief Slows video play speed based on video fps
* in case of video frames being proccessed too fast 
*/
void Application::LockFrameRate(double frame_rate) 
{
    // Note: frame_start_time is called first thing in the main loop
    _frameEndTime = glfwGetTime();  // in seconds
    _frameDrawTime = _frameEndTime - _frameStartTime;

    ++_frameCount;
    static double allowed_frame_time = 1.0 / frame_rate;
    double sleep_time = 0.0;
    

    if (_frameDrawTime < allowed_frame_time) {
        sleep_time = allowed_frame_time - _frameDrawTime;
        // TODO add multiplatform
        Sleep( (DWORD) (1000 * sleep_time) );
    }

    if (DEBUG)
    {
        // Debug stuff
        double potential_fps = 1.0 / _frameDrawTime;
        double locked_fps = 1.0 / (glfwGetTime() - _frameStartTime);
        std::cout << "Frame [" << _frameCount << "] ";
        std::cout << "Draw: " << _frameDrawTime << " Sleep: " << sleep_time;
        std::cout << " Pot. FPS: " << potential_fps << " Locked FPS: " << locked_fps << std::endl;
    }
}




/**
* @brief Application main loop
*/
void Application::start() 
{
    double video_start_time = glfwGetTime();

    // example of setting position of model by model matrix
    float delta = 0.f;
    glm::mat4 model1 = glm::translate(glm::mat4(1.0f), glm::vec3((float)windowHandler->width / 2 - 200.f , (float)windowHandler->height /2 - 100.f, 0.f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(-23.f), glm::vec3(1.0f, 0.0f, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 0.8f, 0.8f));

    glm::mat4 model2 = glm::translate(glm::mat4(1.0f), glm::vec3((float)windowHandler->width / 2 + 40.f, (float)windowHandler->height / 2 - 40.f, 0.f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.f), glm::vec3(-0.2f, 1.0f, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(100.f, 100.f, 100.f));

    for (float i = 0; i < 11;i++)
    {
        for (float j = 0; j < 2;j++)
        {
            glm::mat4 tmpMap = glm::translate(model1, glm::vec3( i*100.f ,j*50.f,0.f) );
            _circle->createObjectInstance(tmpMap, METALIC);
        }
    }

    _arrow->createObjectInstance(model2, EMERALD);


    while (!glfwWindowShouldClose(windowHandler->GetWindow()))
    {
        // load current time and scene
        _frameStartTime = glfwGetTime();

        // Start drawing
        // Clear color and depth buffers
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // update gui
        windowHandler->gui.NewFrame();

        // Load video frame
        if (!_video->loadFrame(windowHandler->gui))
        {
            // stop exectuion of main loop if video ends
            break;
        }

        // draw video texture as a background frame
        _video->drawVideoBackgroundFrame(windowHandler->width, windowHandler->height);

        // load projection matrices
        glm::mat4 proj = windowHandler->GetCamera()->getProjMatrix();
        glm::mat4 view = windowHandler->GetCamera()->getViewMatrix();
        glm::vec3 camera = glm::vec3(glm::inverse(view) * glm::vec4(0.f, 0.f, 0.f, 1.f));

        // object vertical movement, will be joined with matrix initialization
        float moveX = 1.f;
        if (delta > 180.f)
        {
            moveX = -1.f;
        }

        for (uint8_t i = 0;i < _circle->instances.size(); i++)
        {
            if (i < _circle->instances.size())
            {
                glm::mat4 tmpMat = glm::translate(glm::mat4(1.0f), glm::vec3(moveX, 0.f, 0.f))
                    * glm::rotate(_circle->instances.at(i).modelMat, glm::radians(5.f), glm::vec3(0.0f, 1.0f, 0.0f));
                _circle->setObjectMat(tmpMat, i);
            }
        }

        if (windowHandler->gui.GetSliderVal(5) != 0 ||
            windowHandler->gui.GetSliderVal(6) != 0 ||
            windowHandler->gui.GetSliderVal(7) != 0)
        {
            model2 = glm::rotate(model2, glm::radians(5.f), glm::vec3(
                (float)windowHandler->gui.GetSliderVal(5) / 10, 
                (float)windowHandler->gui.GetSliderVal(6) / 10,
                (float)windowHandler->gui.GetSliderVal(7) / 10) );
            _arrow->setObjectMat(model2, 0);
        }

        _light.x = (float)windowHandler->gui.GetSliderVal(0);
        _light.y = (float)windowHandler->gui.GetSliderVal(1);
        _light.z = (float)windowHandler->gui.GetSliderVal(2);
        _light *= 400;
        
        // load scene light from gui


        // Object Drawing
        glUseProgram(_objectShader->getShaderProg());

        // Pass uniform values into shaders
        glUniformMatrix4fv(_uniform.viewMat, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(_uniform.projMat, 1, GL_FALSE, glm::value_ptr(proj));
        glUniform3fv(_uniform.lightVec, 1, glm::value_ptr(_light));
        glUniform3fv(_uniform.cameraVec, 1, glm::value_ptr(camera));

        // Draw the models
        // show player highlighting if user enables this option
        if (windowHandler->gui.GetPlayerHighliting())
        {
            SetObjectMaterialUniforms(_circle, 0);
            for (uint8_t i = 0;i < _circle->instances.size(); i++)
            {
                SetObjectModelMatUniform(_circle, i);
                _circle->model.drawModel();
            }
        }

        // show arrow if user enables this option
        if (windowHandler->gui.GetArrow())
        {
            SetObjectMaterialUniforms(_arrow, 0);
            SetObjectModelMatUniform(_arrow, 0);
            _arrow->model.drawModel();
        }
        
        // Stop using shader
        glUseProgram(0);

        // Perform masking
        _video->drawMaskFrame(windowHandler->gui, windowHandler->width, windowHandler->height);

        LockFrameRate(_video->getFPS());
        delta += 5.f;
        if (delta > 360.f)
        {
            delta = 0.f;
        }


        // Render gui window
        windowHandler->gui.Render();

        // Swap window buffers
        glfwSwapBuffers(windowHandler->GetWindow());

        // Manage window events
        glfwPollEvents();
    }

    if (DEBUG)
    {
        std::cout << "Total video time: " << glfwGetTime() - video_start_time << " seconds" << std::endl;
    }
}



void Application::SetObjectMaterialUniforms(GraphicObject* obj, uint8_t instance)
{
    if (instance < obj->instances.size())
    {
        glUniform3fv(_uniform.colorVec, 1, glm::value_ptr(obj->instances.at(instance).material.diffuse));
        glUniform3fv(_uniform.ambient, 1, glm::value_ptr(obj->instances.at(instance).material.ambient));
        glUniform3fv(_uniform.specularColor, 1, glm::value_ptr(obj->instances.at(instance).material.specular));
        glUniform1f(_uniform.shininess, obj->instances.at(instance).material.shininess);
    }
}


void Application::SetObjectModelMatUniform(GraphicObject* obj, uint8_t instance)
{
    if (instance < obj->instances.size())
    {
        glUniformMatrix4fv(_uniform.modelMat, 1, GL_FALSE, glm::value_ptr(obj->instances.at(instance).modelMat));
    }
}