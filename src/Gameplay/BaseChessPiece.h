#pragma once
#include "Pch/Chess_pch.h"
#include "BoardPosition.h"
#include "MovementRule.h"
#include "Core/Drawable.h"
namespace Chess_Game
{
    /**
     * @brief Enum representing different chess pieces.
     * 
     * ChessPieceType_kUnknown - For debugging.
     * ChessPieceType_kPawn - Enum representing the pawn.
     * ChessPieceType_kBishop - Enum representing the bishop.
     * ChessPieceType_kRook - Enum representing the rook.
     * ChessPieceType_kKnight - Enum representing the knight.
     * ChessPieceType_kQueen - Enum representing the queen.
     * ChessPieceType_kKing - Enum representing the king.
     */
    enum ChessPieceType_
    {
        ChessPieceType_kUnknown, ///< For debugging.
        ChessPieceType_kPawn,    ///< Enum representing the pawn.
        ChessPieceType_kBishop,  ///< Enum representing the bishop.
        ChessPieceType_kRook,    ///< Enum representing the rook.
        ChessPieceType_kKnight,  ///< Enum representing the knight.
        ChessPieceType_kQueen,   ///< Enum representing the queen.
        ChessPieceType_kKing     ///< Enum representing the king.Enum representing the king.
    };

    /**
     * @brief Class representing a chess piece.
     *
     * Every chess piece inherits from this class. The main idea of the class is to share a 
     * common interface, functions like 'CanMove()','CanMoveBoardSpecific()','GetChessPieceType()'.
     * 
     * It also shares a common variables across all of the pieces like:
     * 'm_PieceDrawable','m_pieceMovementRules','m_pieceBoardSpecificMovementRule' 
     * and 'm_PiecePosition'.
     *  
     * 
     * How the chess game logic works ?
     * 
     * Every piece have 2 arrays of 2 types of movement rules:
     *  - Movement rules that are independent of the rest of the board @see 'MovementRule'.
     *    like bishop diagonal movement or the rook non-diagonal movement.
     *  - Movement rules that depend on knowing the rest of the pieces on the board
     *    @see 'BoardSpecificMovementRule' like restrict movement if piece is Infront.
     * 
     * Using these movement rules together and providing a current and a new position we can
     * determine if a piece can move using 'CanMove()'.
     * 
     */
    class ChessPiece
    {
    public:
        /**
         * @brief Base constructor of a chess piece.
         *
         * The constructor sets a starting position and a drawable.
         * It also adds a 'CanMoveToTarget' to the 'm_PieceBoardSpecificMovementRule' array.
         * 
         * @param start_position The piece starting position.
         * @param drawable The piece drawable @see 'Drawable' on what drawable is.
         */
        ChessPiece(BoardPosition start_position, const std::shared_ptr<Drawable>& drawable);
        /**
         * @brief Sets the piece position.
         * 
         * This function can be overwritten if a child class must know when a position has been changed.
         * This is used in the 'Pawn' class to remove the ability for a pawn to move 2 positions after
         * its first movement.
         * @param new_position The new piece position.
         * 
         */
        virtual void SetPiecePosition(BoardPosition new_position) { m_PiecePosition = new_position; }
        /**
         * @brief Returns the position of the piece.
         * Where board position is a 2D position from [{0,a}] to [{8,h}]
         * @returns Returns the current board position of the piece.
         */
        BoardPosition GetPiecePosition()const { return m_PiecePosition; }
        /**
         * @brief Returns the chess piece type.
         * 
         * @note The function is virtual for the other classes to be considered a polymorphic 
         * classes.
         * 
         * @returns Returns the chess piece type.
         */
        virtual ChessPieceType_ GetChessPieceType()const = 0;
        /**
         * @brief Returns true if the new position obeys the piece movement rules.
         *
         * This function uses the 'MovementRule' array and returns a union of all of the
         * movement rules. This means hat if you have a piece like the Queen that
         * has a combination of rook and bishop movement rules only 1 of them must
         * be true for the movement to be considered true.
         * 
         * @returns Returns a union of all of the movement rules in the array.
         */
        bool CanMove(BoardPosition new_piece_position)const;
        /**
         * @brief Returns true if the new position obeys the 'BoardSpecificMovementRule'.
         * 
         * The function uses the 'BoardSpecificMovementRule' array and returns a intersection
         * of all of the movement rules.
         * This means that all of the rules must be true for this to return true.
         *
         * @returns Returns a intersection of all of the 'BoardSpecificMovementRule' in the array.
         */
        bool CanMoveBoardSpecific(BoardPosition new_piece_position, ChessBoard& board)const;
        /**
         * @brief Returns the piece drawable.
         * 
         * @returns Returns a weak ref of the piece drawable.
         */
        std::weak_ptr<Drawable> GetPieceDrawable() { return m_PieceDrawable; }
    protected:
        std::shared_ptr<Drawable> m_PieceDrawable; ///< The piece drawable. 
        BoardPosition m_PiecePosition{}; ///< The piece board position.
        std::vector<std::unique_ptr<MovementRule>> m_PieceMovementRules{}; ///< The piece movement rules. 
        std::vector<std::unique_ptr<BoardSpecificMovementRule>> m_PieceBoardSpecificMovementRule{}; ///< The piece board specific movement rules.
    };

#pragma region Child classes

