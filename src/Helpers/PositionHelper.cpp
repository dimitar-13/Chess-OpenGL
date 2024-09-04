#include "PositionHelper.h"

Chess_Game::ScreenPositionHelper::ScreenPositionHelper(OrthographicMatrixBorder projection_border)
    :m_CurrentBorder(projection_border)
{
    RecalculateVariables();
}

glm::vec2 Chess_Game::ScreenPositionHelper::BoardToScreenPosition(BoardPosition board_position)const
{
    glm::vec2 offset = { board_position.horizontalPosition - 'a', board_position.VerticalPosition};
    glm::vec2 result = m_BoardPosition + (m_SingleSquareSize * offset);
    return result;
}

Chess_Game::BoardPosition Chess_Game::ScreenPositionHelper::ScreenPositionToBoard(
    glm::vec2 orthographic_coordinates)
{
    if(orthographic_coordinates.x < m_BoardPosition.x || orthographic_coordinates.x > m_BoardPosition.x*2.f)
        return BoardPosition{-1,-1};

    //Derived by this formula:'pos = board_start + (single_square * offset)'
    //Where offset is: 'offset = { board_position.x - 'a', board_position.y}'

    glm::vec2 offset = (orthographic_coordinates -  m_BoardPosition)/m_SingleSquareSize;

    BoardPosition result = { offset.x + 'a', offset.y};
    return result;
}

void Chess_Game::ScreenPositionHelper::UpdateProjectionBorder(OrthographicMatrixBorder new_border)
{
    m_CurrentBorder = new_border;
    RecalculateVariables();
}

void Chess_Game::ScreenPositionHelper::RecalculateVariables()
{
    constexpr glm::vec2 kBoardRelativePosition = glm::vec2(20.f);

    float window_center_width = m_CurrentBorder.right / 3.f;
    float board_height = m_CurrentBorder.top - (2.0f * kBoardRelativePosition.y);
    m_BoardPosition = glm::vec2(window_center_width, kBoardRelativePosition.y);

    m_SingleSquareSize = glm::vec2(window_center_width, board_height) / glm::vec2(8.0f);
}
