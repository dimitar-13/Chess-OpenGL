#include "GPU-Side/ShaderClass.h"
#include "Application.h"
#include <iostream>
#include "GPU-Side/BatchRenderer.h"
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
    
    BatchRenderer batch_renderer_test{};
   
    //batch_renderer_test.Push({ 3,2 }, { 1.0f,1.0f,0.0f });

    while (m_isApplicationRunning) {
        glClear(GL_COLOR_BUFFER_BIT);


        batch_renderer_test.Push({ -40,0 }, { 1.0f,1.0f,1.0f });
        batch_renderer_test.Push({ -30,0 }, { 1.0f,0.0f,1.0f });
        batch_renderer_test.Push({ -20,0 }, { 2.0f,0.0f,1.0f });
        batch_renderer_test.Push({ -10,0 }, { 0.0f,0.0f,1.0f });
        batch_renderer_test.Push({ 00,0 }, { 0.0f,0.0f,1.0f });
        batch_renderer_test.Push({ 10,0 }, { 0.0f,0.0f,1.0f });
        batch_renderer_test.Push({ 20,0 }, { 0.0f,0.0f,1.0f });
        batch_renderer_test.Push({ 30,0 }, { 0.0f,0.0f,1.0f });


        glClearColor(1.f, 0.f, 0.f, 1.0f);

        test_shader_class.UseProgram();

        batch_renderer_test.Flush();


        m_ApplicationWindow->OnUpdate();
    }

}

void Chess_Game::Application::OnEvent(int test_value)
{
    m_isApplicationRunning = false;
}
