#include "D:/c++/OpenGl/Chess-OpenGL/build/CMakeFiles/Chess.dir/Debug/cmake_pch.hxx"
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
            m_PauseMenuGroup->SetVisibility(!m_PauseMenuGroup->GetElementVisibility());
            m_IsPaused = !m_IsPaused;
        };

    auto button_reset_callback_test = [application]() {
        auto new_scene = std::make_shared<DefaultChessScene>(application);
        application->SwitchCurrentApplicationScene(new_scene);

        };

    m_PauseButton = ui_manager.CreateUIElement<Button>(glm::vec2(-70), glm::vec2(50));
    m_PauseButton->SetButtonCallback(invert_visibility);
    m_PauseButton->SetButtonCustomTexture(TextureName_kPauseButton);
    m_PauseButton->SetPositionPivot(PositionPivot_kTopRight);

    m_PauseMenuGroup = ui_manager.CreateUIElement<Panel>(glm::vec2(0), glm::vec2(400));
    m_PauseMenuGroup->SetVisibility(false);
    m_PauseMenuGroup->EnablePanelBackground(true);
    m_PauseMenuGroup->SetPanelCustomTexture(TextureName_kUIGroupBackground);
    m_PauseMenuGroup->SetElementDepth(.5);

    m_ResetButton = ui_manager.CreateUIElement<Button>(glm::vec2(0), kMenuButtonSize);
    m_ResetButton->SetButtonCallback(button_reset_callback_test);
    m_ResetButton->SetButtonCustomTexture(TextureName_kButton);
    m_ResetButton->SetElementDepth(.5);
    m_PauseMenuGroup->AddChildElement(m_ResetButton);

    m_MainMenuButton = ui_manager.CreateUIElement<Button>(glm::vec2(0, 200.0f), kMenuButtonSize);
    m_MainMenuButton->SetButtonCallback(to_main_menu_button_callback);
    m_MainMenuButton->SetElementDepth(.5);
    m_MainMenuButton->SetButtonCustomTexture(TextureName_kButton);
    m_PauseMenuGroup->AddChildElement(m_MainMenuButton);

    m_ResumeButton = ui_manager.CreateUIElement<Button>(glm::vec2(0, -200.0f), kMenuButtonSize);
    m_ResumeButton->SetButtonCallback(invert_visibility);
    m_ResumeButton->SetButtonCustomTexture(TextureName_kButton);
    m_PauseMenuGroup->AddChildElement(m_ResumeButton);
}
