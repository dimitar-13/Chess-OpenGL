#pragma once
#include "Core/Chess_pch.h"
#include "BoardPosition.h"
#include "ChessBoard.h"

namespace Chess_Game
{
    /**
     * While you are seeing the finished project I want to talk about why I ended up using this approach.
     *
     * As many would know programming is an iterative process meaning: No,I didn't have the idea just
     * laying around.
     * 
     * First attempt at this went using 'CanMove' virtuals within the 'ChessPiece' class.
     * Every chess piece can/will override the 'CanMove' function and alter its behaviour.
     * 
     * This however did introduce lots of code duplication. The movement rule for if a piece is blocked
     * by another is practically everywhere except the knight. The queen movement is bishop and rook in one
     * 
     * @note As of now writing this, one solution to this can be to use utils functions and when overriding 
     * the 'CanMove' to use those functions to evaluate the result. Meaning every piece will
     * override its 'CanMove' to use different util functions. 
     * 
     * At the end I used composition over inheritance to solve the issue. Meaning instead of every
     * piece overriding their 'CanMove' and introduce their behavior every piece will be composed of
     * different behaviors this turned out to be the most flexible solution. It reduced the code duplication 
     * and it also made it so every check is self contained meaning you don't have one big function with
     * a tower of nested if-else statements but one check per function. 
     *
     *
     * This approach has its flaws however
     * one of them is that say you want to use the 'SingleSquareRule','RookMovementRule' and 
     * 'BishopMovement' rule to create a King. This is a problem because when dealing with different 
     * if-else functions you choose either to get the union of them or intersection.
     * 
     * Example of union will be for the Queen the queen can move like a bishop or rook. A union states
     * I don't care if its a bishop move or rook as long one of them is true then movement is valid.
     * 
     * Example of intersection is the opposite of union where the next move must be true for both
     * statements.Intersection as a position that is true for both of them.
     * 
     * In the case of the King we want to union the Rook and the Bishop movement and then intersect 
     * with the single square movement. Without knowing what type of movement this is, this becomes 
     * hard.
     * 
     * Another interesting movement was when I needed to decide some game state rules.
     * First attempt at this was just to pass every board chess piece.
     * The idea was about if the king was checked. A rule that will prohibit the move if it resulted
     * the king beeing checked. The problem with that is that again to reduced code duplication I 
     * thought we can use every piece 'CanMove' because a king is checked when a piece can move to him
     * the problem with this approach was hard to debug and understand recursion.
     * 
     * 
     * It also let to circular dependencies to occur between 'ChessPiece' and 'MovementRule'. 
     * This is because the 'MovementRule' class needs access to an array or collection 
     * of 'ChessPiece' objects (to determine valid moves in relation to other pieces on the board), 
     * and at the same time, each 'ChessPiece' must have an associated 'MovementRule' to define its movement logic.
     * 
     * In other words:
     * - 'ChessPiece' relies on 'MovementRule' to define how it can move.
     * - 'MovementRule' depends on the state of the chess pieces to calculate valid moves, meaning it needs to be aware of 'ChessPiece'.
     * 
     * This interdependency between the two classes introduces a circular dependency issue.
     * 
     * The better approach as with most of these problems with dependencies is to introduce a middle man.
     * Someone or a system that has the bigger picture.
     * That is why the 2 classes 'ChessGame' and 'ChessBoard' exit.
     * 
     * 
     * 
     * How thins work ?
     * 
     * 
     * The idea of the movement logic is simple. Every chess piece have a collection of rules 
     * defining how a piece move. Like in real chess every piece have their own rules on how they can
     * or cannot move. The idea of the rules defined in this header file is simple. Determine if a piece
     * can move both isolated and relative to the board.
     * 
     * There are 2 types of movement rules that we can observe in the chess game.
     * - A rule on how a piece can move isolated(meaning if the piece is alone in the board how it can move).
     * - A rule on how the piece can move in relationship with the other pieces on the board.
     * 
     * An example of the stated above will be a bishop.
     * 1)
     * A bishop can move diagonally like this:
     *            \        /
     *             \      /
     *              bishop
     *             /      \
     *            /        \
     * This is an example of a rule that is isolated meaning I do not care about the other pieces on
     * the board.
     * 
     * 2)
     * If we image that now there is a piece on the way of the bishop say of the same team the 
     * bishop should not be able to move pass or at the same position.
     *          \            x
     *           \        pawn
     *            \        /
     *             \      /
     *              bishop
     *             /      \
     *            /        \
     * As you can see above the pawn is blocking the way of the bishop. This is called a
     * 'BoardSpecificMovementRule' in the project. Not all pieces are restricted to this rule
     * one example is the knight who can jump over pieces.
     * 
     * 
     * There is a third movement rules that are handled by a higher system.
     * These rules are related to game state. Say you make a move but the next move result in your
     * king beeing checked this move is not valid. Another example will be if you are already checked
     * but the next move you make will not result the check this is also an invalid move.
     * These moves are handled in the 'ChessGame'.
     * 
     * @see 'ChessGame' for more information on this.
     * 
     * 
     * OOP here is merely a organization/maintenance tool then anything else. You can get the same logic
     * with simple array of function pointers.The reason why I didn't go for function pointers is 
     * that I've already wrote the code and it felt like pre-mature optimization.
     * 
     * You can 100% do this in C with simple POD(plain old data) structs.
     */


