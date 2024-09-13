#include "D:/c++/OpenGl/Chess-OpenGL/build/CMakeFiles/Chess.dir/Debug/cmake_pch.hxx"
#include "MainMenuScene.h"
#include "Core/Application.h"
void Chess_Game::MainMenuScene::InitScene()
{
    if (auto application = m_Application.lock())
    {
        m_StartButton = application->GetUIManager().CreateUIElement<Button>(TextureName_kButton,
            glm::vec2(0),glm::vec2(200));
    }

}

void Chess_Game::MainMenuScene::DrawScene(std::shared_ptr<BatchRenderer> application_batch_renderer)
{
   

}

void Chess_Game::MainMenuScene::OnUpdate()
{
}

void Chess_Game::MainMenuScene::DestroyScene()
{
}
