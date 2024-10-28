#pragma once
#include "Pch/Chess_pch.h"
#include "Gameplay/BoardPosition.h"
#include "Core/ApplicationData.h"
#include "Core/Application.h"

namespace Chess_Game
{
    /**
     * @brief Helper class for converting orthographic screen positions to ChessBoard postion and vice versa.
     * 
     * The class is responsible for 2 things.
     *  - For converting positions from orthographic screen positions to ChessBoard postion and vice versa.
     *  - For positioning the chess board in a way that is always in the middle of the screen.
     * 
     * From provided 'OrthographicMatrixBorder' the class calculates the chess board start position and size so that 
     * the chess board is always in the middle of the screen.
     * 
     */
    class ScreenPositionHelper
    {
    public:
        ScreenPositionHelper(const OrthographicMatrixBorder& projection_border);
        /**
         * @brief Convers a board position to 2D position in orthographic projection space.
         * 
         * The conversion is made using this formula:
         * 'converted_position = board_start + (single_square * offset)'
         * Where offset is: 'offset = { board_position.x - 'a', board_position.y}'
         * 
         * @param board_position The board position to be converted.
         * @returns The converted board position to orthographic projection space.
         */
        const glm::vec2& BoardToProjectionSpace(BoardPosition board_position)const;
        /**
         * @brief Convers a 2D position in orthographic projection space to board position.
         *
         * The conversion is made using this formula:
         * 'converted_position = board_start + (single_square * offset)'
         * Where offset is: 'offset = { board_position.x - 'a', board_position.y}'
         *
         * @param orthographic_coordinates The 2D position in orthographic projection space to be converted.
         * @returns The converted 2D position in orthographic projection space to board position.
         */
        BoardPosition ScreenPositionToBoard(const glm::vec2& orthographic_coordinates)const;
        /**
         * @brief Updates the orthographic projection borders and recalculates the chess board position and size. 
         * @param new_border New orthographic projection borders.
         */
        void UpdateProjectionBorder(const OrthographicMatrixBorder& new_border);
        /**
         * @brief Gets the board lower left corner.
         * @returns Returns the board lower left corner position in orthographic coordinate system.
         */
        const glm::vec2& GetBoardStart()const { return m_BoardStart; }
        /**
         * @brief Gets the board size(by size I mean half of its width).
         * @returns Returns the size of the chess board.
         */
        const glm::vec2& GetBoardSize()const { return m_BoardSize; }
        /**
         * @brief Returns the size of a single square of the chess board.
         * @returns Returns the size of a single square of the chess board.
         */
        const glm::vec2& SingleSquareSize()const { return m_SingleSquareSize; }
    private:
        /**
         * @brief Calculates the chess board variables(size of single square, board start position and board size).
         */
        void RecalculateVariables();
    private:
        OrthographicMatrixBorder m_CurrentBorder{}; ///< Current orthographic matrix boarders.
        glm::vec2 m_SingleSquareSize{};             ///< Single chess board square.
        glm::vec2 m_BoardStart{};                   ///< Start of the chess board(lower left coner).
        glm::vec2 m_BoardSize{};                    ///< The size of the chess board.

    };


}