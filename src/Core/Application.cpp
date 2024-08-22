#include "GPU-Side/ShaderClass.h"
#include "Application.h"
#include <iostream>
Chess_Game::Application::Application()
{
    constexpr int kStartWindowWidth = 600;
    constexpr int kStartWindowHeight = 800;
    const char* kWindowTittle = "Chess";

    Chess_Game::WindowCreateInfo window_create_info{};
    window_create_info.windowHeight = kStartWindowHeight;
    window_create_info.windowWidth = kStartWindowWidth;
    window_create_info.windowTittle = kWindowTittle;
    window_create_info.windowOnEventCallback = std::bind(&Application::OnEvent, this, std::placeholders::_1);
   

    m_ApplicationWindow = std::make_unique<Window>(window_create_info);
    if (!m_ApplicationWindow->IsWindowValid())
        m_ApplicationInitStatus = false;

    glViewport(0, 0, kStartWindowWidth, kStartWindowHeight);
}

void Chess_Game::Application::RenderLoop()
{
    ShaderClass test_shader_class("D:/c++/OpenGl/Chess-OpenGL/Shaders/TestShader.glsl");
    

    while (m_isApplicationRunning) {
        glClear(GL_COLOR_BUFFER_BIT);

        glClearColor(1.f, 0.f, 0.f, 1.0f);

        m_ApplicationWindow->OnUpdate();
    }

}

void Chess_Game::Application::OnEvent(int test_value)
{
    m_isApplicationRunning = false;
}
