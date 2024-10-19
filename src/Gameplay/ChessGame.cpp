#include "ChessBoard.h"
#include "Logging/Logger.h"
#include "ChessGame.h"
#include <thread>

Chess_Game::ChessGame::ChessGame(const std::shared_ptr<ChessPlayer>& white_player,
    const std::shared_ptr<ChessPlayer>& black_player,
    const std::function<std::shared_ptr<ChessPiece>(bool, BoardPosition)>& pawn_promotion_callback):
    m_ActivePlayer(white_player),m_NonActivePlayer(black_player),
    m_OnPawnPromotion(pawn_promotion_callback)
{
    SetupChessBoardBitMask(white_player, black_player);
}

bool Chess_Game::ChessGame::CanMove(BoardPosition new_position, std::shared_ptr<ChessPlayer> player_to_check,
    std::shared_ptr<ChessPlayer> opposite_player)
{  
    if (!ChessBoard::IsNewPositionInBounds(new_position))
    {
        return false;
    }

    if (m_ChessBoardData.GetChessboardPositionFlag(new_position) & BoardPositionFlags_kIsPieceImmortal)
    {
        CHESS_LOG_INFO("You can't capture the king.");
        return false;
    }

    if (!player_to_check->CanSelectedPieceMove(new_position, m_ChessBoardData))
    {
        return false;
    }
 
    if (!CanResolveCheck(new_position))
    {
        CHESS_LOG_INFO("The next move must resolve the kings check.");
        return false;
    }
    
    if (!IsKingSafeAfterMove(new_position, player_to_check, opposite_player))
    {
        CHESS_LOG_INFO("The piece cannot move in this position because the next position will result in a mate.");
        return false;
    }

    CHESS_LOG_INFO("Movement is valid.");
    
    return true;
    
}


void Chess_Game::ChessGame::MoveCurrentPlayerSelectedPiece(BoardPosition new_position)
{
    if (auto selected_piece = m_ActivePlayer->GetSelectedPiece().lock())
    {

        BoardPosition current_piece_position = selected_piece->GetPiecePosition();

        BoardPositionFlags_ new_position_flag = BoardPositionFlags_kIsPositionOccupied;

        BoardPositionFlags_ current_position_flag_piece_team =
            m_ChessBoardData.GetChessboardPositionFlag(current_piece_position)
            & BoardPositionFlags_kIsPieceFromWhiteTeam ?
            BoardPositionFlags_kIsPieceFromWhiteTeam : BoardPositionFlags_kIsPieceFromBlackTeam;

        ChessPieceType_ selected_piece_type = selected_piece->GetChessPieceType();

        if (m_ChessBoardData.GetChessboardPositionFlag(new_position) & BoardPositionFlags_kIsPositionOccupied)
        {
            CapturePiece(new_position);
        }

        m_ActivePlayer->MoveSelectedPiece(new_position);

        new_position_flag = selected_piece_type == ChessPieceType_kKing ?
            static_cast<BoardPositionFlags_>(new_position_flag | BoardPositionFlags_kIsPieceImmortal | current_position_flag_piece_team)
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
            // 

            bool is_white_team = 
                current_position_flag_piece_team & BoardPositionFlags_kIsPieceFromWhiteTeam;

            m_ActivePlayer->SetSelectedPiece(m_OnPawnPromotion(is_white_team, new_position));
        }

        if (IsKingChecked())
        {
            m_IsKingCheckMated = IsKingCheckMated();
        }
    }
}
std::vector<Chess_Game::BoardPosition> Chess_Game::ChessGame::GetSelectedPieceAllPossibleMoves(
    std::shared_ptr<ChessPlayer> current_player,
    std::shared_ptr<ChessPlayer> opposite_player)
{
    std::vector<BoardPosition> result{};
  if (auto selected_piece = current_player->GetSelectedPiece().lock())
  {
      size_t vertical{}, horizontal{};
      for (vertical = 1; vertical <= 8; vertical++)
      {
          for (horizontal = 'a'; horizontal <= 'h'; horizontal++)
          {
              BoardPosition position_to_check = {static_cast<char>(horizontal) ,static_cast<char>(vertical) };

              if (CanMove(position_to_check,current_player, opposite_player))
              {
                  result.push_back(position_to_check);
              }
          }
      }
      return result;
  }
}

void Chess_Game::ChessGame::UpdateChessGamePlayers(
    const std::shared_ptr<ChessPlayer>& active_player,
    const std::shared_ptr<ChessPlayer>& opposite_player)
{
    m_ActivePlayer = active_player;
    m_NonActivePlayer = opposite_player;
}

std::vector<std::shared_ptr<Chess_Game::ChessPiece>> Chess_Game::ChessGame::GetAllChessboardPieces()
{
    std::vector<std::shared_ptr<ChessPiece>> player_1_pieces = m_ActivePlayer->GetPlayerPieces();
    const std::vector<std::shared_ptr<ChessPiece>>& player_2_pieces = m_NonActivePlayer->GetPlayerPieces();
    player_1_pieces.insert(player_1_pieces.end(), player_2_pieces.begin(), player_2_pieces.end());
    return player_1_pieces;
}

