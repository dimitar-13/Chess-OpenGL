#pragma once
#include "Gameplay/BaseChessPiece.h"
#include "Gameplay/ChessPlayer.h"
#include "Gameplay/ChessGame.h"
#include "Core/Chess_pch.h"
#include "CurrentPlayerController.h"
namespace Chess_Game
{

    class ChessGameController {
    public:
        ChessGameController(std::shared_ptr<ChessPlayer> white_team_player,
            std::shared_ptr<ChessPlayer> black_team_player,
            const std::function<void(std::weak_ptr<ChessPiece>)>& selected_piece_changed_callback,
            const std::function<void(std::shared_ptr<ChessPiece>)>& selected_piece_moved_callback,
            const std::function<std::shared_ptr<ChessPiece>(bool,BoardPosition)>& pawn_promotion_callback):

            m_PlayerCurrentController(white_team_player, black_team_player),
            m_OnSelectedPieceChanged(selected_piece_changed_callback),
            m_OnSelectedPieceMoved(selected_piece_moved_callback)
        {
            m_ChessGame = std::make_unique<ChessGame>(white_team_player,
                black_team_player, pawn_promotion_callback);
        }
        void ProcessInput(BoardPosition new_position);
        bool IsGameOver()const { return m_ChessGame->IsGameOver(); }
        std::vector<std::shared_ptr<ChessPiece>> GetAllBoardPieces() { return m_ChessGame->GetAllChessboardPieces(); }
        std::vector<BoardPosition> GetSelectedPieceAllPossibleMoves() { return m_ChessGame->GetSelectedPieceAllPossibleMoves(m_PlayerCurrentController.GetActivePlayer(), m_PlayerCurrentController.GetNotActivePlayer()); }
    private:
        void SelectPiece(BoardPosition piece_board_position);
        void UnselectPiece();
        bool IsPieceSelected();
        bool CanMoveSelectedPiece(BoardPosition new_position);
        void MoveSelectedPiece(BoardPosition new_position);
        std::weak_ptr<ChessPiece> GetSelectedPiece() { return m_PlayerCurrentController.GetActivePlayer()->GetSelectedPiece();}
    private:
        std::unique_ptr<ChessGame> m_ChessGame{};
        CurrentPlayerController m_PlayerCurrentController;
        std::function<void(std::weak_ptr<ChessPiece>)> m_OnSelectedPieceChanged{};
        std::function<void(std::shared_ptr<ChessPiece>)> m_OnSelectedPieceMoved{};
    };
}