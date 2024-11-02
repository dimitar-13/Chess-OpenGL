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

void Chess_Game::ChessPlayer::SetSelectedPiece(std::shared_ptr<ChessPiece> new_piece)
{
    if (auto selected_piece = this->m_SelectedPiece.lock())
    {

        OptionalIndex selected_piece_index = 
            GetPieceArrayIndex(selected_piece->GetPiecePosition());

        if (!selected_piece_index.is_set)
        {
            return;
        }
        m_PlayerPieces[selected_piece_index.index_value] = new_piece;
        this->m_SelectedPiece = new_piece;
    }
}

void Chess_Game::ChessPlayer::RemovePiece(BoardPosition piece_board_position)
{
    OptionalIndex piece_to_remove_index = GetPieceArrayIndex(piece_board_position);
    if(piece_to_remove_index.is_set)
        m_PlayerPieces.erase(m_PlayerPieces.begin() + piece_to_remove_index.index_value);
}

void Chess_Game::ChessPlayer::MoveSelectedPiece(BoardPosition new_piece_position)
{
    if (auto selected_piece = m_SelectedPiece.lock())
    {
        selected_piece->SetPiecePosition(new_piece_position);

        if (m_PlayerKingCheckData.is_king_checked)
        {
            m_PlayerKingCheckData.is_king_checked = false;
            m_PlayerKingCheckData.pieces_delivering_check.resize(0);
        }
    }
}

bool Chess_Game::ChessPlayer::CanSelectedPieceMove(BoardPosition new_position, ChessBoard& chess_board)
{
    if (auto selected_piece = m_SelectedPiece.lock())
    {
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
        return true;
    }
    CHESS_LOG_INFO("Selected piece was empty.");
    return false;
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