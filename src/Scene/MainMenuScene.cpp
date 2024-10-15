#include "D:/c++/OpenGl/Chess-OpenGL/build/CMakeFiles/Chess.dir/Debug/cmake_pch.hxx"
#include "MainMenuScene.h"
#include "Core/Application.h"
#include "Scene/DefaultChessGameScene.h"
#include "Logging/Logger.h"

void Chess_Game::MainMenuScene::InitScene()
{
  
    if (auto application = m_Application.lock())
    {

       m_StartMenuPanel = application->GetUIManager().CreateUIElement<Panel>(
               glm::vec2(0), glm::vec2(400));
       
       m_StartMenuPanel->SetElementDepth(-.5);
       m_StartMenuPanel->SetVisibility(true);
       m_StartMenuPanel->SetPanelCustomTexture(TextureName_kUIGroupBackground);
       
       m_StartButton = application->GetUIManager().
           CreateUIElement<Button>(glm::vec2(0,-200.0f),glm::vec2(200,30.));   
       m_StartButton->SetButtonCustomTexture(TextureName_kButton);    
       m_StartButton->SetElementDepth(0.);
       m_StartMenuPanel->AddChildElement(m_StartButton);
       
       m_GameLogoImage = application->GetUIManager().CreateUIElement<Image>
           (glm::vec2(0, 40.0f), glm::vec2(300));       
       m_GameLogoImage->SetImageTexture(TextureName_kGameLogo);
       m_GameLogoImage->SetElementDepth(0.);
       m_StartMenuPanel->AddChildElement(m_GameLogoImage);
       
       m_GameNameUIText = application->GetUIManager().CreateUIElement<TextUI>
           (glm::vec2(-200.0f, -80.0f), glm::vec2(1.5f));
       m_GameNameUIText->SetElementDepth(.1);
       m_GameNameUIText->SetText("Chess Game");
       m_StartMenuPanel->AddChildElement(m_GameNameUIText);


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
    static std::shared_ptr<TextFont> test_font =
        std::make_shared<TextFont>();
    auto asset_loader = m_Application.lock()->GetAssetLoader();
    
    application_batch_renderer->DrawTextBatch(m_Application.lock()->GetApplicationProjection().GetMatrix());

}

void Chess_Game::MainMenuScene::OnUpdate()
{

}

void Chess_Game::MainMenuScene::DestroyScene()
{
}
