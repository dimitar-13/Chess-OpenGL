#include "ChessBoard.h"
#include "Logging/Logger.h"
#include "ChessGame.h"

Chess_Game::ChessGame::ChessGame()
{

    //std::vector<std::shared_ptr<ChessPiece>> black_team_vector =
    //{ std::make_shared<King>(BoardPosition{ 'h', 8 }) };

    //m_BlackTeamPlayer = std::make_shared<ChessPlayer>(black_team_vector);

    //m_ChessBoardData.SetChessboardPositionFlag(BoardPosition{ 'd', 4 },
    //    static_cast<BoardPositionFlags_>(BoardPositionFlags_kIsPositionOcupied |
    //    BoardPositionFlags_kIsPieceFromBlackTeam | BoardPositionFlags_kIsPieceImortal));

    //m_ChessBoardData.SetChessboardPositionFlag(BoardPosition{ 'a', 1 },
    //    static_cast<BoardPositionFlags_>(BoardPositionFlags_kIsPositionOcupied | 
    //        BoardPositionFlags_kIsPieceFromWhiteTeam));

    //m_ChessBoardData.SetChessboardPositionFlag(BoardPosition{ 'c', 2 },
    //    static_cast<BoardPositionFlags_>(BoardPositionFlags_kIsPositionOcupied | 
    //        BoardPositionFlags_kIsPieceFromWhiteTeam));



    //std::vector<std::shared_ptr<ChessPiece>> white_team_vector = 
    //{ std::make_shared<Queen>(BoardPosition{ 'a', 1 }),
    //  std::make_shared<Rook>(BoardPosition{ 'h', 1 }),
    //  std::make_shared<Rook>(BoardPosition{ 'a', 2 }),
    //std::make_shared<King>(BoardPosition{ 'g', 3 }) };

    //m_WhiteTeamPlayer = std::make_shared<ChessPlayer>(white_team_vector);


    InitializeTeamPieces();

    m_CurrentPlayer = m_WhiteTeamPlayer;
}

void Chess_Game::ChessGame::SelectPiece(BoardPosition piece_board_position)
{
    if (!ChessBoard::IsNewPositionInBounds(piece_board_position))
    {
        CHESS_LOG_INFO("The selected position is not in bounds.");
        return;
    }
    if (auto current_player = m_CurrentPlayer.lock())
    {
        current_player->SelectPiece(piece_board_position);
    }
    return;
}

bool Chess_Game::ChessGame::CanMoveSelectedPiece(BoardPosition new_position)
{
    if (auto current_player = m_CurrentPlayer.lock())
    {
        if (!ChessBoard::IsNewPositionInBounds(new_position))
        {
            return false;
        }

        if (!current_player->CanSelectedPieceMove(new_position, m_ChessBoardData))
        {
            return false;
        }

        if (!IsKingSafeAfterMove(new_position))
        {
            CHESS_LOG_INFO("The piece cannot move in this position because the next position will result in a mate.");
            return false;
        }

        if (m_ChessBoardData.GetChessboardPositionFlag(new_position) & BoardPositionFlags_kIsPieceImortal)
        {
            CHESS_LOG_INFO("You can't capture the king.");
            return false;
        }

        CHESS_LOG_INFO("Movement is valid.");

        return true;
    }
    return false;

}

