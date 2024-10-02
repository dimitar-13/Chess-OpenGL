#pragma once
#include "BaseChessPiece.h"
#include "ChessPlayer.h"
#include "Core/Chess_pch.h"
#include <future>
namespace Chess_Game
{
    class ChessGame {
    public:
        ChessGame(const std::shared_ptr<ChessPlayer>& white_player,
            const std::shared_ptr<ChessPlayer>& black_player,
            const std::function<std::shared_ptr<ChessPiece>(bool, BoardPosition)>& pawn_promotion_callback);
        void MoveCurrentPlayerSelectedPiece(BoardPosition new_position);
        bool CanMoveSelectedPiece(BoardPosition new_position){ return CanMove(new_position,m_ActivePlayer,m_NonActivePlayer);}
        std::vector<BoardPosition> GetSelectedPieceAllPossibleMoves(std::shared_ptr<ChessPlayer> current_player,
            std::shared_ptr<ChessPlayer> opposite_player);
        void UpdateChessGamePlayers(const std::shared_ptr<ChessPlayer>& active_player,
            const std::shared_ptr<ChessPlayer>& opposite_player);
        bool IsGameOver()const {return m_IsKingCheckMated; }
        std::vector<std::shared_ptr<ChessPiece>> GetAllChessboardPieces();
    private:     
        bool CanResolveCheck(BoardPosition new_position);
        bool IsKingChecked();
        bool IsKingCheckMated();
        void CapturePiece(BoardPosition position_of_piece_to_capture);
        bool CanKingResolveTheCheck(std::shared_ptr<ChessPlayer> player_to_check,
            std::shared_ptr<ChessPlayer> opposite_player);
        bool CanOtherPiecesResolveTheCheck(std::shared_ptr<ChessPlayer> player_to_check,
            std::shared_ptr<ChessPlayer> opposite_player);
        void SetupChessBoardBitMask(std::shared_ptr<ChessPlayer> white_team_player,
            std::shared_ptr<ChessPlayer> black_team_player);
        bool CanMove(BoardPosition new_position, std::shared_ptr<ChessPlayer> player_to_check,
            std::shared_ptr<ChessPlayer> opposite_player);
        bool IsKingSafeAfterMove(BoardPosition new_position, std::shared_ptr<ChessPlayer> player_to_check,
            std::shared_ptr<ChessPlayer> opposite_player);
    private:
        ChessBoard m_ChessBoardData{};
        std::shared_ptr<ChessPlayer> m_ActivePlayer{};
        std::shared_ptr<ChessPlayer> m_NonActivePlayer{};
        bool m_IsKingCheckMated = false;
        std::function<std::shared_ptr<ChessPiece>(bool, BoardPosition)> m_OnPawnPromotion{};
    };
}