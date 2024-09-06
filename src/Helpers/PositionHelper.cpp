#include "PositionHelper.h"

Chess_Game::ScreenPositionHelper::ScreenPositionHelper(OrthographicMatrixBorder projection_border)
    :m_CurrentBorder(projection_border)
{
    RecalculateVariables();
}

glm::vec2 Chess_Game::ScreenPositionHelper::BoardToScreenPosition(BoardPosition board_position)const
{
    glm::vec2 offset = { board_position.horizontalPosition - 'a',board_position.VerticalPosition - 1};
    glm::vec2 square_to_center_offset = m_SingleSquareSize / 2.0f;
    glm::vec2 result = m_BoardStart + square_to_center_offset + (m_SingleSquareSize * offset);

    return result;
}

Chess_Game::BoardPosition Chess_Game::ScreenPositionHelper::ScreenPositionToBoard(
    glm::vec2 orthographic_coordinates)
{
    if(abs(orthographic_coordinates.x) > abs(m_BoardStart.x) ||
        abs(orthographic_coordinates.y) > abs(m_BoardStart.y))
        return BoardPosition{-1,-1};

    //Derived by this formula:'pos = board_start + (single_square * offset)'
    //Where offset is: 'offset = { board_position.x - 'a', board_position.y}'

    glm::vec2 offset = (orthographic_coordinates - m_BoardStart)/m_SingleSquareSize;

    BoardPosition result = { offset.x + 'a', 9 - offset.y};
    return result;
}

void Chess_Game::ScreenPositionHelper::UpdateProjectionBorder(OrthographicMatrixBorder new_border)
{
    m_CurrentBorder = new_border;
    RecalculateVariables();
}

void Chess_Game::ScreenPositionHelper::RecalculateVariables()
{
    float smallest_screen_dimension = 
        std::min(m_CurrentBorder.right + abs(m_CurrentBorder.left),
            m_CurrentBorder.top + abs(m_CurrentBorder.bottom));

    smallest_screen_dimension = smallest_screen_dimension / 3.0f;

    float third_of_window_width = m_CurrentBorder.right / 3.f;

    m_BoardSize = glm::vec2(smallest_screen_dimension, smallest_screen_dimension);
    m_BoardStart = -m_BoardSize;
    m_SingleSquareSize = (2.f * m_BoardSize) / glm::vec2(8.0f);
}