void Chess_Game::ChessGame::MoveSelectedPiece(BoardPosition new_position)
{
    if (auto current_player = m_CurrentPlayer.lock())
    {   
        BoardPosition current_piece_position = current_player->GetSelectedPiece().GetPiecePosition();

        BoardPositionFlags_ new_position_flag = BoardPositionFlags_kIsPositionOcupied;

        BoardPositionFlags_ current_position_flag_piece_team = 
            m_ChessBoardData.GetChessboardPositionFlag(current_piece_position)
            & BoardPositionFlags_kIsPieceFromWhiteTeam ?
            BoardPositionFlags_kIsPieceFromWhiteTeam : BoardPositionFlags_kIsPieceFromBlackTeam;

        ChessPieceType_ selected_piece_type = current_player->GetSelectedPiece().GetChessPieceType();
 
        if (m_ChessBoardData.GetChessboardPositionFlag(new_position) & BoardPositionFlags_kIsPositionOcupied)
        {
            CapturePiece(new_position);
        }

        current_player->MoveSelectedPiece(new_position);

        new_position_flag = selected_piece_type == ChessPieceType_kKing ?
            static_cast<BoardPositionFlags_>(new_position_flag | BoardPositionFlags_kIsPieceImortal | current_position_flag_piece_team)
            : static_cast<BoardPositionFlags_>(new_position_flag | current_position_flag_piece_team);

        m_ChessBoardData.SetChessboardPositionFlag(current_piece_position, static_cast<BoardPositionFlags_>(0));

        m_ChessBoardData.SetChessboardPositionFlag(new_position, new_position_flag);

        if (ChessBoard::IsPositionAtVerticalBoarder(new_position) && selected_piece_type == ChessPieceType_kPawn)
        {
            //Await this thread for the answer. 
            //Reason why this should be on separate thread is that we don't want to block the input polling and 
            // the rendering.

            //Issue a draw for a selection box for the promote piece type.
            //Wait for the answer in this thread and then proceed.

            current_player->PromotePawn(ChessPieceType_kQueen);
        }

        if (IsKingChecked())
        {
            IsKingCheckMated();
        }

        m_CurrentPlayer = GetNonActivePlayer();
    }
}

bool Chess_Game::ChessGame::IsKingSafeAfterMove(BoardPosition new_position)
{
    if (auto current_player = m_CurrentPlayer.lock())
    {
        BoardPosition kings_position = current_player->GetPlayerKing().GetPiecePosition();
        BoardPosition current_piece_position = current_player->GetSelectedPiece().GetPiecePosition();
        BoardPositionFlags_ previous_position_flags = 
            m_ChessBoardData.GetChessboardPositionFlag(current_piece_position);

        bool result = true;

        m_ChessBoardData.SetChessboardPositionFlag(current_piece_position, static_cast<BoardPositionFlags_>(0));
        for (const auto& piece : GetNonActivePlayer()->GetPlayerPieces())
        {
            if (piece->CanMove(kings_position) && piece->CanMoveBoardSpecific(kings_position, m_ChessBoardData))
            {
                result = false;
                break;
            }
        }
        m_ChessBoardData.SetChessboardPositionFlag(current_piece_position, previous_position_flags);
        return result;
    }
}

void Chess_Game::ChessGame::InitializeTeamPieces()
{
    std::vector<std::shared_ptr<ChessPiece>> white_team_vector ={
        std::make_shared<Rook>(BoardPosition{ 'a', 1 }),
        std::make_shared<Knight>(BoardPosition{ 'b', 1 }),
        std::make_shared<Bishop>(BoardPosition{ 'c', 1 }),
        std::make_shared<Queen>(BoardPosition{ 'd', 1 }),
        std::make_shared<King>(BoardPosition{ 'e', 1 }),
        std::make_shared<Bishop>(BoardPosition{ 'f', 1 }),
        std::make_shared<Knight>(BoardPosition{ 'g', 1 }),
        std::make_shared<Rook>(BoardPosition{ 'h', 1 })};
            
    // Pawns
    for (char file = 'a'; file <= 'h'; ++file) {
        white_team_vector.emplace_back(std::make_shared<Pawn>(BoardPosition{ file, 2 }));
    }

    m_WhiteTeamPlayer = std::make_shared<ChessPlayer>(white_team_vector);

    std::vector<std::shared_ptr<ChessPiece>> black_team_vector = {
        std::make_shared<Rook>(BoardPosition{ 'a', 8 }),
        std::make_shared<Knight>(BoardPosition{ 'b', 8 }),
        std::make_shared<Bishop>(BoardPosition{ 'c', 8 }),
        std::make_shared<Queen>(BoardPosition{ 'd', 8 }),
        std::make_shared<King>(BoardPosition{ 'e', 8 }),
        std::make_shared<Bishop>(BoardPosition{ 'f', 8 }),
        std::make_shared<Knight>(BoardPosition{ 'g', 8 }),
        std::make_shared<Rook>(BoardPosition{ 'h', 8 })};

    // Pawns
    for (char file = 'a'; file <= 'h'; ++file) {
        black_team_vector.emplace_back(std::make_shared<Pawn>(BoardPosition{ file, 7 }));
    }

    m_BlackTeamPlayer = std::make_shared<ChessPlayer>(black_team_vector);

    size_t current_index{};
    for(const auto& piece : white_team_vector)
    {
        m_ChessBoardData.SetChessboardPositionFlag(piece->GetPiecePosition(),static_cast<BoardPositionFlags_>(
            BoardPositionFlags_kIsPositionOcupied | BoardPositionFlags_kIsPieceFromWhiteTeam));
    }
    for (const auto& piece : black_team_vector)
    {
        m_ChessBoardData.SetChessboardPositionFlag(piece->GetPiecePosition(),static_cast<BoardPositionFlags_>(
            BoardPositionFlags_kIsPositionOcupied | BoardPositionFlags_kIsPieceFromBlackTeam));
    }

    m_ChessBoardData.SetChessboardPositionFlag({ 'e', 8 },BoardPositionFlags_kIsPieceImortal);
    m_ChessBoardData.SetChessboardPositionFlag({ 'e', 1 },BoardPositionFlags_kIsPieceImortal);
}

