#include "ChessBoard.h"
#include "Logging/Logger.h"
#include "ChessGame.h"

Chess_Game::ChessGame::ChessGame(const std::shared_ptr<ChessPlayer>& white_player,
    const std::shared_ptr<ChessPlayer>& black_player) :
    m_WhiteTeamPlayer(white_player), m_BlackTeamPlayer(black_player)
{

    SetupChessBoardBitMask();

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

        m_IsSelectedPieceChanged = true;
    }
    return;
}

bool Chess_Game::ChessGame::IsPieceSelected()
{
    if (auto current_player = m_CurrentPlayer.lock())
    {
        return !current_player->GetSelectedPiece().expired();
    }
    return false;
}

std::weak_ptr<Chess_Game::ChessPiece> Chess_Game::ChessGame::GetSelectedPiece()
{
    if (auto current_player = m_CurrentPlayer.lock())
    {
        return current_player->GetSelectedPiece();
    }
    return std::weak_ptr<ChessPiece>();
}

bool Chess_Game::ChessGame::CanMove(BoardPosition new_position, std::shared_ptr<ChessPlayer> player_to_check)
{  
    if (!ChessBoard::IsNewPositionInBounds(new_position))
    {
        return false;
    }

    if (!player_to_check->CanSelectedPieceMove(new_position, m_ChessBoardData))
    {
        return false;
    }
    
    if (m_ChessBoardData.GetChessboardPositionFlag(new_position) & BoardPositionFlags_kIsPieceImortal)
    {
        CHESS_LOG_INFO("You can't capture the king.");
        return false;
    }
    
    if (!CanResolveCheck(new_position, player_to_check))
    {
        CHESS_LOG_INFO("The next move must resolve the kings check.");
        return false;
    }
    
    if (!IsKingSafeAfterMove(new_position, player_to_check))
    {
        CHESS_LOG_INFO("The piece cannot move in this position because the next position will result in a mate.");
        return false;
    }

    CHESS_LOG_INFO("Movement is valid.");
    
    return true;
    
}

bool Chess_Game::ChessGame::CanMoveSelectedPiece(BoardPosition new_position)
{
    if (auto current_player = m_CurrentPlayer.lock())
    {     
        if (!current_player->GetSelectedPiece().expired() && 
            current_player->GetSelectedPiece().lock()->GetPiecePosition() == new_position)
        {
            CHESS_LOG_INFO("Piece was unselected.");
            current_player->UnSelectPiece();
            m_IsSelectedPieceChanged = true;

            return false;
        }     
        return CanMove(new_position, current_player);
    }
    return false;
}

void Chess_Game::ChessGame::MoveSelectedPiece(BoardPosition new_position)
{
    if (m_CurrentPlayer.expired())
        return;

    auto current_player = m_CurrentPlayer.lock();

    if (current_player->GetSelectedPiece().expired())
        return;
        
    auto selected_piece = current_player->GetSelectedPiece().lock();

    BoardPosition current_piece_position = selected_piece->GetPiecePosition();

    BoardPositionFlags_ new_position_flag = BoardPositionFlags_kIsPositionOcupied;

    BoardPositionFlags_ current_position_flag_piece_team = 
        m_ChessBoardData.GetChessboardPositionFlag(current_piece_position)
        & BoardPositionFlags_kIsPieceFromWhiteTeam ?
        BoardPositionFlags_kIsPieceFromWhiteTeam : BoardPositionFlags_kIsPieceFromBlackTeam;

    ChessPieceType_ selected_piece_type = selected_piece->GetChessPieceType();
 
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
        m_IsGameOver = IsKingCheckMated();
    }

    current_player->UnSelectPiece();
    m_IsSelectedPieceChanged = true;
    m_CurrentPlayer = GetNonActivePlayer();
    
}

bool Chess_Game::ChessGame::IsSelectedPieceChanged()
{
    bool result = m_IsSelectedPieceChanged;
    if (m_IsSelectedPieceChanged)
        m_IsSelectedPieceChanged = false;
    return result;
}

std::vector<Chess_Game::BoardPosition> Chess_Game::ChessGame::GetSelectedPieceAllPossibleMoves()
{
    if (m_CurrentPlayer.expired())
        return std::vector<BoardPosition>();

    auto current_player = m_CurrentPlayer.lock();

    if (auto selected_piece = current_player->GetSelectedPiece().lock())
    {
        std::vector<BoardPosition> result{};

        size_t vertical{}, horizontal{};
        for (vertical = 1; vertical <= 8; vertical++)
        {
            for (horizontal = 'a'; horizontal <= 'h'; horizontal++)
            {
                BoardPosition position_to_check = {static_cast<char>(horizontal) ,static_cast<char>(vertical) };
                if (CanMove(position_to_check,current_player))
                {
                    result.push_back(position_to_check);
                }
            }
        }
        return result;

    }
    return std::vector<BoardPosition>();
}

