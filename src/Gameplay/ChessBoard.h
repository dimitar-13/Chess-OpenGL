#pragma once
#include "BoardPosition.h"
#include "Core/Chess_pch.h"

namespace Chess_Game
{
    /**
     * @brief Enum representing board position flags.
     * BoardPositionFlags_kIsPositionOccupied - Set if there is a chess piece on the position.
     * BoardPositionFlags_kIsPieceFromBlackTeam - Set if the piece is from the black team.
     * BoardPositionFlags_kIsPieceFromWhiteTeam - Set if the piece is from the white team.
     * BoardPositionFlags_kIsPieceImmortal - Set if the piece is immortal where an immortal piece is
     *                                       a piece that cant be claimed or in chess the King piece.
     */
    enum BoardPositionFlags_ : uint8_t
    {
        BoardPositionFlags_kIsPositionOccupied = 1 << 0,   ///< Set if there is a chess piece on the position.
        BoardPositionFlags_kIsPieceFromBlackTeam = 1 << 1, ///< Set if the piece is from the black team.
        BoardPositionFlags_kIsPieceFromWhiteTeam = 1 << 2, ///< Set if the piece is from the white team.
        BoardPositionFlags_kIsPieceImmortal = 1 << 3,      ///< Set if the piece is immortal.
    };

    /**
     * @brief ChessBoard class holding information for every position on the board.
     *
     * The class uses a bit mask to keep track of every chess board position.
     * Every position on the board has a 'BoardPositionFlags_' flag that gives the following information:
     * - The piece team(is white or black team).
     * - Is the position occupied.
     * - Is the piece immortal(an immortal piece is the king).
     * 
     * The class provides a way of getting and setting the flags for every position.
     * 
     */
    class ChessBoard
    {
    public:
        /**
         * @brief Returns the flags on the provided position.
         * 
         * @param position The position you want to get the flags.
         * 
         * @returns Returns the flags on the provided position.
         */
        BoardPositionFlags_ GetChessboardPositionFlag(BoardPosition position);
        /**
         * @brief Sets the flags on the provided position.
         *
         * @param position The position you want to set the flags.
         * @param bit_mask The flags to set.      
         */
        void SetChessboardPositionFlag(BoardPosition position, BoardPositionFlags_ bit_mask);
        /**
         * @brief Returns true if the position is at vertical start or end of the board.
         * 
         * Utility function that returns true if the position is at the start or at the end of the board
         * in vertical sense.
         * 
         * @param position_to_check Position to check.
         * 
         * @returns Returns true if the position is at vertical start or end of the board.
         */
        static bool IsPositionAtVerticalBoarder(BoardPosition position_to_check);
        /**
         * @brief Returns true if the position is at horizontal start or end of the board.
         *
         * Utility function that returns true if the position is at the start or at the end of the board
         * in horizontal sense.
         *
         * @param position_to_check Position to check.
         *
         * @returns Returns true if the position is at horizontal start or end of the board.
         */
        static bool IsNewPositionInBounds(BoardPosition position_to_check);
    private:
        /**
         * @brief Calculates the 'chessboard_bit_mask' index from the provided 'board_position'.
         * 
         * @returns Returns the index of the specified board position.
         */
        size_t GetIndexByChessboardPosition(BoardPosition board_position);
    private:
        std::array<BoardPositionFlags_, 64> chessboard_bit_mask{}; ///< Array holding every board position flag.
    };

}