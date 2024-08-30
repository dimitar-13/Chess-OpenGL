#pragma once
#include<vector>
#include "BaseChessPiece.h"

namespace Chess_Game
{
    struct KingCheckData
    {
        bool is_king_checked = false;
        std::vector<std::shared_ptr<ChessPiece>> pieces_delivering_check{};
    };
    struct OptionalIndex
    {
        OptionalIndex() = default;
        OptionalIndex(size_t value):index_value(value), is_set(true)
        {}
        size_t index_value{};
        bool is_set = false;
    };

    class ChessPlayer
    {
    public:
        ChessPlayer(const std::vector<std::shared_ptr<ChessPiece>>& starting_pieces);
        void PromotePawn(ChessPieceType_ replace_piece_type);
        void SelectPiece(BoardPosition piece_board_position);
        void RemovePiece(BoardPosition piece_board_position);
        void MoveSelectedPiece(BoardPosition new_piece_position);
        const std::vector<std::shared_ptr<ChessPiece>>& GetPlayerPieces() { return m_PlayerPieces; }
        void SetPlayerKingCheckData(const KingCheckData& data) { m_PlayerKingCheckData = data; }

        const ChessPiece& GetPlayerKing() { return *m_PlayerKing; }
        const ChessPiece& GetSelectedPiece() { return *m_SelectedPiece.lock(); }
        void IncreaseScore(size_t amount) { m_PlayerScore += amount; }
        bool CanSelectedPieceMove(BoardPosition new_position, ChessBoard& chess_board);
    private:
        bool CanResolveCheck(BoardPosition new_position, ChessBoard& chess_board);
        OptionalIndex GetPieceArrayIndex(BoardPosition piece_board_position)const;
    private:
        std::vector<std::shared_ptr<ChessPiece>> m_PlayerPieces;
        std::shared_ptr<King> m_PlayerKing;
        std::weak_ptr<ChessPiece> m_SelectedPiece;
        size_t m_PlayerScore{};
        KingCheckData m_PlayerKingCheckData{};
    };
}