#pragma once
#include "BoardPosition.h"
#include "Core/Chess_pch.h"

namespace Chess_Game
{


    enum BoardPositionFlags_ : uint8_t
    {
        BoardPositionFlags_kIsPositionOcupied = 1 << 0,
        BoardPositionFlags_kIsPieceFromBlackTeam = 1 << 1,
        BoardPositionFlags_kIsPieceFromWhiteTeam = 1 << 2,
        BoardPositionFlags_kIsPieceImortal = 1 << 3,
    };

    class ChessBoard
    {
    public:
        BoardPositionFlags_ GetChessboardPositionFlag(BoardPosition position);
        void SetChessboardPositionFlag(BoardPosition position, BoardPositionFlags_ bit_mask);

        static bool IsPositionAtVerticalBoarder(BoardPosition new_position);
        static bool IsNewPositionInBounds(BoardPosition new_position);
    private:
        size_t GetIndexByChessboardPosition(BoardPosition board_position);
    private:
        std::array<BoardPositionFlags_, 64> chessboard_bit_mask{};
    };

}