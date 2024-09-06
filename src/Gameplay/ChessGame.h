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
        void IsGameOver() {}
    private:
        bool IsKingSafeAfterMove(BoardPosition new_position);
        bool IsKingChecked();
        bool IsKingCheckMated();
        void InitializeTeamPieces();
        void CapturePiece(BoardPosition position_of_piece_to_capture);
        std::shared_ptr<ChessPlayer> GetNonActivePlayer();
        void SetupChessBoardBitMask();
    private:
        ChessBoard m_ChessBoardData{};

        std::weak_ptr<ChessPlayer> m_CurrentPlayer{};
        std::shared_ptr<ChessPlayer> m_WhiteTeamPlayer{};
        std::shared_ptr<ChessPlayer> m_BlackTeamPlayer{};

    };
}