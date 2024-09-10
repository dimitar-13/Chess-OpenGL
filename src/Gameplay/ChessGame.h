#pragma once
#include "BaseChessPiece.h"
#include "ChessPlayer.h"
#include <vector>

namespace Chess_Game
{
    class ChessGame {
    public:
        ChessGame(const std::shared_ptr<ChessPlayer>& white_player,
            const std::shared_ptr<ChessPlayer>& black_player);
        void SelectPiece(BoardPosition piece_board_position);
        bool IsPieceSelected();
        std::weak_ptr<ChessPiece> GetSelectedPiece();
        bool CanMoveSelectedPiece(BoardPosition new_position);
        void MoveSelectedPiece(BoardPosition new_position);
        bool IsGameOver()const { return m_IsGameOver; }
    private:
        bool IsKingSafeAfterMove(BoardPosition new_position,std::shared_ptr<ChessPlayer> current_player);
        bool CanResolveCheck(BoardPosition new_position,std::shared_ptr<ChessPlayer> current_player);
        bool IsKingChecked();
        bool IsKingCheckMated();
        bool CanKingResolveTheCheck(std::shared_ptr<ChessPlayer> current_player);
        bool CanOtherPiecesResolveTheCheck(std::shared_ptr<ChessPlayer> player_to_check);
        bool CanMove(BoardPosition new_position, std::shared_ptr<ChessPlayer> player_to_check);
        void CapturePiece(BoardPosition position_of_piece_to_capture);
        std::shared_ptr<ChessPlayer> GetNonActivePlayer();
        void SetupChessBoardBitMask();     
    private:
        ChessBoard m_ChessBoardData{};
        std::weak_ptr<ChessPlayer> m_CurrentPlayer{};
        std::shared_ptr<ChessPlayer> m_WhiteTeamPlayer{};
        std::shared_ptr<ChessPlayer> m_BlackTeamPlayer{};
        bool m_IsGameOver = false;
    };
}