void Chess_Game::ChessGame::CapturePiece(BoardPosition position_of_piece_to_capture)
{
    if (auto current_player = m_CurrentPlayer.lock())
    {
        current_player->IncreaseScore(1);
        GetNonActivePlayer()->RemovePiece(position_of_piece_to_capture);

        CHESS_LOG_INFO("Captured.");
    }
}

std::shared_ptr<Chess_Game::ChessPlayer> Chess_Game::ChessGame::GetNonActivePlayer()
{
    if (auto current_active_player = m_CurrentPlayer.lock())
    {
        return current_active_player == m_WhiteTeamPlayer ? m_BlackTeamPlayer : m_WhiteTeamPlayer;
    }

    return std::shared_ptr<ChessPlayer>();
}

bool Chess_Game::ChessGame::IsKingChecked()
{
    if (auto current_player = m_CurrentPlayer.lock())
    {
        auto non_active_player = GetNonActivePlayer();

        BoardPosition king_to_check_board_position = non_active_player->GetPlayerKing().GetPiecePosition();
        KingCheckData check_data{};


        for (const auto& piece : current_player->GetPlayerPieces())
        {
            if (piece->CanMove(king_to_check_board_position) &&
                piece->CanMoveBoardSpecific(king_to_check_board_position, m_ChessBoardData))
            {
                CHESS_LOG_INFO("The opposite team king has been mated.");
                check_data.is_king_checked = true;
                check_data.pieces_delivering_check.push_back(piece);    
            }
        }
        if(check_data.is_king_checked)
            current_player->SetPlayerKingCheckData(check_data);

        return check_data.is_king_checked;
    }
    return false;
}

bool Chess_Game::ChessGame::IsKingCheckMated()
{
    bool result = false;

    if (auto current_player = m_CurrentPlayer.lock())
    {

        CHESS_LOG_INFO("Checking for checkmate.");

        auto opposite_player = GetNonActivePlayer();
        BoardPosition king_board_position = opposite_player->GetPlayerKing().GetPiecePosition();
        constexpr BoardPosition kKingMovementOffsets[] = {
            {1,1},
            { -1,1 },
            { 1,-1 },
            { -1,-1 },
            {0,1},
            {0,-1},
            {1,0},
            {1,0}
        };

        opposite_player->SelectPiece(king_board_position);
        m_CurrentPlayer = opposite_player;
        for (const auto offset: kKingMovementOffsets)
        {
            BoardPosition new_position = king_board_position;

            new_position.VerticalPosition += offset.VerticalPosition;
            new_position.horizontalPosition += offset.horizontalPosition;

            if (this->CanMoveSelectedPiece(new_position))
            {
                result = true;
                break;
            }

        }
        CHESS_LOG_INFO("King has been checkmated.");
        opposite_player->SelectPiece({ -1,-1 });
        m_CurrentPlayer = current_player;
    }
    
    return result;
}