bool Chess_Game::ChessGame::IsKingSafeAfterMove(BoardPosition new_position,
    std::shared_ptr<ChessPlayer> current_player)
{
  
    if (auto selected_piece = current_player->GetSelectedPiece().lock())
    {
        BoardPosition kings_position = selected_piece->GetChessPieceType() == ChessPieceType_kKing ?
            new_position : current_player->GetPlayerKing().GetPiecePosition();

        BoardPosition current_piece_position = selected_piece->GetPiecePosition();
        BoardPositionFlags_ current_position_flags =
            m_ChessBoardData.GetChessboardPositionFlag(current_piece_position);
        BoardPositionFlags_ new_position_flags =
            m_ChessBoardData.GetChessboardPositionFlag(new_position);
        bool result = true;

        m_ChessBoardData.SetChessboardPositionFlag(current_piece_position, static_cast<BoardPositionFlags_>(0));
        m_ChessBoardData.SetChessboardPositionFlag(new_position, static_cast<BoardPositionFlags_>(BoardPositionFlags_kIsPositionOcupied));

        for (const auto& piece : GetNonActivePlayer()->GetPlayerPieces())
        {
            if (piece->GetPiecePosition() == new_position &&
                !(m_ChessBoardData.GetChessboardPositionFlag(new_position) & BoardPositionFlags_kIsPieceImortal))
            {
                continue;
            }

            if (piece->CanMove(kings_position) && piece->CanMoveBoardSpecific(kings_position, m_ChessBoardData))
            {
                result = false;
                break;
            }
        }
        m_ChessBoardData.SetChessboardPositionFlag(current_piece_position, current_position_flags);
        m_ChessBoardData.SetChessboardPositionFlag(new_position, new_position_flags);

        return result;
    }
    return false;

    
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

void Chess_Game::ChessGame::SetupChessBoardBitMask()
{
    BoardPositionFlags_ white_team_flags = static_cast<BoardPositionFlags_>(
        BoardPositionFlags_kIsPositionOcupied | BoardPositionFlags_kIsPieceFromWhiteTeam);
    BoardPositionFlags_ black_team_flags = static_cast<BoardPositionFlags_>(
        BoardPositionFlags_kIsPositionOcupied | BoardPositionFlags_kIsPieceFromBlackTeam);


    for (const auto& piece : m_WhiteTeamPlayer->GetPlayerPieces())
    {
        m_ChessBoardData.SetChessboardPositionFlag(piece->GetPiecePosition(), white_team_flags);
    }
    for (const auto& piece : m_BlackTeamPlayer->GetPlayerPieces())
    {
        m_ChessBoardData.SetChessboardPositionFlag(piece->GetPiecePosition(), black_team_flags);
    }

    m_ChessBoardData.SetChessboardPositionFlag(m_WhiteTeamPlayer->GetPlayerKing().GetPiecePosition(), 
        static_cast<BoardPositionFlags_>(BoardPositionFlags_kIsPieceImortal | white_team_flags));
    m_ChessBoardData.SetChessboardPositionFlag(m_BlackTeamPlayer->GetPlayerKing().GetPiecePosition(),
        static_cast<BoardPositionFlags_>(BoardPositionFlags_kIsPieceImortal | black_team_flags));

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
                CHESS_LOG_INFO("The opposite team king has been checked.");
                BoardPosition piece_position =  piece->GetPiecePosition();
         
                check_data.is_king_checked = true;
                check_data.pieces_delivering_check.emplace_back(piece);
            }
        }
        if(check_data.is_king_checked)
            non_active_player->SetPlayerKingCheckData(check_data);

        return check_data.is_king_checked;
    }
    return false;
}

bool Chess_Game::ChessGame::IsKingCheckMated()
{
    CHESS_LOG_INFO("Checking for checkmate.");

    if (auto current_player = m_CurrentPlayer.lock())
    {
        auto player_to_check = GetNonActivePlayer();

        // - Move the king
        if (CanKingResolveTheCheck(player_to_check))
        {
            return false;
        }

        //- Move piece to prevent check.
        //- Claim the piece causing the check
        if (CanOtherPiecesResolveTheCheck(player_to_check))
        {
            return false;
        }

        CHESS_LOG_INFO("King has been checkmated.");
    }
    return true;
}

