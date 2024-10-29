#pragma once
#include "Pch/Chess_pch.h"
#include "UI/UIManager.h"
#include "UI/Button.h"
#include "UI/Panel.h"
#include "Gameplay/BaseChessPiece.h"
namespace Chess_Game
{
    /**
     * @brief Helper class for creating and managing a pawn promotion screen.
     *
     * This class creates the UI for the pawn promotion selection UI and 
     * handles the user interface for promoting a pawn in a chess game.
     * 
     * It allows users to select which piece they want to promote their pawn to
     * (Queen, Bishop, Rook, or Knight) and manages the visibility and position
     * of the promotion UI panel.
     * 
     * 
     * It sets the button callbacks and provides a way to get notified on user selection 
     * via a callback.
     */
    class PawnPromotionSelectionUI
    {
    public:
        /**
         * @brief Creates the pawn promotion UI.
         *
         * Constructor creates the pawn promotion UI setting the corresponding button callbacks and
         * button textures. It also sets the visibility to be not-visible.
         * 
         * @param ui_manager A shared pointer to the UIManager that will handle
         * UI operations and events.
         */
        PawnPromotionSelectionUI(std::shared_ptr<UIManager> ui_manager);
        /**
         * @brief Toggles the visibility of the pawn promotion UI.
         *
         * @param is_enabled A boolean value indicating whether the promotion UI
         * should be enabled (visible) or disabled (hidden).
         */
        void TogglePawnPromotionUI(bool is_enabled);
        /**
         * @brief Switches the team color of the promotion pieces.
         *
         * @param is_white_team A boolean value indicating whether the promotion
         *                      pieces should represent the white team (true) or the black team(false).                 
         */
        void SwitchPromotionPieceTeamCol(bool is_white_team);
        /**
         * @brief Sets a new position for the pawn promotion panel.
         *
         * @param new_pos A glm::vec2 representing the new position for the
         *                pawn promotion panel, relative to its parent.
         */
        void SetPosition(const glm::vec2& new_pos) { m_PawnPromotionPanel->SetRelativePosition(new_pos); }
        /**
         * @brief Sets the callback function for user selection.
         *
         * This function assigns a callback to be invoked when the user selects
         * a promotion piece.
         *
         * @param test_callback A function that will be called with the selected
         *                      ChessPieceType when the user makes a selection.
         */
        void SetCallback(const std::function<void(ChessPieceType_)>& test_callback) { m_OnUserSelection = test_callback; }
    private:
        std::shared_ptr<Button> m_QueenSelectionButton;     ///< Button for selecting a Queen.
        std::shared_ptr<Button> m_BishopSelectionButton;    ///< Button for selecting a Bishop.
        std::shared_ptr<Button> m_RookSelectionButton;      ///< Button for selecting a Rook.
        std::shared_ptr<Button> m_KnightSelectionButton;    ///< Button for selecting a Knight.
        std::shared_ptr<Panel> m_PawnPromotionPanel;        ///< Panel for the promotion UI.
        std::shared_ptr<UIManager> m_UIManager;             ///< UI manager ref for creating UI widgets. 
        std::function<void(ChessPieceType_)> m_OnUserSelection; ///< Callback for user selection.
    };

}