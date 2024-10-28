#pragma once
#include "Pch/Chess_pch.h"
#include "BaseChessPiece.h"
#include "ChessPlayer.h"

namespace Chess_Game
{

    /**
     * @brief Class representing a chess game.
     * 
     * The idea of the class is to represent a chess game by a given 2 players. While the chess pieces don't know chess rules like
     * king beeing check/checkmated this class is the one that keeps track of that.
     * 
     */
    class ChessGame {
    public:
        /**
         * @brief Constructor sets member variables and sets the 'ChessBoard' bit mask from the provided players.
         * 
         * @param white_player White chess player.
         * @param black_player Black chess player.
         * @param pawn_promotion_callback Callback on pawn promotion.
         */
        ChessGame(const std::shared_ptr<ChessPlayer>& white_player,
            const std::shared_ptr<ChessPlayer>& black_player,
            const std::function<std::shared_ptr<ChessPiece>(bool, BoardPosition)>& pawn_promotion_callback);
        /**
         * @brief Moves the current player selected piece to the provided new position.
         * 
         * Moves the current player selected piece to the provided new position without checking for the validity of the move.
         * After the move the function checks for piece capturing,pawn promotion and check/checkmate to the opposite player.
         * 
         * @param new_position The new position to move the current player selected piece.
         */
        void MoveCurrentPlayerSelectedPiece(BoardPosition new_position);
        /**
         * @brief Returns true if the move is valid(the move is done using the currently selected piece).
         *
         * Internally this function is just a facade like call it just calls 'CanMove()' with the appropriate params.
         * @param new_position The new position used to determine if the move is valid.
         * @returns Returns true if the move is valid.
         */
        bool CanMoveSelectedPiece(BoardPosition new_position){ return CanMove(new_position,m_ActivePlayer,m_NonActivePlayer);}
        /**
         * @brief Returns all valid moves by the selected piece. 
         * 
         * @param current_player The player containing the selected piece.
         * @param opposite_player The opposite player.
         * @returns Returns all valid moves by the selected piece.
         */
        std::vector<BoardPosition> GetSelectedPieceAllPossibleMoves(std::shared_ptr<ChessPlayer> current_player,
            std::shared_ptr<ChessPlayer> opposite_player);
        /**
         * @brief Updates the active and the opposite players.
         *
         * @param active_player New active player ref.
         * @param opposite_player New opposite player ref.
         *
         */
        void UpdateChessGamePlayers(const std::shared_ptr<ChessPlayer>& active_player,
            const std::shared_ptr<ChessPlayer>& opposite_player);
        /**
         * @brief Returns true if the one of the players king is checkmated.
         *
         * @returns Returns true if the one of the players king is checkmated.
         */
        bool IsGameOver()const {return m_IsKingCheckMated; }
        /**
         * @brief Returns an array of all of the chess board pieces.
         *
         * The function creates an array of both player pieces.
         * 
         * @returns Returns an array of all of the chess board pieces.
         */
        std::vector<std::shared_ptr<ChessPiece>> GetAllChessboardPieces();
    private: 
        /**
         * @brief Returns true if the new position will resolve the check.
         * Using the current player selected piece it checks if the move will resolve the check. If no 
         * check is present it returns true. If the new move can resolve the check it returns true otherwise false.
         * 
         * @param new_position The new position to check.
         * @returns  Returns true if the new position will resolve the check.
         */
        bool CanResolveCheck(BoardPosition new_position);
        /**
         * @brief Returns true if king is checked and sets the non-active player 'KingCheckData'.
         *
         * The function gets the non-active player king position and iterates through the active
         * player pieces. Using the 'ChessPiece' API 'CanMove()','CanMoveBoardSpecific()'
         * functions with the king position as new position. If both of the functions return true 
         * the piece is added to the non-active player 'KingCheckData'.
         * 
         * @returns Returns true if king is checked by atleast 1 piece.
         */
        bool IsKingChecked();
        /**
         * @brief Returns true if the non-active player is check mated.
         *
         * This function is called at the end of every valid move.
         * To determine if a check-mate has happened we gonna do the opposite and define when 
         * we don't have a check-mate.
         * 
         * One note a check-mate will happened if we have been checked if we have not then this 
         * function wont be called. 
         * 
         * By definition a check-mate can be understood as lack of possible movements.
         * To not have a check-mate we either:
         * 1) Can move the king to resolve the check.
         * 2) Can move another piece to block/resolve the check.
         * 3) Can claim the piece delivering the check with the king or another piece.
         * 
         * This means that we can have a union of these 3 criterias if any of them is true then
         * we are not check-mated.
         * 
         * The function does 2 checks using the following 2 utility function:
         * 1) 'CanKingResolveTheCheck()'
         * 2) 'CanOtherPiecesResolveTheCheck()'
         * 
         * @note The order in which these checks are performed does not affect the outcome,
         *       as either condition being true will prevent checkmate.
         *
         * We first try to see if the king can do any possible move @see 'CanKingResolveTheCheck()'.
         * If so we return false since we found a possible move/moves.
         * If that returns false we test every piece from the team if it can resolve the check.
         * Once again if that returns true we return false since we found a possible move/moves.
         * 
         * @returns Returns true if the non-active player is check mated.
         */
        bool IsKingCheckMated();
        /**
         * @brief Capture the opposite player piece and increase the score of the current player.
         * Removes the opposite player piece that needs to be captured and increase the score.
         * 
         * @param position_of_piece_to_capture The 'BoardPosition' of the piece to capture.
         */
        void CapturePiece(BoardPosition position_of_piece_to_capture);
        /**
         * @brief Returns true if the king can move and resolve the check.
         *
         * The function uses a set of pre-defined directions:
         * 1) Up
         * 2) Down
         * 3) Right
         * 4) Left
         * 5) Up-left
         * 6) Down-left
         * 7) Up-right
         * 8) Down-right
         * 
         * The idea is that we can try to move the king in any of these directions.
         * We simulate this movement by changing the state of the king piece and the 'ChessBoard'
         * and using the 'CanMove()' we test.For every direction in the set of directions
         * we move the king from its starting position and if the function 'CanMove()' 
         * returns true  we return true. If not we return false.
         *
         * 
         * @param player_to_check The player that we want to check.
         * @param opposite_player The opposite player.
         * 
         * @note This function is a utility and is meant to be used for checks that why it ask for 
         * us to pass the 'ChessPlayer' we want to check.
         * 
         * @returns Returns true if the king can move and resolve the check.
         */
        bool CanKingResolveTheCheck(std::shared_ptr<ChessPlayer> player_to_check,
            std::shared_ptr<ChessPlayer> opposite_player);
        /**
         * @brief Returns true if other pieces of the 'player_to_check' can resolve the king check.
         * 
         * From provided 2 players current and opposite the function gets the current player 'KingCheckData'.
         * If the current player is not beeing checked this function returns true. If not then it uses the 'ChessGame' API and the
         * pieces in the 'KingCheckData' to try and find a single possible move that can resolve the check. If atleast one move is found
         * it returns true if not it returns false.
         * 
         * @param player_to_check Player to check.
         * @param opposite_player Opposite player.
         * @returns Returns true if other pieces of the 'player_to_check' can resolve the king check.
         */
        bool CanOtherPiecesResolveTheCheck(std::shared_ptr<ChessPlayer> player_to_check,
            std::shared_ptr<ChessPlayer> opposite_player);
        /**
         * @brief Given 2 players it sets the 'ChessBoard' bit mask.
         * 
         * The function takes as input 2 players and uses their pieces to set up a 'ChessBoard' bit mask.
         * 
         * @param white_team_player The white team player.
         * @param black_team_player The black team player.
         * 
         */
        void SetupChessBoardBitMask(std::shared_ptr<ChessPlayer> white_team_player,
            std::shared_ptr<ChessPlayer> black_team_player);
        /**
         * @brief Determines if the move to the given position is valid based on a series of checks.
         *
         * This function aggregates multiple helper functions and checks to assess the validity of a move
         * according to the game's rules. Below is a list of the key checks that are performed:
         *
         * ### Key Checks and Functions:
         * 1. `IsNewPositionInBounds()`: First, the function checks if the new position is within the boundaries of the board.
         * 2. Check if the new position contains the opposite player's king. A player cannot move directly to a position occupied by the opposite player's king.
         * 3. `CanSelectedPieceMove()`: Uses the `ChessPlayer` API to determine whether the selected piece can make the move, both independently and with respect to the current board state.
         * 4. `CanResolveCheck()`: If the player is in check, this function verifies whether the new move will resolve the check. If it does not, the move is invalid.
         * 5. `IsKingSafeAfterMove()`: Ensures that the move does not place the player's own king in check after completion.
         *
         * @param new_position The target position the piece is attempting to move to.
         * @param player_to_check The player who is making the move (the active player).
         * @param opposite_player The opponent player (the non-active player).
         *
         * @note This function takes both the active and non-active players as parameters so it can be
         *       reused for different checking logic.
         *
         * @returns `true` if the move is valid according to all the checks, otherwise `false`.
         */
        bool CanMove(BoardPosition new_position, std::shared_ptr<ChessPlayer> player_to_check,
            std::shared_ptr<ChessPlayer> opposite_player);
        /**
         * @brief Returns true if the move wont cause the current player king to be checked.
         * 
         * This function uses the currently selected piece of the active player and simulates the move.
         * It changes the piece position and the 'ChessBoard' state. After that it uses the non-active 
         * player pieces to see if they can deliver a check to the king.
         * To see if a piece can deliver a check is enough to use the 'CanMove''function with the position
         * of the active player king. If the function returns true at any point it means that king is not 
         * safe after this move and the move is invalid.
         * 
         * @param new_position The new position to simulate the movement.
         * @param player_to_check The current player to simulate.
         * @param opposite_player The opposite player.
         * 
         * @returns Returns true if the move wont cause the current player king to be checked.
         */
        bool IsKingSafeAfterMove(BoardPosition new_position, std::shared_ptr<ChessPlayer> player_to_check,
            std::shared_ptr<ChessPlayer> opposite_player);
    private:
        ChessBoard m_ChessBoardData{};                    ///< Chess board bit mask.
        std::shared_ptr<ChessPlayer> m_ActivePlayer{};    ///< Currently active player.
        std::shared_ptr<ChessPlayer> m_NonActivePlayer{}; ///< Not active player.
        bool m_IsKingCheckMated = false;                  ///< True if any of the 2 players king is check-mated.
        std::function<std::shared_ptr<ChessPiece>(bool, BoardPosition)> m_OnPawnPromotion{}; ///< Callback called on pawn promotion.
    };
}