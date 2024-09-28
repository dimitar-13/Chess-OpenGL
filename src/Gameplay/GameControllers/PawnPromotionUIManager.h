#pragma once
#include "Core/Chess_pch.h"
#include "UI/UIManager.h"
#include "UI/Button.h"
#include "UI/Panel.h"
#include "Gameplay/BaseChessPiece.h"
namespace Chess_Game
{
    class PawnPromotionSelectionUI
    {
    public:
        PawnPromotionSelectionUI(std::shared_ptr<UIManager> ui_manager);
        void SetPromotionPosition();
        void TogglePawnPromotionUI(bool is_enabled);
        void SwitchPromotionPieceTeamCol(bool is_white_team);
        void SetPosition(const glm::vec2& new_pos) { m_PawnPromotionPanel->SetRelativePosition(new_pos); }
        void SetCallback(const std::function<void(ChessPieceType_)>& test_callback) { m_OnUserSelection = test_callback; }
    private:
        std::shared_ptr<Button> m_QueenSelectionButton;
        std::shared_ptr<Button> m_BishopSelectionButton;
        std::shared_ptr<Button> m_RookSelectionButton;
        std::shared_ptr<Button> m_KnightSelectionButton;
        std::shared_ptr<Panel>  m_PawnPromotionPanel;
        std::shared_ptr<UIManager> m_UIManager;
        std::function<void(ChessPieceType_)> m_OnUserSelection;
    };
}