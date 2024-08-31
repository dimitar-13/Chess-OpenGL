#pragma once
#include "BoardPosition.h"
#include "MovementRule.h"
#include "Core/Attachable.h"
namespace Chess_Game
{
    enum ChessPieceType_
    {
        ChessPieceType_kUnknown,
        ChessPieceType_kPawn,
        ChessPieceType_kBishop,
        ChessPieceType_kRook,
        ChessPieceType_kKnight,
        ChessPieceType_kQueen,
        ChessPieceType_kKing
    };

    class ChessPiece : public Attachable
    {
    public:
        ChessPiece(BoardPosition start_position):
            m_PiecePosition(start_position)
        {
            m_pieceBoardSpecificMovementRule.push_back(std::make_unique<CanMoveToTarget>());
        }
        void SetPiecePosition(BoardPosition new_position) { m_PiecePosition = new_position; OnPositionChanged(); }
        BoardPosition GetPiecePosition()const { return m_PiecePosition; }
        virtual ChessPieceType_ GetChessPieceType()const = 0;
        bool CanMove(BoardPosition new_piece_position);
        virtual void OnPositionChanged() {};
        bool CanMoveBoardSpecific(BoardPosition new_piece_position, ChessBoard& board);
    protected:
        BoardPosition m_PiecePosition;
        std::vector<std::unique_ptr<MovementRule>> m_pieceMovementRules{};
        std::vector<std::unique_ptr<BoardSpecificMovementRule>> m_pieceBoardSpecificMovementRule{};
    };

    class Queen : public ChessPiece
    {
    public:
        Queen(BoardPosition start_position);
        ChessPieceType_ GetChessPieceType()const override {return ChessPieceType_kQueen; }
    };

    class Pawn : public ChessPiece
    {
    public:
        Pawn(BoardPosition start_position);
        ChessPieceType_ GetChessPieceType()const override { return ChessPieceType_kPawn; }
    private:
        void OnPositionChanged() override;
        bool is_first_move = true;
    };

    class Knight : public ChessPiece
    {
    public:
        Knight(BoardPosition start_position);
        ChessPieceType_ GetChessPieceType()const override { return ChessPieceType_kKnight; }
    };

    class Bishop : public ChessPiece
    {
    public:
        Bishop(BoardPosition start_position);
        ChessPieceType_ GetChessPieceType()const override { return ChessPieceType_kBishop; }
    };

    class Rook : public ChessPiece
    {
    public:
        Rook(BoardPosition start_position);
        ChessPieceType_ GetChessPieceType()const override { return ChessPieceType_kRook; }
    };
    class King : public ChessPiece
    {
    public:
        King(BoardPosition start_position);
        ChessPieceType_ GetChessPieceType()const override{ return ChessPieceType_kKing; }
    };


}

