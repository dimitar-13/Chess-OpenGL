#pragma once
#include "Gameplay/BaseChessPiece.h"
#include "Gameplay/ChessPlayer.h"
#include "Gameplay/ChessGame.h"
#include "Core/Chess_pch.h"
#include "CurrentPlayerController.h"
namespace Chess_Game
{
    /**
     * @brief Class for simplifying the work with 'ChessGame' class. 
     * 
     * The class uses the 'ChessGame' class and it serves as a middle man between the class and the user of the 'ChessGame' class.
     * It provides simple functions for playing and extracting information about the game while managing things like current player
     * and etc.
     * 
     */
    class ChessGameController {
    public:
        /**
         * @brief Constructor creates a valid 'ChessGame' from provided 2 player.
         * @param white_team_player White team player.
         * @param black_team_player Black team player.
         * @param selected_piece_changed_callback Callback when selected piece was change.
         * @param selected_piece_moved_callback Callback when selected piece has change its position.
         * @param pawn_promotion_callback Pawn promotion callback.
         */
        ChessGameController(std::shared_ptr<ChessPlayer> white_team_player,
            std::shared_ptr<ChessPlayer> black_team_player,
            const std::function<void(std::weak_ptr<ChessPiece>)>& selected_piece_changed_callback,
            const std::function<void(std::shared_ptr<ChessPiece>)>& selected_piece_moved_callback,
            const std::function<std::shared_ptr<ChessPiece>(bool, BoardPosition)>& pawn_promotion_callback);
        /**
         * @brief Process input accordingly based on the state of the game.
         *
         * This function gets a 'BoardPosition' as input and it first checks if a piece is selected. If its not it attempts to 
         * selected a piece,if any at the given position. 
         * If a piece was already selected but the provided 'BoardPosition' is the same as the selected piece it deselects it.
         * 
         * If none of the above statement is true then the provided 'BoardPosition' is passed to the 'ChessGame' to check for 
         * validity if valid it handles it by moving the piece and switching game state.
         * 
         * @param new_position The inputted 'BoardPosition'.
         */
        void ProcessInput(BoardPosition new_position);
        /**
         * @brief Checks if the game is over.
         * @returns Returns true if the game is over using the 'ChessGame' API.
         */
        bool IsGameOver()const { return m_ChessGame->IsGameOver(); }
        /**
         * @brief Returns all of the board chess pieces.
         * @returns Returns all of the board chess pieces.
         */
        std::vector<std::shared_ptr<ChessPiece>> GetAllBoardPieces() { return m_ChessGame->GetAllChessboardPieces(); }
        /**
         * @brief Returns all of the current selected piece possible positions.
         * @return Returns all of the current selected piece possible positions.
         */
        std::vector<BoardPosition> GetSelectedPieceAllPossibleMoves() { return m_ChessGame->GetSelectedPieceAllPossibleMoves(m_PlayerCurrentController.GetActivePlayer(), m_PlayerCurrentController.GetNotActivePlayer()); }
    private:
        /**
         * @brief Attempts to selected a piece that belongs to the active player.
         * @param piece_board_position The piece to select 'BoardPosition'. 
         */
        void SelectPiece(BoardPosition piece_board_position);
        /**
         * @brief Unselects the selected piece from the active player(if any). 
         */
        void UnselectPiece();
        /**
         * @brief Checks if the current player has selected piece.
         * @returns Returns true if the current player has selected piece.
         */
        bool IsPieceSelected();
        /**
         * @brief Uses the 'ChessGame' API to check if the move is valid.
         * @param new_position New position to check the validity of.
         * @returns Returns true if the move is valid.
         */
        bool CanMoveSelectedPiece(BoardPosition new_position);
        /**
         * @brief Moves the selected piece to the provided position and invokes the on piece move callback.
         * 
         * @param new_position New position to move the selected piece.
         */
        void MoveSelectedPiece(BoardPosition new_position);
        /**
         * @brief Gets current player selected piece.
         *
         * @returns Returns the current player selected piece.
         */
        std::shared_ptr<ChessPiece> GetSelectedPiece() { return m_PlayerCurrentController.GetActivePlayer()->GetSelectedPiece().lock();}
    private:
        std::unique_ptr<ChessGame> m_ChessGame{};               ///< The chess game.
        CurrentPlayerController m_PlayerCurrentController;      ///< Controller to keep track of current and non current player.
        std::function<void(std::weak_ptr<ChessPiece>)> m_OnSelectedPieceChanged{}; ///< On slected piece change callback.
        std::function<void(std::shared_ptr<ChessPiece>)> m_OnSelectedPieceMoved{}; ///< On selected piece move callback.
    };
}