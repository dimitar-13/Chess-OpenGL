#include "ChessPlayer.h"
#include "Logging/Logger.h"
#include "ChessBoard.h"

Chess_Game::ChessPlayer::ChessPlayer(const std::vector<std::shared_ptr<ChessPiece>>& starting_pieces)
    :m_PlayerPieces(starting_pieces)
{
    for (const auto& piece : m_PlayerPieces)
    {
        if (piece->GetChessPieceType() == ChessPieceType_kKing)
            m_PlayerKing =std::dynamic_pointer_cast<King>(piece);
    }
}

void Chess_Game::ChessPlayer::PromotePawn(ChessPieceType_ replace_piece_type)
{
    ChessPiece* new_class = nullptr;
    auto ptr = m_SelectedPiece.lock();

    BoardPosition pawn_position = ptr->GetPiecePosition();
    OptionalIndex array_index = GetPieceArrayIndex(pawn_position);

    if (!array_index.is_set)
    {
        CHESS_LOG_WARN("Was no able to find the pawn to be promoted.");
        return;
    }

    switch (replace_piece_type)
    {
    case Chess_Game::ChessPieceType_kBishop:
        new_class = new Bishop(pawn_position);
        break;
    case Chess_Game::ChessPieceType_kRook:
        new_class = new Rook(pawn_position);
        break;
    case Chess_Game::ChessPieceType_kKnight:
        new_class = new Knight(pawn_position);
        break;
    case Chess_Game::ChessPieceType_kQueen:
        new_class = new Queen(pawn_position);
        break;
    default:
        CHESS_LOG_ERROR("Invalid enum to replace the pawn.");
        break;
    }

    m_PlayerPieces[array_index.index_value].reset(new_class);
}

void Chess_Game::ChessPlayer::SelectPiece(BoardPosition piece_board_position)
{
    OptionalIndex selected_piece_index = GetPieceArrayIndex(piece_board_position);
    if (!selected_piece_index.is_set)
    {
        m_SelectedPiece = std::weak_ptr<ChessPiece>();
        return;
    }

    m_SelectedPiece = m_PlayerPieces[selected_piece_index.index_value];
}

void Chess_Game::ChessPlayer::RemovePiece(BoardPosition piece_board_position)
{
    OptionalIndex piece_to_remove_index = GetPieceArrayIndex(piece_board_position);
    if(piece_to_remove_index.is_set)
        m_PlayerPieces.erase(m_PlayerPieces.begin() + piece_to_remove_index.index_value);
}

void Chess_Game::ChessPlayer::MoveSelectedPiece(BoardPosition new_piece_position)
{
    if(auto selected_piece = m_SelectedPiece.lock())
        selected_piece->SetPiecePosition(new_piece_position);
}

bool Chess_Game::ChessPlayer::CanSelectedPieceMove(BoardPosition new_position, ChessBoard& chess_board)
{
    if (auto selected_piece = m_SelectedPiece.lock())
    {
        if (selected_piece->GetPiecePosition() == new_position)
        {
            CHESS_LOG_INFO("Piece was unselected.");
            UnSelectPiece();
            return false;
        }

        if (!selected_piece->CanMove(new_position))
        {
            CHESS_LOG_INFO("New position does not obey piece rules.");
            return false;
        }
        if (!selected_piece->CanMoveBoardSpecific(new_position, chess_board))
        {
            CHESS_LOG_INFO("New position does not obey board piece rules.");
            return false;
        }
        if (chess_board.GetChessboardPositionFlag(new_position) & BoardPositionFlags_kIsPieceImortal)
        {
            CHESS_LOG_INFO("Can't capture the opposite team king.");
            return false;
        }

        if (m_PlayerKingCheckData.is_king_checked && !CanResolveCheck(new_position, chess_board))
        {
            CHESS_LOG_INFO("The next move must resolve the kings check.");
            return false;
        }
        m_PlayerKingCheckData.is_king_checked = false;
        m_PlayerKingCheckData.pieces_delivering_check.resize(0);

        return true;
    }
    return false;
}

bool Chess_Game::ChessPlayer::CanResolveCheck(BoardPosition new_position, ChessBoard& chess_board)
{
    BoardPositionFlags_ previous_position_flags = chess_board.GetChessboardPositionFlag(new_position);
    BoardPosition king_position = m_PlayerKing->GetPiecePosition();
    bool result = true;

    chess_board.SetChessboardPositionFlag(new_position, BoardPositionFlags_kIsPositionOcupied);

    for (const auto& piece_delivering_the_check : m_PlayerKingCheckData.pieces_delivering_check)
    {
        if (piece_delivering_the_check->CanMoveBoardSpecific(king_position, chess_board))
        {
            result = false;
            break;
        }
    }
    chess_board.SetChessboardPositionFlag(new_position, previous_position_flags);
    return result;
}

Chess_Game::OptionalIndex Chess_Game::ChessPlayer::GetPieceArrayIndex(BoardPosition piece_position)const
{
    for (size_t i = 0; i < m_PlayerPieces.size(); i++)
    {
        if (m_PlayerPieces[i]->GetPiecePosition() == piece_position)
        {
            return i;
        }
    }
    return OptionalIndex();
}