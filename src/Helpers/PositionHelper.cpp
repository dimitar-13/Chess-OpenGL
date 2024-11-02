#include "PositionHelper.h"

Chess_Game::ScreenPositionHelper::ScreenPositionHelper(const OrthographicMatrixBorder& projection_border)
    :m_CurrentBorder(projection_border)
{
    RecalculateVariables();
}

const glm::vec2& Chess_Game::ScreenPositionHelper::BoardToProjectionSpace(BoardPosition board_position)const
{
    glm::vec2 offset = { board_position.horizontal_position - 'a',board_position.vertical_position - 1};
    glm::vec2 square_to_center_offset = m_SingleSquareSize / 2.0f;
    glm::vec2 result = m_BoardStart + square_to_center_offset + (m_SingleSquareSize * offset);
    return result;
}

Chess_Game::BoardPosition Chess_Game::ScreenPositionHelper::ScreenPositionToBoard( const glm::vec2& orthographic_coordinates)const
{
    if(abs(orthographic_coordinates.x) > abs(m_BoardStart.x) ||
        abs(orthographic_coordinates.y) > abs(m_BoardStart.y))
        return BoardPosition{-1,-1};

    //Derived by this formula:'pos = board_start + (single_square * offset)'
    //Where offset is: 'offset = { board_position.x - 'a', board_position.y}'

    glm::vec2 offset = (orthographic_coordinates - m_BoardStart)/m_SingleSquareSize;

    BoardPosition result = { offset.x + 'a', 9.0f - offset.y};
    return result;
}

void Chess_Game::ScreenPositionHelper::UpdateProjectionBorder(const OrthographicMatrixBorder& new_border)
{
    m_CurrentBorder = new_border;
    RecalculateVariables();
}

void Chess_Game::ScreenPositionHelper::RecalculateVariables()
{
    glm::vec2 board_width;
    constexpr glm::vec2 KSquaresPerRowAndColumn = glm::vec2(8.0f);

    float smallest_screen_dimension = std::min(m_CurrentBorder.right + abs(m_CurrentBorder.left),
                                               m_CurrentBorder.top + abs(m_CurrentBorder.bottom));
    smallest_screen_dimension = smallest_screen_dimension / 3.0f;

    /**
    * By getting the 1/3th of this we effectively did is this:
    * 
    * Screen
    * _____________________________
    * |      |              |      |
    * |      |              |      |
    * |______|______________|______|
    *                       ^    
    *                       |
    *                       1/3 of the screen(If we say these are the same size in theory visually Im lazy to make them).
    * 
    * Because the orthographic matrix middle is at 0,0(center of the screen) to get symmetrical board in both quadrants
    * we just need to set the beginning to be the negative of whatever 'm_BoardSize' is.
    */
    m_BoardSize = glm::vec2(smallest_screen_dimension, smallest_screen_dimension);
    board_width = (2.f * m_BoardSize);
    m_BoardStart = -m_BoardSize;

    m_SingleSquareSize = board_width / KSquaresPerRowAndColumn;
}