bool Chess_Game::ChessGame::CanKingResolveTheCheck(std::shared_ptr<ChessPlayer> player_to_check)
{
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

    bool result = false;

    BoardPosition king_board_position = player_to_check->GetPlayerKing().GetPiecePosition();
    BoardPositionFlags_ king_board_flags = m_ChessBoardData.GetChessboardPositionFlag(king_board_position);

    player_to_check->SelectPiece(king_board_position);

    for (const auto offset : kKingMovementOffsets)
    {
        BoardPosition new_position = king_board_position;

        new_position.VerticalPosition += offset.VerticalPosition;
        new_position.horizontalPosition += offset.horizontalPosition;

        if (!ChessBoard::IsNewPositionInBounds(new_position))
        {
            continue;
        }

        BoardPositionFlags_ previous_flags = m_ChessBoardData.GetChessboardPositionFlag(new_position);
        m_ChessBoardData.SetChessboardPositionFlag(new_position, king_board_flags);

        result = this->CanMove(new_position, player_to_check);
       
        m_ChessBoardData.SetChessboardPositionFlag(new_position, previous_flags);
        player_to_check->UnSelectPiece();

        if (result)
        {
            return result;
        }
    }
    return result;
}

bool Chess_Game::ChessGame::CanOtherPiecesResolveTheCheck(std::shared_ptr<ChessPlayer> player_to_check)
{
    bool result = false;
    const KingCheckData& player_check_info = player_to_check->GetPlayerKingCheckData();
    std::vector<BoardPosition> check_line_points{};
    BoardPosition king_board_position = player_to_check->GetPlayerKing().GetPiecePosition();

    for (const auto& piece_delivering_check_info : player_check_info.pieces_delivering_check)
    {
        BoardPosition start_pos = piece_delivering_check_info->GetPiecePosition();

        if (piece_delivering_check_info->GetChessPieceType() == ChessPieceType_kKnight)
        {
            check_line_points.push_back(start_pos);
            continue;
        }

        BoardPosition direction = king_board_position - start_pos;

        size_t length = std::max(abs(direction.VerticalPosition),abs(direction.horizontalPosition));

        direction = BoardPosition::Normalize(direction);

        for (size_t i = 0; i < length; i++)
        {
            BoardPosition new_position{};
            new_position.VerticalPosition = start_pos.VerticalPosition + (direction.VerticalPosition * i);
            new_position.horizontalPosition = start_pos.horizontalPosition + (direction.horizontalPosition * i);

            check_line_points.push_back(new_position);
        }
    }

    for (const auto position_to_check : check_line_points)
    {
        for (const auto& piece_to_check : player_to_check->GetPlayerPieces())
        {          
            player_to_check->SelectPiece(piece_to_check->GetPiecePosition());

            result = this->CanMove(position_to_check, player_to_check);

            if (result)
            {
                player_to_check->UnSelectPiece();
                return result;
            }
        }
    }

    return result;
}

bool Chess_Game::ChessGame::CanResolveCheck(BoardPosition new_position,
    std::shared_ptr<ChessPlayer> current_player)
{
    if (auto selected_piece = current_player->GetSelectedPiece().lock())
    {
     
        const KingCheckData& check_data = current_player->GetPlayerKingCheckData();

        if (!check_data.is_king_checked)
            return true;

        BoardPositionFlags_ previous_position_flags = m_ChessBoardData.GetChessboardPositionFlag(new_position);
        BoardPositionFlags_ selected_piece_flag = 
            m_ChessBoardData.GetChessboardPositionFlag(selected_piece->GetPiecePosition());
         
        BoardPosition king_position = selected_piece->GetChessPieceType() == ChessPieceType_kKing ?
            new_position : current_player->GetPlayerKing().GetPiecePosition();

        bool result = true;
        m_ChessBoardData.SetChessboardPositionFlag(new_position, selected_piece_flag);

        for (const auto& piece_delivering_the_check : check_data.pieces_delivering_check)
        {
            if (new_position == piece_delivering_the_check->GetPiecePosition())
            {
                continue;
            }


            if (piece_delivering_the_check->CanMove(king_position) && 
                piece_delivering_the_check->CanMoveBoardSpecific(king_position, m_ChessBoardData))
            {
                result = false;
                break;
            }
        }
        m_ChessBoardData.SetChessboardPositionFlag(new_position, previous_position_flags);
        return result;
    }
}
