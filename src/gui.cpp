#include "gui.hpp"


Gui::Gui()
{
}

Gui::~Gui()
{

}

void Gui::Init(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);

    //ImGui_ImplOpenGL3_Init("#version 450");

    for (int i = 1; i <= sliderSize; i++)
    {
        _slider.push_back(0);
    }
}


int Gui::GetSliderVal(int index) 
{ 
    if (index < sliderSize)
    {
        return _slider.at(index); 
    }
    return -1;
}

void Gui::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Gui::NewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (_debug)
    {
        DebugWindowDraw();
    }
    VideoStateHandling();

    ImGui::End();
}

void Gui::Render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::VideoStateHandling()
{

    ImGui::SetNextWindowPos(ImVec2(_windowSize.x / 2.f - 200.f, 0.f ));
    ImVec2 windowSize(500.f, 210.f);

    ImGui::SetNextWindowSize(windowSize);
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoDecoration
        | ImGuiWindowFlags_AlwaysAutoResize
        | ImGuiWindowFlags_NoSavedSettings
        | ImGuiWindowFlags_NoBackground;

    ImGui::Begin("Video Handling", nullptr, flags);
    {
        ImGui::PushID(" DetectPlayer ");
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.7f, 1.f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.7f, 0.8f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.7f, 1.f, 0.5f));
        if (ImGui::Button((_playerHighlighting) ? "Hide player highlighting" : "Show player highlighting")) {
            _playerHighlighting = !_playerHighlighting;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();ImGui::SameLine();

        ImGui::PushID(" DrawArrows ");
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.7f, 1.f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.7f, 0.8f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.7f, 1.f, 0.5f));
        if (ImGui::Button((_arrowDrawing) ? "Stop drawing arrows" : "Start drawing arrows")) {
            _arrowDrawing = !_arrowDrawing;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();ImGui::SameLine();

        ImGui::PushID(" Play ");
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.7f, 1.f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.7f, 0.8f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.7f, 1.f, 0.5f));
        if (ImGui::Button((_running) ? "pause" : "start")) { // 
            _running = !_running;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();

    }
    ImGui::End();

}


void Gui::DebugWindowDraw()
{

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Debug Window");                          // Create a window called "Hello, world!" and append into it.

    for (int i = 0; i < sliderSize; i++)
    {
        int maxVal = 10;
        if (i < 3) maxVal = 100;
        int minVal = 0;
        if (i < 3) minVal = -100;
        std::string SliderName = std::string("Slider") + std::to_string(i);
        ImGui::SliderInt(SliderName.c_str(), &_slider.at(i), minVal, maxVal);
    }

    ImGui::RadioButton("x", &_blockSize, 11); ImGui::SameLine();
    ImGui::RadioButton("y", &_blockSize, 21); ImGui::SameLine();
    ImGui::RadioButton("z", &_blockSize, 31);

    ImGui::RadioButton("Original Video", &_imgView, 0); ImGui::SameLine();
    ImGui::RadioButton("Threshold", &_imgView, 1); ImGui::SameLine();
    ImGui::RadioButton("Masking", &_imgView, 2);

    //ImGui::Text("Gui calc fps %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("");
}