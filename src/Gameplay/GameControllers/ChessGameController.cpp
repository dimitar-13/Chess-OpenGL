#include "D:/c++/OpenGl/Chess-OpenGL/build/CMakeFiles/Chess.dir/Debug/cmake_pch.hxx"
#include "ChessGameController.h"

void Chess_Game::ChessGameController::ProcessInput(BoardPosition new_position)
{
    if (!this->IsPieceSelected())
    {
        this->SelectPiece(new_position);
        m_OnSelectedPieceChanged(m_PlayerCurrentController.GetActivePlayer()->GetSelectedPiece());
        return;
    }

    if (this->GetSelectedPiece().lock()->GetPiecePosition() == new_position)
    {
        this->UnselectPiece();
        m_OnSelectedPieceChanged(m_PlayerCurrentController.GetActivePlayer()->GetSelectedPiece());

        return;
    }

    if (CanMoveSelectedPiece(new_position))
    {
        MoveSelectedPiece(new_position);
        this->UnselectPiece();
        m_PlayerCurrentController.SwitchPlayer();

        m_ChessGame->UpdateChessGamePlayers(
            m_PlayerCurrentController.GetActivePlayer(),
            m_PlayerCurrentController.GetNotActivePlayer());

        m_OnSelectedPieceChanged(m_PlayerCurrentController.GetActivePlayer()->GetSelectedPiece());
    }

}

void Chess_Game::ChessGameController::SelectPiece(BoardPosition piece_board_position)
{
    m_PlayerCurrentController.GetActivePlayer()->SelectPiece(piece_board_position);
}

void Chess_Game::ChessGameController::UnselectPiece()
{
    m_PlayerCurrentController.GetActivePlayer()->UnSelectPiece();
}

bool Chess_Game::ChessGameController::IsPieceSelected()
{
    return !m_PlayerCurrentController.GetActivePlayer()->GetSelectedPiece().expired();
}

bool Chess_Game::ChessGameController::CanMoveSelectedPiece(BoardPosition new_position)
{
    return m_ChessGame->CanMoveSelectedPiece(new_position);
}

void Chess_Game::ChessGameController::MoveSelectedPiece(BoardPosition new_position)
{
    m_ChessGame->MoveCurrentPlayerSelectedPiece(new_position);

    m_OnSelectedPieceMoved(m_PlayerCurrentController.GetActivePlayer()->GetSelectedPiece().lock());
}
