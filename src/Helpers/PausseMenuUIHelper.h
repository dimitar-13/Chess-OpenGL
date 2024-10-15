#pragma once
#include "Core/Chess_pch.h"
#include "UI/Button.h"
#include "Core/Application.h"
namespace Chess_Game
{
    class PauseMenuUIHelper
    {
    public:
        PauseMenuUIHelper(std::shared_ptr<Application> application);
        bool IsPauseMenuOn()const { return m_IsPaused; }
    private:
        std::shared_ptr<Button> m_PauseButton;
        std::shared_ptr<Button> m_ResetButton;
        std::shared_ptr<Button> m_ResumeButton;
        std::shared_ptr<Button> m_MainMenuButton;
        std::shared_ptr<Panel> m_PauseMenuGroup;
        bool m_IsPaused = false;
    };
}