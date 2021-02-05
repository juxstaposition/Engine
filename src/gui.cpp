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

    ImGui::SetNextWindowPos(ImVec2(0, _windowSize.x - 210.f));
    ImVec2 windowSize(300.f, 210.f);

    ImGui::SetNextWindowSize(windowSize);
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoDecoration
        | ImGuiWindowFlags_AlwaysAutoResize
        | ImGuiWindowFlags_NoSavedSettings
        | ImGuiWindowFlags_NoBackground;

    ImGui::Begin("Video Handling", nullptr, flags);
    {
        //ImGuiIO& io = ImGui::GetIO();
        //io.Fonts->AddFontDefault();
        //ImTextureID btnTexId = io.Fonts->TexID;
        //float my_tex_w = (float)io.Fonts->TexWidth;
        //float my_tex_h = (float)io.Fonts->TexHeight;
        //ImVec2 size = ImVec2(32.0f, 32.0f);                     
        //ImVec2 uv0 = ImVec2(0.0f, 0.0f);                        
        //ImVec2 uv1 = ImVec2(32.0f / my_tex_w, 32.0f / my_tex_h);
        //ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);         
        //ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);     

        //ImGui::PushID(" PlayIcon ");
        //ImGui::ImageButton(btnTexId, size, uv0, uv1, 0, bg_col, tint_col);
        //ImGui::PopID();


        ImGui::PushID(" DetectPlayer ");
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.7f, 1.f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.7f, 0.8f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.7f, 1.f, 0.5f));
        if (ImGui::Button((_playerHighlighting) ? "Hide player highlighting" : "Show player highlighting")) {
            _playerHighlighting = !_playerHighlighting;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();

        ImGui::PushID(" DrawArrows ");
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.7f, 1.f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.7f, 0.8f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.7f, 1.f, 0.5f));
        if (ImGui::Button((_arrowDrawing) ? "Stop drawing arrows" : "Start drawing arrows")) {
            _arrowDrawing = !_arrowDrawing;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();

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

    ImGui::SliderInt("Slider1", &_slider1, 0, 1000);
    ImGui::SliderInt("Slider2", &_slider2, 0, 1000);
    ImGui::SliderInt("Slider3", &_slider3, 0, 1000);

    ImGui::RadioButton("11", &_blockSize, 11); ImGui::SameLine();
    ImGui::RadioButton("21", &_blockSize, 21); ImGui::SameLine();
    ImGui::RadioButton("31", &_blockSize, 31); ImGui::SameLine();
    ImGui::RadioButton("41", &_blockSize, 41); ImGui::SameLine();
    ImGui::RadioButton("55", &_blockSize, 55);

    ImGui::RadioButton("Original Video", &_imgView, 0); ImGui::SameLine();
    ImGui::RadioButton("Threshold", &_imgView, 1); ImGui::SameLine();
    ImGui::RadioButton("Masking", &_imgView, 2);

    //ImGui::Text("Gui calc fps %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("");
}