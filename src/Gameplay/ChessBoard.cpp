#include "ChessBoard.h"

size_t Chess_Game::ChessBoard::GetIndexByChessboardPosition(BoardPosition board_position)
{
    constexpr size_t kPerRowPositionCount = 8;
    size_t result{};
    size_t vertical_based_array_offset = static_cast<size_t>(board_position.vertical_position - 1);

    result += static_cast<size_t>(board_position.horizontal_position - 'a');
    result += vertical_based_array_offset * kPerRowPositionCount;
    return result;
}

Chess_Game::BoardPositionFlags_ Chess_Game::ChessBoard::GetChessboardPositionFlag(BoardPosition position)
{
    size_t index = GetIndexByChessboardPosition(position);
    if (index < chessboard_bit_mask.size())
        return chessboard_bit_mask[index];

    return BoardPositionFlags_();
}

void Chess_Game::ChessBoard::SetChessboardPositionFlag(BoardPosition position, BoardPositionFlags_ bit_mask)
{
    size_t index = GetIndexByChessboardPosition(position);

    if (index < chessboard_bit_mask.size())
        chessboard_bit_mask[index] = bit_mask;
}

bool Chess_Game::ChessBoard::IsPositionAtVerticalBoarder(BoardPosition position_to_check)
{
    return position_to_check.vertical_position == 8 || position_to_check.vertical_position == 1;
}

bool Chess_Game::ChessBoard::IsNewPositionInBounds(BoardPosition position_to_check)
{
    return (position_to_check.vertical_position <= 8 && position_to_check.vertical_position >= 1) &&
           (position_to_check.horizontal_position <= 'h' && position_to_check.horizontal_position >= 'a');
}
