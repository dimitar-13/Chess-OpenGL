#include "GPU-Side/ShaderClass.h"
#include "Application.h"
#include <iostream>
#include "GPU-Side/BatchRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Gameplay/ChessGame.h"
#include "Scene/DefaultChessGameScene.h"

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
    //ChessGame test_chess_board;


    //if (test_chess_board.CanMoveSelectedPiece({ 'c',3 }))
    //{
    //    test_chess_board.MoveSelectedPiece({ 'c',3 });
    //}
       // Simulate moves for Scholar's Mate

    //test_chess_board.SelectPiece({ 'e', 2 });
    //if (test_chess_board.CanMoveSelectedPiece({ 'e', 4 })) {
    //    test_chess_board.MoveSelectedPiece({ 'e', 4 });
    //}

    //// Black pawn e7 -> e5
    //test_chess_board.SelectPiece({ 'e', 7 });
    //if (test_chess_board.CanMoveSelectedPiece({ 'e', 5 })) {
    //    test_chess_board.MoveSelectedPiece({ 'e', 5 });
    //}

    //// White knight g1 -> f3
    //test_chess_board.SelectPiece({ 'g', 1 });
    //if (test_chess_board.CanMoveSelectedPiece({ 'f', 3 })) {
    //    test_chess_board.MoveSelectedPiece({ 'f', 3 });
    //}

    //// Black knight b8 -> c6
    //test_chess_board.SelectPiece({ 'b', 8 });
    //if (test_chess_board.CanMoveSelectedPiece({ 'c', 6 })) {
    //    test_chess_board.MoveSelectedPiece({ 'c', 6 });
    //}

    //// White castles kingside (O-O)
    //test_chess_board.SelectPiece({ 'a', 2 });
    //if (test_chess_board.CanMoveSelectedPiece({ 'a', 3 })) {
    //    test_chess_board.MoveSelectedPiece({ 'a', 3 });
    //}

    //// Black pawn d7 -> d6
    //test_chess_board.SelectPiece({ 'd', 7 });
    //if (test_chess_board.CanMoveSelectedPiece({ 'd', 6 })) {
    //    test_chess_board.MoveSelectedPiece({ 'd', 6 });
    //}

    //// White pawn d2 -> d4
    //test_chess_board.SelectPiece({ 'd', 2 });
    //if (test_chess_board.CanMoveSelectedPiece({ 'd', 4 })) {
    //    test_chess_board.MoveSelectedPiece({ 'd', 4 });
    //}

    //// Black captures White's pawn: e5 -> d4
    //test_chess_board.SelectPiece({ 'e', 5 });
    //if (test_chess_board.CanMoveSelectedPiece({ 'd', 4 })) {
    //    test_chess_board.MoveSelectedPiece({ 'd', 4 });
    //}

    //// White recaptures with knight: f3 -> e4
    //test_chess_board.SelectPiece({ 'f', 3 });
    //if (test_chess_board.CanMoveSelectedPiece({ 'd', 4 })) {
    //    test_chess_board.MoveSelectedPiece({ 'd', 4 });
    //}

}

void Chess_Game::Application::RenderLoop()
{
    ShaderClass test_shader_class("D:/c++/OpenGl/Chess-OpenGL/Shaders/TestShader.glsl");
    
    BatchRenderer batch_renderer_test{};
    m_CurrentApplicationScene = std::make_unique<DefaultChessScene>();
    m_CurrentApplicationScene->InitScene();
    //batch_renderer_test.Push({ 3,2 }, { 1.0f,1.0f,0.0f });
   

    while (m_isApplicationRunning) {

        glClear(GL_COLOR_BUFFER_BIT);

        glClearColor(1.f, 0.f, 0.f, 1.0f);

        test_shader_class.UseProgram();
        test_shader_class.SetUniform4x4Matrix("orthographicProjection", m_ApplicationOrthographicProjection);

        m_CurrentApplicationScene->OnUpdate();
        m_CurrentApplicationScene->DrawScene();


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

    const float kHalfWindowWidth = (window_size.width / 2.0f)*2;
    const float kHalfWindowHeight = (window_size.height / 2.0f)*2;

    m_ApplicationOrthographicProjection = glm::ortho(-kHalfWindowWidth* orthoAspect,
        kHalfWindowWidth* orthoAspect, -kHalfWindowHeight* orthoAspect, kHalfWindowHeight* orthoAspect);

}
