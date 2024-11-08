#include "PausseMenuUIHelper.h"
#include "Scene/DefaultChessGameScene.h"
#include "Scene/MainMenuScene.h"

Chess_Game::PauseMenuUIHelper::PauseMenuUIHelper(std::shared_ptr<Application> application)
{
    constexpr glm::vec2 kMenuButtonSize = glm::vec2{ 200,60.0f };
    UIManager& ui_manager = application->GetUIManager();

    auto to_main_menu_button_callback = [application]() {
        auto main_menu_scene = std::make_shared<MainMenuScene>(application);
        application->SwitchCurrentApplicationScene(main_menu_scene);
        };

    auto invert_visibility = [this]()
        {
            m_PauseMenuGroup->EnableElement(!m_PauseMenuGroup->IsElementEnabled());
            m_IsPaused = !m_IsPaused;
        };

    auto button_reset_callback_test = [application]() {
        auto new_scene = std::make_shared<DefaultChessScene>(application);
        application->SwitchCurrentApplicationScene(new_scene);

        };

    m_PauseButton = ui_manager.CreateUIElement<Button>(glm::vec2(-70), glm::vec2(50));
    m_PauseButton->SetButtonOnClickCallback(invert_visibility);
    m_PauseButton->SetButtonCustomTexture("chess_game_pause_button.png");
    m_PauseButton->SetPositionPivot(PositionPivot_kTopRight);

    m_PauseMenuGroup = ui_manager.CreateUIElement<Panel>(glm::vec2(0), glm::vec2(400));
    m_PauseMenuGroup->SetVisibility(true);
    m_PauseMenuGroup->EnableElement(false);
    m_PauseMenuGroup->SetPanelCustomTexture("chess_game_panel_background.png");
    m_PauseMenuGroup->SetElementDepth(.7);

    m_ResetButton = ui_manager.CreateUIElement<Button>(glm::vec2(0), kMenuButtonSize);
    m_ResetButton->SetButtonOnClickCallback(button_reset_callback_test);
    m_ResetButton->SetButtonCustomTexture("chess_game_button_background.png");
    m_ResetButton->SetElementDepth(.1);
    m_PauseMenuGroup->AddChildElement(m_ResetButton);

    m_ResetText = ui_manager.CreateUIElement<TextUI>(glm::vec2(-65.0f, -5.0f), glm::vec3(1));
    m_ResetText->SetText("Reset");
    m_ResetText->SetElementDepth(.9);
    m_PauseMenuGroup->AddChildElement(m_ResetText);

    m_MainMenuButton = ui_manager.CreateUIElement<Button>(glm::vec2(0, -200.0f), kMenuButtonSize);
    m_MainMenuButton->SetButtonOnClickCallback(to_main_menu_button_callback);
    m_MainMenuButton->SetElementDepth(.1);
    m_MainMenuButton->SetButtonCustomTexture("chess_game_button_background.png");
    m_PauseMenuGroup->AddChildElement(m_MainMenuButton);

    m_PauseText = ui_manager.CreateUIElement<TextUI>(glm::vec2(-85.0f, 195.0f), glm::vec3(1));
    m_PauseText->SetText("Resume");
    m_PauseText->SetElementDepth(.9);
    m_PauseMenuGroup->AddChildElement(m_PauseText);

    m_ResumeButton = ui_manager.CreateUIElement<Button>(glm::vec2(0, 200.0f), kMenuButtonSize);
    m_ResumeButton->SetButtonOnClickCallback(invert_visibility);
    m_ResumeButton->SetButtonCustomTexture("chess_game_button_background.png");
    m_ResumeButton->SetElementDepth(.1);
    m_PauseMenuGroup->AddChildElement(m_ResumeButton);

    m_MainMenuText = ui_manager.CreateUIElement<TextUI>(glm::vec2(-115.0f, -205.0f), glm::vec3(1));
    m_MainMenuText->SetText("Main menu");
    m_MainMenuText->SetElementDepth(.9);
    m_PauseMenuGroup->AddChildElement(m_MainMenuText);

}

void Chess_Game::PauseMenuUIHelper::OnGameOverScreen(std::shared_ptr<Application> application)
{
    m_PauseMenuGroup->AddChildElement(m_GameOverText);

    m_PauseMenuGroup->RemoveChildElement(m_PauseText);
    m_PauseText->EnableElement(false);
    m_PauseMenuGroup->RemoveChildElement(m_ResumeButton);
    m_ResumeButton->EnableElement(false);

    m_GameOverText =
        application->GetUIManager().CreateUIElement<TextUI>(glm::vec2(-230.0f, 180.0f), glm::vec2(2.0));
    m_GameOverText->SetText("Game over");
    m_GameOverText->SetElementDepth(.9);
    m_PauseMenuGroup->AddChildElement(m_GameOverText);

    m_PauseMenuGroup->EnableElement(true);
}