bool Chess_Game::ChessGame::IsKingSafeAfterMove(BoardPosition new_position,
    std::shared_ptr<ChessPlayer> player_to_check,
    std::shared_ptr<ChessPlayer> opposite_player)
{
  
    if (auto selected_piece = player_to_check->GetSelectedPiece().lock())
    {
        BoardPosition kings_position = selected_piece->GetChessPieceType() == ChessPieceType_kKing ?
            new_position : player_to_check->GetPlayerKing().GetPiecePosition();

        BoardPosition current_piece_position = selected_piece->GetPiecePosition();
        BoardPositionFlags_ current_position_flags =
            m_ChessBoardData.GetChessboardPositionFlag(current_piece_position);
        BoardPositionFlags_ new_position_flags =
            m_ChessBoardData.GetChessboardPositionFlag(new_position);

        bool result = true;

        m_ChessBoardData.SetChessboardPositionFlag(current_piece_position, static_cast<BoardPositionFlags_>(0));
        m_ChessBoardData.SetChessboardPositionFlag(new_position, current_position_flags);

        for (const auto& piece : opposite_player->GetPlayerPieces())
        {
            if (piece->GetPiecePosition() == new_position &&
                !(m_ChessBoardData.GetChessboardPositionFlag(new_position) & BoardPositionFlags_kIsPieceImmortal))
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
    m_ActivePlayer->IncreaseScore(1);
    m_NonActivePlayer->RemovePiece(position_of_piece_to_capture);
    CHESS_LOG_INFO("Captured.");
}


void Chess_Game::ChessGame::SetupChessBoardBitMask(std::shared_ptr<ChessPlayer> white_team_player,
    std::shared_ptr<ChessPlayer> black_team_player)
{
    BoardPositionFlags_ white_team_flags = static_cast<BoardPositionFlags_>(
        BoardPositionFlags_kIsPositionOccupied | BoardPositionFlags_kIsPieceFromWhiteTeam);
    BoardPositionFlags_ black_team_flags = static_cast<BoardPositionFlags_>(
        BoardPositionFlags_kIsPositionOccupied | BoardPositionFlags_kIsPieceFromBlackTeam);


    for (const auto& piece : white_team_player->GetPlayerPieces())
    {
        m_ChessBoardData.SetChessboardPositionFlag(piece->GetPiecePosition(), white_team_flags);
    }
    for (const auto& piece : black_team_player->GetPlayerPieces())
    {
        m_ChessBoardData.SetChessboardPositionFlag(piece->GetPiecePosition(), black_team_flags);
    }

    m_ChessBoardData.SetChessboardPositionFlag(white_team_player->GetPlayerKing().GetPiecePosition(),
        static_cast<BoardPositionFlags_>(BoardPositionFlags_kIsPieceImmortal | white_team_flags));
    m_ChessBoardData.SetChessboardPositionFlag(black_team_player->GetPlayerKing().GetPiecePosition(),
        static_cast<BoardPositionFlags_>(BoardPositionFlags_kIsPieceImmortal | black_team_flags));

}

bool Chess_Game::ChessGame::IsKingChecked()
{    
    BoardPosition king_to_check_board_position = m_NonActivePlayer->GetPlayerKing().GetPiecePosition();
    KingCheckData check_data{};
    
    
    for (const auto& piece : m_ActivePlayer->GetPlayerPieces())
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
        m_NonActivePlayer->SetPlayerKingCheckData(check_data);
    
    return check_data.is_king_checked;
    
}

bool Chess_Game::ChessGame::IsKingCheckMated()
{
    CHESS_LOG_INFO("Checking for checkmate.");
   
    // - Move the king
    if (CanKingResolveTheCheck(m_NonActivePlayer, m_ActivePlayer))
    {
        return false;
    }
    
    //- Move piece to prevent check.
    //- Claim the piece causing the check
    if (CanOtherPiecesResolveTheCheck(m_NonActivePlayer, m_ActivePlayer))
    {
        return false;
    }
    
    CHESS_LOG_INFO("King has been checkmated.");

    return true;
}

bool Chess_Game::ChessGame::CanKingResolveTheCheck(std::shared_ptr<ChessPlayer> player_to_check,
    std::shared_ptr<ChessPlayer> opposite_player)
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

        result = this->CanMove(new_position, player_to_check, opposite_player);
       
        if (result)
            break;
    }

    player_to_check->UnSelectPiece();
    return result;
}

bool Chess_Game::ChessGame::CanOtherPiecesResolveTheCheck(std::shared_ptr<ChessPlayer> player_to_check,
    std::shared_ptr<ChessPlayer> opposite_player)
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

            result = this->CanMove(position_to_check, player_to_check, opposite_player);

            if (result)
            {
                player_to_check->UnSelectPiece();
                return result;
            }
        }
    }

    return result;
}

bool Chess_Game::ChessGame::CanResolveCheck(BoardPosition new_position)
{
    if (auto selected_piece = m_ActivePlayer->GetSelectedPiece().lock())
    {
     
        const KingCheckData& check_data = m_ActivePlayer->GetPlayerKingCheckData();

        if (!check_data.is_king_checked)
            return true;

        BoardPositionFlags_ previous_position_flags = m_ChessBoardData.GetChessboardPositionFlag(new_position);
        BoardPositionFlags_ selected_piece_flag = 
            m_ChessBoardData.GetChessboardPositionFlag(selected_piece->GetPiecePosition());
         
        BoardPosition king_position = selected_piece->GetChessPieceType() == ChessPieceType_kKing ?
            new_position : m_ActivePlayer->GetPlayerKing().GetPiecePosition();

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
