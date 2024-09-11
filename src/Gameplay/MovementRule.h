#pragma once
#include "Core/Chess_pch.h"
#include "BoardPosition.h"
#include "ChessBoard.h"

namespace Chess_Game
{
    class ChessPiece;

    class BoardSpecificMovementRule
    {
    public:
        virtual bool CanMove(BoardPosition current_position,
            BoardPosition new_position, ChessBoard& board)const = 0;
    };

    class PawnCaptureBoardSpecificMovementRule : public BoardSpecificMovementRule
    {
    public:
         bool CanMove(BoardPosition current_position,
             BoardPosition new_position, ChessBoard& board)const override;
    };
    class PawnTeamMovementRule : public BoardSpecificMovementRule
    {
    public:
        bool CanMove(BoardPosition current_position,
            BoardPosition new_position, ChessBoard& board)const override;
    };
    class BlockableBoardSpecificMovementRule : public BoardSpecificMovementRule
    {
    public:
         bool CanMove(BoardPosition current_position,
             BoardPosition new_position, ChessBoard& board)const override;
    };
    class CanMoveToTarget : public BoardSpecificMovementRule
    {
    public:
        bool CanMove(BoardPosition current_position,
            BoardPosition new_position, ChessBoard& board) const override;
    };

    class MovementRule
    {
    public:
        virtual bool CanMove(BoardPosition current_position,BoardPosition new_position)const = 0;
    };

    class RookMovementRule : public MovementRule
    {
    public:
        bool CanMove(BoardPosition current_position, BoardPosition new_position)const override;
    };

    class BishopMovementRule : public MovementRule
    {
    public:
        bool CanMove(BoardPosition current_position, BoardPosition new_position)const override;
    };

    class KnightMovementRule : public MovementRule
    {
    public:
        bool CanMove(BoardPosition current_position, BoardPosition new_position)const override;
    };
    class KingMovement : public MovementRule
    {
    public:
        bool CanMove(BoardPosition current_position, BoardPosition new_position)const override;
    };
    class SingleForwardMovementRule : public MovementRule
    {
    public:
        bool CanMove(BoardPosition current_position, BoardPosition new_position)const override;
    };

    class PawnStartingMovementRule : public MovementRule
    {
    public:
        bool CanMove(BoardPosition current_position, BoardPosition new_position)const override;
    };
    class PawnSidewayCaptureRule : public MovementRule
    {
    public:
        bool CanMove(BoardPosition current_position, BoardPosition new_position)const override;
    };


}