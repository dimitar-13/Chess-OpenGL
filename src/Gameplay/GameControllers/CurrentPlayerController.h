#pragma once
#include "Gameplay/BaseChessPiece.h"
#include "Gameplay/ChessPlayer.h"
#include "Gameplay/ChessGame.h"
#include "Core/Chess_pch.h"
namespace Chess_Game
{
    /**
     * @brief Manages the current player in a chess game, allowing for player switching.
     *
     * The 'CurrentPlayerController' class is responsible for managing the active player in a chess game.
     * It keeps track of two players (white and black) and provides functionality to switch between them
     * during the game. The class ensures that the current player can be easily retrieved and toggled
     * between turns.
     *
     */
    class CurrentPlayerController
    {
    public:
        /**
         * @brief Constructs a 'CurrentPlayerController' with the given players.
         *
         * Initializes the player controller with the white and black players, setting the
         * white player as the starting active player.
         *
         * @param white_team_player The player representing the white team.
         * @param black_team_player The player representing the black team.
         */
        CurrentPlayerController(std::shared_ptr<ChessPlayer> white_team_player,
            std::shared_ptr<ChessPlayer> black_team_player) :
            m_WhitePlayer(white_team_player), m_BlackPlayer(black_team_player)
        {
            m_CurrentPlayer = m_WhitePlayer;
        }
        /**
         * @brief Switches the active player to the non-active player.
         *
         * This function toggles the current active player between the white and black players.
         */
        void SwitchPlayer() { m_CurrentPlayer = GetNotActivePlayer(); }
        /**
         * @brief Retrieves the current active player.
         *
         * @return A shared pointer to the active `ChessPlayer`.
         */
        std::shared_ptr<ChessPlayer> GetActivePlayer() { return m_CurrentPlayer; }
        /**
         * @brief Retrieves the player who is not currently active.
         *
         * @return A shared pointer to the non-active `ChessPlayer`.
         */
        std::shared_ptr<ChessPlayer> GetNotActivePlayer(){return m_CurrentPlayer == m_WhitePlayer ? m_BlackPlayer : m_WhitePlayer; }

    private:
        std::shared_ptr<ChessPlayer> m_WhitePlayer{};   ///< The white team player.
        std::shared_ptr<ChessPlayer> m_BlackPlayer{};   ///< The black team player.
        std::shared_ptr<ChessPlayer> m_CurrentPlayer{}; ///< The current active player (either white or black).
    };

}