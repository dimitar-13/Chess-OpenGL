#pragma once
#include "Pch/Chess_pch.h"
#include "UI/Button.h"
#include "Core/Application.h"
#include "UI/TextUI.h"
namespace Chess_Game
{
    /**
     * @brief Class for creating and managing pause menu logic.
     *
     * The class creates all of the needed UI for the pause menu and provide functions for 
     * showing if the pause menu is on and for triggering Game over screen.
     * 
     */
    class PauseMenuUIHelper
    {
    public:
        /**
         * @brief Constructor creates the pause menu UI elements and sets their data.
         * 
         * The constructor creates the pause menu UI elements and sets their callbacks and add them to pause
         * menu Panel.
         */
        PauseMenuUIHelper(std::shared_ptr<Application> application);
        /**
         * @brief Checks if to see if game is paused.
         * @returns Returns true if the game is paused.
          */
        bool IsPauseMenuOn()const { return m_IsPaused; }
        /**
         * @brief Changes the pause menu to game over menu.
         * @param application Shared ref to the application class.
         */
        void OnGameOverScreen(std::shared_ptr<Application> application);
    private:
        std::shared_ptr<Button> m_PauseButton;    //< Pasue button UI element.
        std::shared_ptr<TextUI> m_PauseText;      //< Pause button text UI element.
        std::shared_ptr<Button> m_ResetButton;    //< Reset button UI element.
        std::shared_ptr<TextUI> m_ResetText;      //< Reset button text UI element.
        std::shared_ptr<Button> m_ResumeButton;   //< Resume button UI element.
        std::shared_ptr<Button> m_MainMenuButton; //< Main menu button UI element.
        std::shared_ptr<TextUI> m_MainMenuText;   //< Main menu text UI element.
        std::shared_ptr<Panel> m_PauseMenuGroup;  //< Pause menu panel.
        std::shared_ptr<TextUI> m_GameOverText;   //< Game over text UI element.
        bool m_IsPaused = false;                  //< Flag indicating if pause menu is visible.
    };
}