#pragma once
#include "Pch/Chess_pch.h"
#include "UI/TextUI.h"
#include "UI/Button.h"
#include "UI/UIManager.h"
namespace Chess_Game
{
    class UICreditsHelper
    {
    public:
        UICreditsHelper(std::shared_ptr<UIManager> ui_manager);
        void EnableUICreditUI(bool is_enabled);
    private:
        std::shared_ptr<TextUI> m_GameUICredit;
        std::shared_ptr<TextUI> m_GameChessPiecesCredit;
        std::shared_ptr<Button> m_CloseButton;
        std::shared_ptr<Panel> m_CreditUIPanel;
    };
}