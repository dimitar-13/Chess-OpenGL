#pragma once
#include "Core/Chess_pch.h"
#include "BaseChessPiece.h"

namespace Chess_Game
{
   //struct CheckData
   //{
   //    std::weak_ptr<ChessPiece> piece_delivering_the_check{};
   //    BoardPosition check_direction{};
   //};

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
        void SetSelectedPiece(std::shared_ptr<ChessPiece> new_piece);
        void UnSelectPiece() {m_SelectedPiece = std::weak_ptr<ChessPiece>();}
        void RemovePiece(BoardPosition piece_board_position);
        void MoveSelectedPiece(BoardPosition new_piece_position);
        const std::vector<std::shared_ptr<ChessPiece>>& GetPlayerPieces() { return m_PlayerPieces; }
        void SetPlayerKingCheckData(const KingCheckData& data) { m_PlayerKingCheckData = data; }
        const KingCheckData& GetPlayerKingCheckData() { return m_PlayerKingCheckData; }

        const King& GetPlayerKing() { return *m_PlayerKing; }
        std::weak_ptr<ChessPiece> GetSelectedPiece() { return m_SelectedPiece; }
        void IncreaseScore(size_t amount) { m_PlayerScore += amount; }
        bool CanSelectedPieceMove(BoardPosition new_position, ChessBoard& chess_board);
    private:
        OptionalIndex GetPieceArrayIndex(BoardPosition piece_board_position)const;
    private:
        std::vector<std::shared_ptr<ChessPiece>> m_PlayerPieces;
        std::shared_ptr<King> m_PlayerKing;
        std::weak_ptr<ChessPiece> m_SelectedPiece;
        size_t m_PlayerScore{};
        KingCheckData m_PlayerKingCheckData{};
    };
}