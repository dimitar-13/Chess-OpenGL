#include "BaseChessPiece.h"
#include "Logging/Logger.h"

#pragma region Base class

Chess_Game::ChessPiece::ChessPiece(BoardPosition start_position,
    const std::shared_ptr<Drawable>& drawable):
    m_PiecePosition(start_position), m_PieceDrawable(drawable)
{
    m_PieceBoardSpecificMovementRule.push_back(std::make_unique<CanMoveToTarget>());
}

bool Chess_Game::ChessPiece::CanMove(BoardPosition new_piece_position)const
{
    for (auto& movement_rule : m_PieceMovementRules)
    {
        if (movement_rule->CanMove(m_PiecePosition, new_piece_position))
            return true;
    }
    return false;
}

bool Chess_Game::ChessPiece::CanMoveBoardSpecific(BoardPosition new_piece_position,ChessBoard& board)const
{
    for (auto& movement_rule : m_PieceBoardSpecificMovementRule)
    {
        if (!movement_rule->CanMove(m_PiecePosition, new_piece_position, board))
            return false;
    }
    return true;
}
#pragma endregion

#pragma region Child classes
Chess_Game::Queen::Queen(BoardPosition start_position, const std::shared_ptr< Drawable>& drawable):
    ChessPiece(start_position, drawable)
{
    this->m_PieceMovementRules.push_back(std::make_unique<RookMovementRule>());
    this->m_PieceMovementRules.push_back(std::make_unique<BishopMovementRule>());

    this->m_PieceBoardSpecificMovementRule.push_back(std::make_unique<BlockableBoardSpecificMovementRule>());
}

Chess_Game::Bishop::Bishop(BoardPosition start_position, const std::shared_ptr< Drawable>& drawable):
    ChessPiece(start_position, drawable)
{
    this->m_PieceMovementRules.push_back(std::make_unique<BishopMovementRule>());
    this->m_PieceBoardSpecificMovementRule.push_back(std::make_unique<BlockableBoardSpecificMovementRule>());

}

Chess_Game::Pawn::Pawn(BoardPosition start_position, const std::shared_ptr< Drawable>& drawable):
    ChessPiece(start_position, drawable)
{
    this->m_PieceMovementRules.push_back(std::make_unique<SingleForwardMovementRule>());
    this->m_PieceMovementRules.push_back(std::make_unique<PawnSidewayCaptureRule>());
    this->m_PieceMovementRules.push_back(std::make_unique<PawnStartingMovementRule>());
    this->m_PieceBoardSpecificMovementRule.push_back(std::make_unique<BlockableBoardSpecificMovementRule>());
    this->m_PieceBoardSpecificMovementRule.push_back(std::make_unique<PawnTeamMovementRule>());
    this->m_PieceBoardSpecificMovementRule.push_back(std::make_unique<PawnCaptureBoardSpecificMovementRule>());

}

void Chess_Game::Pawn::SetPiecePosition(BoardPosition new_position)
{
    ChessPiece::SetPiecePosition(new_position);

    if (m_IsFirstMove)
    {
        this->m_PieceMovementRules.pop_back();
        m_IsFirstMove = false;
    }
}


Chess_Game::Knight::Knight(BoardPosition start_position, const std::shared_ptr< Drawable>& drawable):
    ChessPiece(start_position, drawable)
{
    this->m_PieceMovementRules.push_back(std::make_unique<KnightMovementRule>());

}

Chess_Game::Rook::Rook(BoardPosition start_position, const std::shared_ptr< Drawable>& drawable):
    ChessPiece(start_position, drawable)
{
    this->m_PieceMovementRules.push_back(std::make_unique<RookMovementRule>());
    this->m_PieceBoardSpecificMovementRule.push_back(std::make_unique<BlockableBoardSpecificMovementRule>());

}

Chess_Game::King::King(BoardPosition start_position, const std::shared_ptr< Drawable>& drawable):
    ChessPiece(start_position, drawable)
{
    this->m_PieceMovementRules.push_back(std::make_unique<KingMovement>());
    this->m_PieceBoardSpecificMovementRule.push_back(std::make_unique<BlockableBoardSpecificMovementRule>());
}
#pragma endregion
