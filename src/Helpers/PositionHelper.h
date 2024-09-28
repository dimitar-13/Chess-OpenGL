#pragma once
#include "Core/Chess_pch.h"
#include "Gameplay/BoardPosition.h"
#include "Core/ApplicationData.h"
#include "Core/Application.h"

namespace Chess_Game
{
    class ScreenPositionHelper
    {
    public:
        ScreenPositionHelper(OrthographicMatrixBorder projection_border);
        glm::vec2 BoardToProjectionSpace(BoardPosition board_position)const;
        BoardPosition ScreenPositionToBoard(glm::vec2 orthographic_coordinates);
        void UpdateProjectionBorder(OrthographicMatrixBorder new_border);
        glm::vec2 GetBoardStart()const { return m_BoardStart; }
        glm::vec2 GetBoardSize()const { return m_BoardSize; }
        glm::vec2 SingleSquareSize()const { return m_SingleSquareSize; }
    private:
        void RecalculateVariables();
    private:
        OrthographicMatrixBorder m_CurrentBorder{};
        glm::vec2 m_SingleSquareSize{};
        glm::vec2 m_BoardStart{};
        glm::vec2 m_BoardSize{};

    };


}