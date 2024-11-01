#include "MainMenuScene.h"
#include "Core/Application.h"
#include "Scene/DefaultChessGameScene.h"
#include "Logging/Logger.h"
#include "Core/Application.h"

Chess_Game::MainMenuScene::MainMenuScene(std::weak_ptr<Application> main_application):
    m_UICreditHelper(main_application.lock()->GetApplicationUIManager()),
    SceneObject(main_application)
{}


void Chess_Game::MainMenuScene::InitScene()
{
  
    if (auto application = m_Application.lock())
    {

       m_StartMenuPanel = application->GetUIManager().CreateUIElement<Panel>(
               glm::vec2(0), glm::vec2(400));
       
       m_StartMenuPanel->SetElementDepth(.1);
       m_StartMenuPanel->SetVisibility(true);
       m_StartMenuPanel->SetPanelCustomTexture("chess_game_panel_background.png");
       
       m_StartButton = application->GetUIManager().
           CreateUIElement<Button>(glm::vec2(0,-140.0f),glm::vec2(200,55.));   
       m_StartButton->SetButtonCustomTexture("chess_game_button_background.png");
       m_StartButton->SetElementDepth(0.1);
       m_StartMenuPanel->AddChildElement(m_StartButton);
       
       m_StartText = application->GetUIManager().
           CreateUIElement<TextUI>(glm::vec2(-50.0f, -146.0f), glm::vec3(1));
       m_StartText->SetText("Start");
       m_StartText->SetElementDepth(0.1);
       m_StartMenuPanel->AddChildElement(m_StartText);

       m_CreditsButton = application->GetUIManager().
           CreateUIElement<Button>(glm::vec2(0, -240.0f), glm::vec2(180, 40.));
       m_CreditsButton->SetButtonCustomTexture("chess_game_button_background.png");
       m_CreditsButton->SetElementDepth(0.1);
       m_CreditsButton->SetButtonOnClickCallback([this] { m_UICreditHelper.EnableUICreditUI(true); });
       m_StartMenuPanel->AddChildElement(m_CreditsButton);

       m_CreditsText = application->GetUIManager().
           CreateUIElement<TextUI>(glm::vec2(-65.0f, -246.0f), glm::vec3(.8));
       m_CreditsText->SetText("Credits");
       m_CreditsText->SetElementDepth(0.1);
       m_StartMenuPanel->AddChildElement(m_CreditsText);

       m_GameLogoImage = application->GetUIManager().CreateUIElement<Image>
           (glm::vec2(0, 60.0f), glm::vec2(200));       
       m_GameLogoImage->SetImageTexture("chess_game_logo.png");
       m_GameLogoImage->SetElementDepth(0.);
       m_StartMenuPanel->AddChildElement(m_GameLogoImage);
       
       m_GameNameUIText = application->GetUIManager().CreateUIElement<TextUI>
           (glm::vec2(-200.0f, -60.0f), glm::vec2(1.5f));
       m_GameNameUIText->SetElementDepth(0.1);
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
       
       m_StartButton->SetButtonOnClickCallback(test_callback);

       
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
