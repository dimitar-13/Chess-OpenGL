#pragma once
#include "Core/Chess_pch.h"
#include "UI/Button.h"
#include "Core/Application.h"
#include "UI/TextUI.h"
namespace Chess_Game
{
    class PauseMenuUIHelper
    {
    public:
        PauseMenuUIHelper(std::shared_ptr<Application> application);
        bool IsPauseMenuOn()const { return m_IsPaused; }
        void OnGameOverScreen(std::shared_ptr<Application> application);
    private:
        std::shared_ptr<Button> m_PauseButton;
        std::shared_ptr<TextUI> m_PauseText;
        std::shared_ptr<Button> m_ResetButton;
        std::shared_ptr<TextUI> m_ResetText;
        std::shared_ptr<Button> m_ResumeButton;
        std::shared_ptr<Button> m_MainMenuButton;
        std::shared_ptr<TextUI> m_ToMainMenu;
        std::shared_ptr<Panel> m_PauseMenuGroup;
        std::shared_ptr<TextUI> m_GameOverText;
        bool m_IsPaused = false;
    };
}