    /**
     * @brief Child class of 'ChessPiece' representing the Queen chess piece.
     * 
     * Like every other child class the difference is how they configure their movement rules.
     * 
     * The specific 'Queen' current movement rules:
     *  - Movement rules:
     *      - 'RookMovementRule'
     *      - 'BishopMovementRule'
     * 
     *  - Board specific movement rules:
     *  - 'BlockableBoardSpecificMovementRule'
     */
    class Queen : public ChessPiece
    {
    public:
        /**
         * @brief Creates a child class of type 'Queen' and sets its movement rules.
         *  See the class definition on what those are.
         * 
         * The constructor class the base 'ChessPiece' class constructor and adds the child class
         * specific movement rules.
         * 
         * @param start_position The piece starting position.
         * @param drawable The piece drawable @see 'Drawable' on what drawable is.
         * 
         */
        Queen(BoardPosition start_position, const std::shared_ptr< Drawable>& drawable);
        /**
         * @brief Returns the enum value 'ChessPieceType_kQueen'.
         * 
         * @note This function is virtual for the class to be considered polymorphic.
         * @returns Returns the enum value 'ChessPieceType_kQueen'.
         */
        ChessPieceType_ GetChessPieceType()const override {return ChessPieceType_kQueen; }
    };

    /**
     * @brief Child class of 'ChessPiece' representing the Pawn chess piece.
     *
     * Like every other child class the difference is how they configure their movement rules.
     * The specific 'Pawn' current movement rules:
     *  - Movement rules:
     *      - 'SingleForwardMovementRule'
     *      - 'PawnSidewayCaptureRule'
     *      - 'PawnStartingMovementRule'
     *
     *  - Board specific movement rules:
     *      - 'BlockableBoardSpecificMovementRule'
     *      - 'PawnTeamMovementRule'
     *      - 'PawnCaptureBoardSpecificMovementRule'
     * 
     * Unlike other child classes this one have a custom logic on first movement.
     * This class override 'SetPiecePosition' to be notified when a piece position was changed.
     * If its the first time the class will remove one of the 'PawnStartingMovementRule' from the
     * array.
     */
    class Pawn : public ChessPiece
    {
    public:
        Pawn(BoardPosition start_position, const std::shared_ptr< Drawable>& drawable);
        /**
         * @brief Returns the enum value 'ChessPieceType_kPawn'.
         *
         * @note This function is virtual for the class to be considered polymorphic.
         * @returns Returns the enum value 'ChessPieceType_kPawn'.
         */
        ChessPieceType_ GetChessPieceType()const override { return ChessPieceType_kPawn; }
        /**
         * @brief Calls the base method and do custom pawn logic.
         *
         * Calls the base method first. Then it checks if the 'm_IsFirstMove' is true.
         * If it is then it sets it to false and removes the 'PawnStartingMovementRule'.
         * @param new_position The new piece position.
         *
         */
        void SetPiecePosition(BoardPosition new_position) override;
    private:
        bool m_IsFirstMove = true; ///< Keeps track if the current move was the first one.
    };
    /**
     * @brief Child class of 'ChessPiece' representing the 'Knight' chess piece.
     *
     * Like every other child class the difference is how they configure their movement rules.
     *
     * The specific 'Knight' current movement rules:
     *  - Movement rules:
     *      - 'KnightMovementRule'
     */
    class Knight : public ChessPiece
    {
    public:
        /**
         * @brief Creates a child class of type 'Knight' and sets its movement rules.
         *  See the class definition on what those are.
         *
         * The constructor class the base 'ChessPiece' class constructor and adds the child class
         * specific movement rules.
         *
         * @param start_position The piece starting position.
         * @param drawable The piece drawable @see 'Drawable' on what drawable is.
         *
         */
        Knight(BoardPosition start_position, const std::shared_ptr< Drawable>& drawable);
        /**
         * @brief Returns the enum value 'ChessPieceType_kKnight'.
         *
         * @note This function is virtual for the class to be considered polymorphic.
         * @returns Returns the enum value 'ChessPieceType_kKnight'.
         */
        ChessPieceType_ GetChessPieceType()const override { return ChessPieceType_kKnight; }
    };

