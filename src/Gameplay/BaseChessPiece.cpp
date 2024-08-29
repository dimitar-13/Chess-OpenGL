#include "BaseChessPiece.h"

bool Chess_Game::ChessPiece::CanMove(BoardPosition new_piece_position)
{
    for (auto& movement_rule : m_pieceMovementRules)
    {
        if (movement_rule->CanMove(m_PiecePosition, new_piece_position))
            return true;
    }

    return false;
}

bool Chess_Game::ChessPiece::CanMoveBoardSpecific(BoardPosition new_piece_position,ChessBoard& board)
{
    for (auto& movement_rule : m_pieceBoardSpecificMovementRule)
    {
        if (!movement_rule->CanMove(this, new_piece_position, board))
            return false;
    }

    return true;
}

Chess_Game::Queen::Queen(BoardPosition start_position):ChessPiece(start_position)
{
    this->m_pieceMovementRules.push_back(std::make_unique<RookMovementRule>());
    this->m_pieceMovementRules.push_back(std::make_unique<BishopMovementRule>());

    this->m_pieceBoardSpecificMovementRule.push_back(std::make_unique<BlockableBoardSpecificMovementRule>());
}

Chess_Game::Bishop::Bishop(BoardPosition start_position):ChessPiece(start_position)
{
    this->m_pieceMovementRules.push_back(std::make_unique<BishopMovementRule>());
}

Chess_Game::Pawn::Pawn(BoardPosition start_position):ChessPiece(start_position)
{
    this->m_pieceMovementRules.push_back(std::make_unique<SingleForwardMovementRule>());
    this->m_pieceMovementRules.push_back(std::make_unique<PawnSidewayCaptureRule>());
    this->m_pieceMovementRules.push_back(std::make_unique<PawnStartingMovementRule>());
}

void Chess_Game::Pawn::OnPositionChanged()
{
    if (is_first_move)
    {
        this->m_pieceMovementRules.pop_back();
        is_first_move = false;
    }
}

Chess_Game::Knight::Knight(BoardPosition start_position):ChessPiece(start_position)
{
    this->m_pieceMovementRules.push_back(std::make_unique<KnightMovementRule>());

}

Chess_Game::Rook::Rook(BoardPosition start_position):ChessPiece(start_position)
{
    this->m_pieceMovementRules.push_back(std::make_unique<RookMovementRule>());
}

Chess_Game::King::King(BoardPosition start_position):ChessPiece(start_position)
{
    this->m_pieceMovementRules.push_back(std::make_unique<KingMovement>());

    //this->m_pieceBoardSpecificMovementRule.push_back(std::make_unique<KingMateBoardSpecificMovementRule>());

}
