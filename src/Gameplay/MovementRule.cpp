#include "MovementRule.h"
#include "BaseChessPiece.h"
#include "Logging/Logger.h"
#include "ChessBoard.h"
#include <glm/glm.hpp>
bool Chess_Game::RookMovementRule::CanMove(BoardPosition current_position, BoardPosition new_position) const
{
    BoardPosition new_position_delta{};
    new_position_delta.VerticalPosition = new_position.VerticalPosition - current_position.VerticalPosition;
    new_position_delta.horizontalPosition = new_position.horizontalPosition - current_position.horizontalPosition;
    bool test = (new_position_delta.VerticalPosition != 0 && new_position_delta.horizontalPosition == 0) ||
        (new_position_delta.VerticalPosition == 0 && new_position_delta.horizontalPosition != 0);
    return test;
}

bool Chess_Game::BishopMovementRule::CanMove(BoardPosition current_position, BoardPosition new_position) const
{
    BoardPosition position_offset{};
    position_offset.VerticalPosition = new_position.VerticalPosition - current_position.VerticalPosition;
    position_offset.horizontalPosition = new_position.horizontalPosition - current_position.horizontalPosition;

    return abs(position_offset.VerticalPosition) == abs(position_offset.horizontalPosition);
}

bool Chess_Game::KnightMovementRule::CanMove(BoardPosition current_position, BoardPosition new_position) const
{
    BoardPosition position_offset{};
    position_offset.VerticalPosition = new_position.VerticalPosition - current_position.VerticalPosition;
    position_offset.horizontalPosition = new_position.horizontalPosition - current_position.horizontalPosition;

    return (abs(position_offset.VerticalPosition) == 2 && abs(position_offset.horizontalPosition) == 1) ||
        (abs(position_offset.VerticalPosition) == 1 && abs(position_offset.horizontalPosition) == 2);
}

bool Chess_Game::BlockableBoardSpecificMovementRule::CanMove(ChessPiece* piece_to_check,
    BoardPosition new_position, ChessBoard& board) const
{
    BoardPosition current_position = piece_to_check->GetPiecePosition();
    BoardPosition move_delta{};
    BoardPosition move_direction{};

    move_delta.VerticalPosition = new_position.VerticalPosition - current_position.VerticalPosition;
    move_delta.horizontalPosition = new_position.horizontalPosition - current_position.horizontalPosition;

    move_direction.VerticalPosition = move_delta.VerticalPosition == 0 ? 0 :
        move_delta.VerticalPosition / abs(move_delta.VerticalPosition);

    move_direction.horizontalPosition = move_delta.horizontalPosition == 0 ? 0 :
        move_delta.horizontalPosition / abs(move_delta.horizontalPosition);

    unsigned char movement_count =
        std::max(abs(move_delta.VerticalPosition), abs(move_delta.horizontalPosition));

    BoardPosition next_position = current_position;

    for (unsigned char i = 0; i < movement_count - 1; i++)
    {

        next_position.VerticalPosition += move_direction.VerticalPosition;
        next_position.horizontalPosition += move_direction.horizontalPosition;

        if (board.GetChessboardPositionFlag(next_position) & BoardPositionFlags_kIsPositionOcupied)
            return false;
    }

    return true;
}

bool Chess_Game::CanMoveToTarget::CanMove(ChessPiece* piece_to_check,
    BoardPosition new_position, ChessBoard& board) const
{
    BoardPosition current_position = piece_to_check->GetPiecePosition();

    if (!(board.GetChessboardPositionFlag(new_position) & BoardPositionFlags_kIsPositionOcupied))
    {
        return true;
    }
    
    bool is_current_piece_white =board.GetChessboardPositionFlag(current_position) & BoardPositionFlags_kIsPieceFromWhiteTeam;

    bool is_new_target_piece_white = board.GetChessboardPositionFlag(new_position) & BoardPositionFlags_kIsPieceFromWhiteTeam;

    return !(is_current_piece_white == is_new_target_piece_white);
}

bool Chess_Game::PawnCaptureBoardSpecificMovementRule::CanMove(ChessPiece* piece_to_check,
    BoardPosition new_position, ChessBoard& board) const
{
    BoardPosition current_position = piece_to_check->GetPiecePosition();
    BoardPosition move_offset{};

    bool is_new_position_taken = board.GetChessboardPositionFlag(new_position) & BoardPositionFlags_kIsPositionOcupied;

    move_offset.VerticalPosition = new_position.VerticalPosition - current_position.VerticalPosition;
    move_offset.horizontalPosition = new_position.horizontalPosition - current_position.horizontalPosition;

    if (abs(move_offset.VerticalPosition) == move_offset.horizontalPosition)
    {
        return is_new_position_taken;
    }

    return !is_new_position_taken;
}

bool Chess_Game::PawnStartingMovementRule::CanMove(BoardPosition current_position, BoardPosition new_position) const
{
    BoardPosition position_offset{};
    position_offset.VerticalPosition = new_position.VerticalPosition - current_position.VerticalPosition;
    position_offset.horizontalPosition = new_position.horizontalPosition - current_position.horizontalPosition;

    if (position_offset.horizontalPosition != 0)
        return false;
    if (abs(position_offset.VerticalPosition) != 2)
        return false;

    return true;
}

bool Chess_Game::PawnSidewayCaptureRule::CanMove(BoardPosition current_position, BoardPosition new_position) const
{
    BoardPosition position_offset{};
    position_offset.VerticalPosition = new_position.VerticalPosition - current_position.VerticalPosition;
    position_offset.horizontalPosition = new_position.horizontalPosition - current_position.horizontalPosition;

    if (position_offset.VerticalPosition < 0)
        return false;

    return abs(position_offset.VerticalPosition) == 1 && abs(position_offset.horizontalPosition) == 1;
}

bool Chess_Game::SingleForwardMovementRule::CanMove(BoardPosition current_position, BoardPosition new_position) const
{
    BoardPosition position_offset{};
    position_offset.VerticalPosition = new_position.VerticalPosition - current_position.VerticalPosition;
    position_offset.horizontalPosition = new_position.horizontalPosition - current_position.horizontalPosition;

    return position_offset.horizontalPosition == 0 && abs(position_offset.VerticalPosition) == 1;
}

bool Chess_Game::KingMovement::CanMove(BoardPosition current_position, BoardPosition new_position) const
{
    BoardPosition position_delta{};
    position_delta.VerticalPosition = new_position.VerticalPosition - current_position.VerticalPosition;
    position_delta.horizontalPosition = new_position.horizontalPosition - current_position.horizontalPosition;
    
    return abs(position_delta.VerticalPosition) + abs(position_delta.horizontalPosition) == 1 ||
        abs(position_delta.VerticalPosition) + abs(position_delta.horizontalPosition) == 2;
}

bool Chess_Game::PawnTeamMovementRule::CanMove(ChessPiece* piece_to_check, 
    BoardPosition new_position, ChessBoard& board) const
{
    BoardPosition piece_position = piece_to_check->GetPiecePosition();
    BoardPosition position_delta{};

    position_delta.VerticalPosition = new_position.VerticalPosition - piece_position.VerticalPosition;
    position_delta.horizontalPosition = new_position.horizontalPosition - piece_position.horizontalPosition;

    bool is_pawn_from_white_team = board.GetChessboardPositionFlag(piece_position) & BoardPositionFlags_kIsPieceFromWhiteTeam;

    return (position_delta.VerticalPosition > 1 && is_pawn_from_white_team) || 
           (position_delta.VerticalPosition < 1 && !is_pawn_from_white_team);
}