    /**
     * @brief Child class of 'ChessPiece' representing the 'Bishop' chess piece.
     *
     * Like every other child class the difference is how they configure their movement rules.
     *
     * The specific 'Bishop' current movement rules:
     *  - Movement rules:
     *      - 'BishopMovementRule'
     *
     *  - Board specific movement rules:
     *  - 'BlockableBoardSpecificMovementRule'
     */
    class Bishop : public ChessPiece
    {
    public:
        /**
         * @brief Creates a child class of type 'Bishop' and sets its movement rules.
         *  See the class definition on what those are.
         *
         * The constructor class the base 'ChessPiece' class constructor and adds the child class
         * specific movement rules.
         *
         * @param start_position The piece starting position.
         * @param drawable The piece drawable @see 'Drawable' on what drawable is.
         *
         */
        Bishop(BoardPosition start_position, const std::shared_ptr< Drawable>& drawable);
        /**
         * @brief Returns the enum value 'ChessPieceType_kBishop'.
         *
         * @note This function is virtual for the class to be considered polymorphic.
         * @returns Returns the enum value 'ChessPieceType_kBishop'.
         */
        ChessPieceType_ GetChessPieceType()const override { return ChessPieceType_kBishop; }
    };
    /**
     * @brief Child class of 'ChessPiece' representing the 'Rook' chess piece.
     *
     * Like every other child class the difference is how they configure their movement rules.
     *
     * The specific 'Rook' current movement rules:
     *  - Movement rules:
     *      - 'RookMovementRule'
     *
     *  - Board specific movement rules:
     *  - 'BlockableBoardSpecificMovementRule'
     */
    class Rook : public ChessPiece
    {
    public:
        /**
         * @brief Creates a child class of type 'Rook' and sets its movement rules.
         *  See the class definition on what those are.
         *
         * The constructor class the base 'ChessPiece' class constructor and adds the child class
         * specific movement rules.
         *
         * @param start_position The piece starting position.
         * @param drawable The piece drawable @see 'Drawable' on what drawable is.
         *
         */
        Rook(BoardPosition start_position, const std::shared_ptr< Drawable>& drawable);
        /**
         * @brief Returns the enum value 'ChessPieceType_kRook'.
         *
         * @note This function is virtual for the class to be considered polymorphic.
         * @returns Returns the enum value 'ChessPieceType_kRook'.
         */
        ChessPieceType_ GetChessPieceType()const override { return ChessPieceType_kRook; }
    };
    /**
     * @brief Child class of 'ChessPiece' representing the 'King' chess piece.
     *
     * Like every other child class the difference is how they configure their movement rules.
     *
     * The specific 'King' current movement rules:
     *  - Movement rules:
     *      - 'KingMovement'
     *
     *  - Board specific movement rules:
     *  - 'BlockableBoardSpecificMovementRule'
     */
    class King : public ChessPiece
    {
    public:
        /**
         * @brief Creates a child class of type 'King' and sets its movement rules.
         *  See the class definition on what those are.
         *
         * The constructor class the base 'ChessPiece' class constructor and adds the child class
         * specific movement rules.
         *
         * @param start_position The piece starting position.
         * @param drawable The piece drawable @see 'Drawable' on what drawable is.
         *
         */
        King(BoardPosition start_position, const std::shared_ptr< Drawable>& drawable);
        /**
         * @brief Returns the enum value 'ChessPieceType_kKing'.
         *
         * @note This function is virtual for the class to be considered polymorphic.
         * @returns Returns the enum value 'ChessPieceType_kKing'.
         */
        ChessPieceType_ GetChessPieceType()const override{ return ChessPieceType_kKing; }
    };

#pragma endregion

}

