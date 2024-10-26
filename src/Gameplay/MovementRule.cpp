#include "MovementRule.h"
#include "BaseChessPiece.h"
#include "Logging/Logger.h"
#include "ChessBoard.h"

bool Chess_Game::RookMovementRule::CanMove(BoardPosition current_position, BoardPosition new_position) const
{
    BoardPosition position_delta = new_position - current_position;

    return (position_delta.vertical_position != 0 && position_delta.horizontal_position == 0) ||
        (position_delta.vertical_position == 0 && position_delta.horizontal_position != 0);
}

bool Chess_Game::BishopMovementRule::CanMove(BoardPosition current_position, BoardPosition new_position) const
{
    BoardPosition position_delta = new_position - current_position;

    return abs(position_delta.vertical_position) == abs(position_delta.horizontal_position);
}

bool Chess_Game::KnightMovementRule::CanMove(BoardPosition current_position, BoardPosition new_position) const
{
    BoardPosition position_delta = new_position - current_position;

    return (abs(position_delta.vertical_position) == 2 && abs(position_delta.horizontal_position) == 1) ||
        (abs(position_delta.vertical_position) == 1 && abs(position_delta.horizontal_position) == 2);
}

bool Chess_Game::BlockableBoardSpecificMovementRule::CanMove(BoardPosition current_position,
    BoardPosition new_position, ChessBoard& board) const
{
    BoardPosition position_delta = new_position - current_position;
    BoardPosition move_direction{};
        
    move_direction = BoardPosition::Normalize(position_delta);
    
    unsigned char movement_count =
        std::max(abs(position_delta.vertical_position), abs(position_delta.horizontal_position));
    
    BoardPosition next_position = current_position;
    
    for (unsigned char i = 0; i < movement_count - 1; i++)
    {
    
        next_position.vertical_position += move_direction.vertical_position;
        next_position.horizontal_position += move_direction.horizontal_position;
    
        if (board.GetChessboardPositionFlag(next_position) & BoardPositionFlags_kIsPositionOccupied)
            return false;
    }
    
    return true;
}

bool Chess_Game::CanMoveToTarget::CanMove(BoardPosition current_position,
    BoardPosition new_position, ChessBoard& board) const
{    
    if (!(board.GetChessboardPositionFlag(new_position) & BoardPositionFlags_kIsPositionOccupied))
    {
        return true;
    }
    
    bool is_current_piece_white = board.GetChessboardPositionFlag(current_position) & BoardPositionFlags_kIsPieceFromWhiteTeam;
    
    bool is_new_target_piece_white = board.GetChessboardPositionFlag(new_position) & BoardPositionFlags_kIsPieceFromWhiteTeam;
    
    return !(is_current_piece_white == is_new_target_piece_white);
}

bool Chess_Game::PawnCaptureBoardSpecificMovementRule::CanMove(BoardPosition current_position,
    BoardPosition new_position, ChessBoard& board) const
{  
    BoardPosition position_delta = new_position - current_position;

    bool is_new_position_taken = 
        board.GetChessboardPositionFlag(new_position) & BoardPositionFlags_kIsPositionOccupied;

    if (abs(position_delta.vertical_position) == abs(position_delta.horizontal_position))
    {
        BoardPositionFlags_ current_piece_flags = board.GetChessboardPositionFlag(current_position);
        BoardPositionFlags_ new_position_flags = board.GetChessboardPositionFlag(new_position);
        bool is_current_piece_from_white_team = current_piece_flags & BoardPositionFlags_kIsPieceFromWhiteTeam;
        bool is_new_piece_from_white_team = new_position_flags & BoardPositionFlags_kIsPieceFromWhiteTeam;

        return is_new_position_taken && is_current_piece_from_white_team != is_new_piece_from_white_team;
    }
    
    return !is_new_position_taken;
}

bool Chess_Game::PawnStartingMovementRule::CanMove(BoardPosition current_position, BoardPosition new_position) const
{
    BoardPosition position_delta = new_position - current_position;

    if (position_delta.horizontal_position != 0)
        return false;
    if (abs(position_delta.vertical_position) != 2)
        return false;

    return true;
}

bool Chess_Game::PawnSidewayCaptureRule::CanMove(BoardPosition current_position, BoardPosition new_position) const
{
    BoardPosition position_delta = new_position - current_position;

    return abs(position_delta.vertical_position) == 1 && abs(position_delta.horizontal_position) == 1;
}

bool Chess_Game::SingleForwardMovementRule::CanMove(BoardPosition current_position, BoardPosition new_position) const
{
    BoardPosition position_delta = new_position - current_position;

    return position_delta.horizontal_position == 0 && abs(position_delta.vertical_position) == 1;
}

bool Chess_Game::KingMovement::CanMove(BoardPosition current_position, BoardPosition new_position) const
{
    BoardPosition position_delta = new_position - current_position;

    if (abs(position_delta.vertical_position) > 1 || abs(position_delta.horizontal_position) > 1)
        return false;

    return abs(position_delta.vertical_position) + abs(position_delta.horizontal_position) == 1 ||
        abs(position_delta.vertical_position) + abs(position_delta.horizontal_position) == 2;
}

bool Chess_Game::PawnTeamMovementRule::CanMove(BoardPosition current_position,
    BoardPosition new_position, ChessBoard& board) const
{
    BoardPosition position_delta = new_position - current_position;
    
    bool is_pawn_from_white_team = board.GetChessboardPositionFlag(current_position) & BoardPositionFlags_kIsPieceFromWhiteTeam;
    
    return (position_delta.vertical_position >= 1 && is_pawn_from_white_team) ||
        (position_delta.vertical_position <= -1 && !is_pawn_from_white_team);
}
