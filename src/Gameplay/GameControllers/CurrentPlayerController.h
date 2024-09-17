#pragma once
#include "Gameplay/BaseChessPiece.h"
#include "Gameplay/ChessPlayer.h"
#include "Gameplay/ChessGame.h"
#include "Core/Chess_pch.h"
namespace Chess_Game
{
    class CurrentPlayerController
    {
    public:
        CurrentPlayerController(std::shared_ptr<ChessPlayer> white_team_player,
            std::shared_ptr<ChessPlayer> black_team_player) :
            m_WhitePlayer(white_team_player), m_BlackPlayer(black_team_player)
        {
            m_CurrentPlayer = m_WhitePlayer;
        }
        void SwitchPlayer() { m_CurrentPlayer = GetNotActivePlayer(); }
        std::shared_ptr<ChessPlayer> GetActivePlayer() { return m_CurrentPlayer; }
        std::shared_ptr<ChessPlayer> GetNotActivePlayer() { return m_CurrentPlayer == m_WhitePlayer ? m_BlackPlayer : m_WhitePlayer; }
    private:
        std::shared_ptr<ChessPlayer> m_WhitePlayer{};
        std::shared_ptr<ChessPlayer> m_BlackPlayer{};
        std::shared_ptr<ChessPlayer> m_CurrentPlayer{};
    };
}