    /**
     * @brief Interface representing a board specific movement rule.
     *
     *                                             ^
     * Explanation on how the movement logic works | beginning of the file.
     * 
     * A Interface representing a board specific movement rule. Where a board specific 
     * movement rule is such that dictates if a rule is valid based on the surrounding pieces. 
     * 
     */
    class BoardSpecificMovementRule
    {
    public:
        /**
         * @brief Virtual function used to determine if a move is valid in relationship
         * with the rest of the chess board pieces.
         * 
         * @param current_position The move start position.
         * @param new_position The move end position.
         * @param board The chess board bit mask.
         * 
         * @returns Returns true if the new position obeys the movement rule.
         */
        virtual bool CanMove(BoardPosition current_position,
            BoardPosition new_position, ChessBoard& board)const = 0;
    };
    /**
     * @brief Defines a pawn capture movement rule.
     *
     * This rule checks first if the new position is taken(there is a piece on it) using
     * the 'ChessBoard' and then again using the 'ChessBoard' gets both the current piece team and
     * the piece at the new position team. If there are not the same then this is true if they are
     * this is false.
     * 
     */
    class PawnCaptureBoardSpecificMovementRule : public BoardSpecificMovementRule
    {
    public:
        /**
         * @brief Returns true if there is a piece of the opposite team.
         *
         * This rule checks first if the new position is taken(there is a piece on it) using
         * the 'ChessBoard' and then again using the 'ChessBoard' gets both the current piece team and
         * the piece at the new position team. If there are not the same then this is true if they are
         * this is false.
         * 
         * @param current_position The move start position.
         * @param new_position The move end position.
         * @param board The chess board bit mask.
         *
         * @returns Returns true if there is a piece of the opposite team.
         */
         bool CanMove(BoardPosition current_position,
             BoardPosition new_position, ChessBoard& board)const override;
    };
    /**
     * @brief Defines pawn direction movement rule.
     *
     * This rule gets the pawn team and check if the direction is valid.
     * For white team pawn must move up and for black team pawn must move down.
     *
     */
    class PawnTeamMovementRule : public BoardSpecificMovementRule
    {
    public:
        /**
         * @brief Returns true if the pawn is moving in the right direction based on its team.
         *
         * This rule gets the pawn team and check if the direction is valid.
         * For white team pawn must move up and for black team pawn must move down.
         *
         * @param current_position The move start position.
         * @param new_position The move end position.
         * @param board The chess board bit mask.
         *
         * @returns Returns true if the pawn is moving in the right direction based on its team
         */
        bool CanMove(BoardPosition current_position,
            BoardPosition new_position, ChessBoard& board)const override;
    };
    /**
     * @brief Checks if there are any pieces blocking the way both from the same and opposite team.
     *
     * This rule calculates a direction and iterates every square until it reaches the end(new position).
     * If it detects any piece of any team at one of the positions it returns false.
     * 
     */
    class BlockableBoardSpecificMovementRule : public BoardSpecificMovementRule
    {
    public:
        /**
         * @brief Returns true if there are no pieces blocking the way.
         *
         * This rule calculates a direction and iterates every square until it reaches the end(new position).
         * If it detects any piece of any team at one of the positions it returns false.
         *
         * @param current_position The move start position.
         * @param new_position The move end position.
         * @param board The chess board bit mask.
         *
         * @returns Returns true if there are no pieces blocking the way.
         */
         bool CanMove(BoardPosition current_position,
             BoardPosition new_position, ChessBoard& board)const override;
    };
    /**
     * @brief Check if the new position is free or taken by opposite team piece.
     * 
     * This rule checks if the new position is taken and if its not then returns true.
     * If it is taken then it check to see if the piece on the new position is of a different team and
     * if it is it returns true.
     * 
     */
    class CanMoveToTarget : public BoardSpecificMovementRule
    {
    public:
        /**
         * @brief Returns true if the new position is free or is taken by piece of opposite team.
         *
         * This rule checks if the new position is taken and if its not then returns true.
         * If it is taken then it check to see if the piece on the new position is of a different team and
         * if it is it returns true.
         *
         * @param current_position The move start position.
         * @param new_position The move end position.
         * @param board The chess board bit mask.
         *
         * @returns Returns true if the new position is free or is taken by piece of opposite team.
         */
        bool CanMove(BoardPosition current_position,
            BoardPosition new_position, ChessBoard& board) const override;
    };
    /**
     * @brief Interface representing a movement rule.
     *
     *                                             ^
     * Explanation on how the movement logic works | beginning of the file.
     *
     * A Interface representing a board isolated movement rule. Where the movement is not aware of
     * the existence of the board or of the pieces around it but just a shape of the the movement.
     * It is like how explaining how a piece can move without specifying the rest of the board details.
     */
    class MovementRule
    {
    public:
        /**
         * @brief Virtual function used to determine if a move is valid.
         *
         * @param current_position The move start position.
         * @param new_position The move end position.
         *
         * @returns Returns true if the new position obeys the movement rule.
         */
        virtual bool CanMove(BoardPosition current_position,BoardPosition new_position)const = 0;
    };
    /**
     * @brief Movement rule defining rook movement.
     *
     * 
     * This movement rule defines rook movement. Rook movement is straight movement in any 4
     * directions up,down,right or left.
     */
    class RookMovementRule : public MovementRule
    {
    public:
        /**
         * @brief Returns true if the move obeys the rook movement rule.
         *
         * A rook movement is defined as a straight movement in any 4
         * directions up,down,right or left.
         *
         * @param current_position The start position.
         * @param new_position The new position.
         * 
         * @returns Returns true if the move obeys the rook movement rule.
         */
        bool CanMove(BoardPosition current_position, BoardPosition new_position)const override;
    };
    /**
     * @brief Movement rule defining bishop movement.
     *
     * This movement rule defines bishop movement. Bishop movement is defined as diagonal
     * movement in 4 directions up-left,down-left,up-right or down-right.
     */
    class BishopMovementRule : public MovementRule
    {
    public:
        /**
         * @brief Returns true if the move obeys the bishop movement rule.
         *
         * Bishop movement is defined as diagonal movement in one of 4 directions
         * up-left,down-left,up-right or down-right.
         *
         * @param current_position The start position.
         * @param new_position The new position.
         *
         * @returns Returns true if the move obeys the bishop movement rule.
         */
        bool CanMove(BoardPosition current_position, BoardPosition new_position)const override;
    };
    /**
     * @brief Movement rule defining knight movement.
     *
     * This movement rule defines knight movement. Knight movement looks like this:
     *           _
     *          |
     *          |       or      __|
     * Either 2 squares vertically and 1 horizontal or vice versa in any direction.
     * 
     */
    class KnightMovementRule : public MovementRule
    {
    public:
        /**
         * @brief Returns true if the move obeys the knight movement rule.
         *
         * Knight movement looks like this:
         *           _
         *          |
         *          |       or      __|
         * Either 2 squares vertically and 1 horizontal or vice versa in any direction.
         *
         * @param current_position The start position.
         * @param new_position The new position.
         *
         * @returns Returns true if the move obeys the knight movement rule.
         */
        bool CanMove(BoardPosition current_position, BoardPosition new_position)const override;
    };
    /**
     * @brief Movement rule defining king movement.
     *
     * The king movement rule is defined as single square move in any direction.
     *
     */
    class KingMovement : public MovementRule
    {
    public:
        /**
         * @brief Returns true if the move obeys the king movement rule.
         *
         * The king movement rule is defined as single square move in any direction.
         *
         * @param current_position The start position.
         * @param new_position The new position.
         *
         * @returns Returns true if the move obeys the king movement rule.
         */
        bool CanMove(BoardPosition current_position, BoardPosition new_position)const override;
    };
    /**
     * @brief Movement rule defining single forward movement.
     *
     * This movement rule is valid if the move is 1 square no matter the direction up or down.
     *
     */
    class SingleForwardMovementRule : public MovementRule
    {
    public:
        /**
         * @brief Returns true if the move is single square up or down.
         *
         * This movement rule is valid if the move is 1 square no matter the direction up or down.
         *
         * @param current_position The start position.
         * @param new_position The new position.
         *
         * @returns  Returns true if the move is single square up or down.
         */
        bool CanMove(BoardPosition current_position, BoardPosition new_position)const override;
    };
    /**
     * @brief Movement rule defining the first movement pawn can make.
     *
     * This movement rule is used for every first pawn move. In chess pawn can move 2 squares
     * up or down(depending on the team) for its first move. 
     *
     */
    class PawnStartingMovementRule : public MovementRule
    {
    public:
        /**
         * @brief Returns true if the move is 2 square up or down.
         *
         * This movement rule is used for every first pawn move. In chess pawn can move 2 squares
         * up or down(depending on the team) for its first move.
         * 
         * @param current_position The start position.
         * @param new_position The new position.
         *
         * @returns Returns true if the move is 2 square up or down.
         */
        bool CanMove(BoardPosition current_position, BoardPosition new_position)const override;
    };
    /**
     * @brief Movement rule defining the pawn sideway movement.
     *
     * This movement rule is of the 'MovementRule' inheritance tree. This means that it doesn't not
     * care if there is a piece on the sideways but rather just makes the move possible.The check if
     * there is a piece is done in the 'PawnCaptureBoardSpecificMovementRule'.
     */
    class PawnSidewayCaptureRule : public MovementRule
    {
    public:
        /**
         * @brief Returns true if the move is 1 square diagonal to the right or left(both up or down).
         *
         * This movement rule is of the 'MovementRule' inheritance tree. This means that it doesn't not
         * care if there is a piece on the sideways but rather just makes the move possible.The check if
         * there is a piece is done in the 'PawnCaptureBoardSpecificMovementRule'.
         *
         * @param current_position The start position.
         * @param new_position The new position.
         *
         * @returns Returns true if the move is 1 
         *          square diagonal to the right or left(both up or down).
         */
        bool CanMove(BoardPosition current_position, BoardPosition new_position)const override;
    };


}