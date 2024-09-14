#include "D:/c++/OpenGl/Chess-OpenGL/build/CMakeFiles/Chess.dir/Debug/cmake_pch.hxx"
#include "MainMenuScene.h"
#include "Core/Application.h"
#include "Scene/DefaultChessGameScene.h"
#include "Logging/Logger.h"

void Chess_Game::MainMenuScene::InitScene()
{
    DrawableData game_logo_data{};
    game_logo_data.color = glm::vec3(1);
    game_logo_data.position = glm::vec3(0, 150, .5f);
    game_logo_data.scale = glm::vec2(200);
    game_logo_data.texture_name = TextureName_kGameLogo;

    
    if (auto application = m_Application.lock())
    {
        Size2D win_size = application->GetApplicationWindow().GetWindowSize();

        Margin button_margin{};
        button_margin.top += 200.0f;

        Margin logo_margin{};
       // button_margin.bottom = 200.0f;

        m_StartButton = application->GetUIManager().CreateUIElement<Button>(
            button_margin,AnchorPoint_kMiddle,glm::vec2(200,70));
        m_StartButton->SetButtonCustomTexture(TextureName_kButton);

        m_GameLogoImage = application->GetUIManager().CreateUIElement<Image>(
            logo_margin, AnchorPoint_kMiddle, glm::vec2(200, 200));
        m_GameLogoImage->SetImageTexture(TextureName_kGameLogo);


        auto test_callback = [this]() 
            {
                 if (auto application = m_Application.lock())
                 {
                     std::shared_ptr<SceneObject> new_scene = std::make_shared<DefaultChessScene>(m_Application);
                     application->SwitchCurrentApplicationScene(new_scene);
                 }
            };

        m_StartButton->SetButtonCallback(test_callback);
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
