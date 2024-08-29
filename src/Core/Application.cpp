#include "GPU-Side/ShaderClass.h"
#include "Application.h"
#include <iostream>
#include "GPU-Side/BatchRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Gameplay/ChessGame.h"


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
    CalculateOrthoProjection(Size2D{ kStartWindowWidth,kStartWindowHeight });
    ChessGame test_chess_board;


    //if (test_chess_board.CanMoveSelectedPiece({ 'c',3 }))
    //{
    //    test_chess_board.MoveSelectedPiece({ 'c',3 });
    //}
       // Simulate moves for Scholar's Mate
    test_chess_board.SelectPiece({ 'e', 2 }); // White pawn e2
    if (test_chess_board.CanMoveSelectedPiece({ 'e', 4 })) {
        test_chess_board.MoveSelectedPiece({ 'e', 4 });
    }

    test_chess_board.SelectPiece({ 'e', 7 }); // Black pawn e7
    if (test_chess_board.CanMoveSelectedPiece({ 'e', 5 })) {
        test_chess_board.MoveSelectedPiece({ 'e', 5 });
    }

    test_chess_board.SelectPiece({ 'd', 1 }); // White queen d1
    if (test_chess_board.CanMoveSelectedPiece({ 'h', 5 })) {
        test_chess_board.MoveSelectedPiece({ 'h', 5 });
    }

    test_chess_board.SelectPiece({ 'b', 8 }); // Black knight b8
    if (test_chess_board.CanMoveSelectedPiece({ 'c', 6 })) {
        test_chess_board.MoveSelectedPiece({ 'c', 6 });
    }

    test_chess_board.SelectPiece({ 'f', 1 }); // White bishop f1
    if (test_chess_board.CanMoveSelectedPiece({ 'c', 4 })) {
        test_chess_board.MoveSelectedPiece({ 'c', 4 });
    }

    test_chess_board.SelectPiece({ 'g', 8 }); // Black knight g8
    if (test_chess_board.CanMoveSelectedPiece({ 'f', 6 })) {
        test_chess_board.MoveSelectedPiece({ 'f', 6 });
    }

    // White's queen delivers checkmate
    test_chess_board.SelectPiece({ 'h', 5 }); // White queen h5
    if (test_chess_board.CanMoveSelectedPiece({ 'f', 7 })) {
        test_chess_board.MoveSelectedPiece({ 'f', 7 });
    }


}

void Chess_Game::Application::RenderLoop()
{
    ShaderClass test_shader_class("D:/c++/OpenGl/Chess-OpenGL/Shaders/TestShader.glsl");
    
    BatchRenderer batch_renderer_test{};
   
    //batch_renderer_test.Push({ 3,2 }, { 1.0f,1.0f,0.0f });
   

    while (m_isApplicationRunning) {
        glClear(GL_COLOR_BUFFER_BIT);

        test_shader_class.UseProgram();
        test_shader_class.SetUniform4x4Matrix("orthographicProjection", m_ApplicationOrthographicProjection);

        batch_renderer_test.Push({ -40,0 }, { 1.0f,1.0f,1.0f });
        batch_renderer_test.Push({ -30,0 }, { 1.0f,0.0f,1.0f });
        batch_renderer_test.Push({ -20,0 }, { 2.0f,0.0f,1.0f });
        batch_renderer_test.Push({ -10,0 }, { 0.0f,0.0f,1.0f });
        batch_renderer_test.Push({ 00,0 }, { 1.0f,1.0f,1.0f });
        batch_renderer_test.Push({ 10,0 }, { 1.0f,1.0f,1.0f });
        batch_renderer_test.Push({ 20,0 }, { 1.0f,1.0f,1.0f });
        batch_renderer_test.Push({ 30,0 }, { 1.0f,1.0f,1.0f });


        glClearColor(1.f, 0.f, 0.f, 1.0f);


        batch_renderer_test.Flush();


        m_ApplicationWindow->OnUpdate();
    }

}

void Chess_Game::Application::OnEvent(const Event& e)
{
    if(e.GetEventType() == EventType_kWindowClosed)
        m_isApplicationRunning = false;
    else if (e.GetEventType() == EventType_kWindowResize)
    {
        const WindowResizeEvent kWindowResizeEvent = dynamic_cast<const WindowResizeEvent&>(e);
        Size2D new_window_size = kWindowResizeEvent.GetWindowSize();

        glViewport(0, 0, new_window_size.width, new_window_size.height);
        CalculateOrthoProjection(new_window_size);
     }
}

void Chess_Game::Application::CalculateOrthoProjection(const Size2D& window_size)
{
    float orthoAspect = 1.0f / 10.0f;
    float screen_aspect_ratio = window_size.width / window_size.height;



    constexpr float orthoScale = 10.f;

    const float kHalfWindowWidth = (window_size.width / 2.0f);
    const float kHalfWindowHeight = (window_size.height / 2.0f);

    m_ApplicationOrthographicProjection = glm::ortho(-kHalfWindowWidth* orthoAspect,
        kHalfWindowWidth* orthoAspect, -kHalfWindowHeight* orthoAspect, kHalfWindowHeight* orthoAspect);

}
