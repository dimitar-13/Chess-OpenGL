#pragma once
#include "Core/Chess_pch.h"
#include "BaseChessPiece.h"
#include <future>
namespace Chess_Game
{
    /**
     * @brief A structure that holds data related to a king being in check.
     *
     * This structure encapsulates information about the king's check status during a chess game.
     * It tracks whether the king is currently in check and the pieces responsible for delivering
     * the check.
     */
    struct KingCheckData
    {
        bool is_king_checked = false; ///< Is the king beeing checked.
        std::vector<std::shared_ptr<ChessPiece>> pieces_delivering_check{};///< The pieces checking the king.
    };

    struct OptionalIndex
    {
        OptionalIndex() = default;
        OptionalIndex(size_t value):index_value(value), is_set(true)
        {}
        size_t index_value{};
        bool is_set = false;
    };

    /**
     * @brief A class representing a chess player in the game.
     *
     * The `ChessPlayer` class manages a collection of chess pieces and provides functionality
     * to select, move, promote, and remove pieces. The player itself is unaware of which team
     * (white or black) they belong to; it only manages a dynamic array of chess pieces, represented
     * by a `std::vector<std::shared_ptr<ChessPiece>>`.
     *
     * Key responsibilities of the `ChessPlayer` class include:
     * - Piece Selection: Allows the player to select a specific chess piece for movement.
     * - Piece Management: Provides functionality to remove a piece from the player's collection
     *   (e.g., when captured) or promote a pawn (e.g., to a queen).
     * - Movement Validation: Includes a `CanMove` function, which checks if the currently
     *   selected piece can move to a new position using the `ChessPiece` API.
     * - Score Tracking: Keeps track of the player's score based on game progress (e.g.,
     *   capturing opponent's pieces).
     *
     * @note The class operates on a high level and delegates piece-specific operations, such as
     *       movement validation, to the `ChessPiece` objects it manages.
     * @note The class also is not aware of other players this again is handled by a higher level system
     *       `ChessPiece`.
     * 
     * @warning The class does not have any direct concept of teams or player color. This is
     *          managed in 'ChessGame' class.
     */
    class ChessPlayer
    {
    public:
        /**
         * @brief Constructor initializes a 'ChessPlayer' on the provided piece array.
         * 
         * Constructor copies the provided starting_pieces and iterates to find the King 
         * chess piece and saves a ref to it.
         * 
         * @warning As of for now if there is no King chess piece in the 'starting_pieces' array
         *       there will be a crash.
         * 
         */
        ChessPlayer(const std::vector<std::shared_ptr<ChessPiece>>& starting_pieces);
        /**
         * @brief Attempts to select a piece given a board position.
         *
         * Given a board position the this function attempts to select a piece. It loops over the 
         * array of pieces and finds the one on using the provided position.
         * 
         * @note While you might consider hash map better here. For approximately 16 pieces this 
         * is good enough and in my opinion better in space compared to hash map.
         * 
         * @param piece_board_position The piece board position.
         */
        void SelectPiece(BoardPosition piece_board_position);
        /**
         * @brief Sets the currently selected chess piece.
         *
         * This function allows the player to select a specific chess piece for potential movement or
         * other actions. The selected piece is stored as a `std::shared_ptr` to ensure proper
         * ownership and reference counting.
         *
         * @param new_piece A shared pointer to the chess piece that the player selects. This should
         *        not be null, as the selection is required for further actions such as movement.
         *
         * @note This function overwrites any previously selected piece.
         */
        void SetSelectedPiece(std::shared_ptr<ChessPiece> new_piece);
        /**
         * @brief Deselects the currently selected chess piece.
         *
         * This function clears the player's currently selected chess piece by setting it to a
         * default `std::weak_ptr`. Once a piece is deselected, the player cannot perform actions
         * on any piece until a new one is selected.
         *
         * @note This function does not destroy or affect the underlying chess piece, it simply
         *       removes the reference from the selection.
         */
        void UnSelectPiece() { m_SelectedPiece = std::weak_ptr<ChessPiece>(); }
        /**
         * @brief Removes a chess piece from the player's collection.
         *
         * This function removes the chess piece located at the given board position from the player's
         * dynamic array of pieces.
         *
         * @param piece_board_position The board position of the piece to be removed.
         *
         * @note If no piece is found at the specified position, no action is taken.
         */
        void RemovePiece(BoardPosition piece_board_position);
        /**
         * @brief Moves the selected piece to the specified position.
         *
         * When this function is called it moves and it resets the player 'KingCheckData'.
         * 
         * @note This function does not do any checks. It assumes the caller has used the API to determine
         * if the new position is to be deemed valid.
         * 
         * @param new_piece_position The new position to move the selected piece to.
         */
        void MoveSelectedPiece(BoardPosition new_piece_position);
        /**
         * @brief Returns a const ref to the player piece array.
         *
         * @returns Returns a const ref to the player piece array.
         */
        const std::vector<std::shared_ptr<ChessPiece>>& GetPlayerPieces() { return m_PlayerPieces; }
        /**
         * @brief Updates the 'KingCheckData' to the proved one.
         * 
         * @param data The new 'KingCheckData' to be used.
         * 
         * @note As with other logic this data is created and used by a higher level class
         *       this class acts as a container to it.
         * 
         * @see 'ChessGame'.
         */
        void SetPlayerKingCheckData(const KingCheckData& data) { m_PlayerKingCheckData = data; }
        /**
         * @brief Returns the currrently stored 'KingCheckData'.
         * 
         * @note As with other logic this data is created and used by a higher level class
         *       this class acts as a container to it.
         *
         * @see 'ChessGame'.
         * 
         * @returns Returns a const ref to the currrently stored 'KingCheckData'.
         */
        const KingCheckData& GetPlayerKingCheckData() { return m_PlayerKingCheckData; }
        /**
         * @brief Returns the King piece.
         *
         * @returns Returns the King piece.
         */
        const King& GetPlayerKing() { return *m_PlayerKing; }
        /**
         * @brief Returns a weak ref to the currently selected piece.
         *
         * @note If no selected piece this will return null
         *      use 'GetSelectedPiece().expired()' to check if that is the case
         * 
         * @returns Returns a weak ref to the currently selected piece.
         */
        std::weak_ptr<ChessPiece> GetSelectedPiece() { return m_SelectedPiece; }
        /**
         * @brief Increases the score by an specified amount.
         *
         * @param amount The amount to increase the score.
         */
        void IncreaseScore(size_t amount) { m_PlayerScore += amount; }
        /**
         * @brief Returns true if the new position obeys the Movement and BoarSpecific movement rules.
         *
         * @see 'BoardSpecificMovementRule' and 'MovementRule'.
         * 
         * The function uses the 'ChessPiece' API to determine if the new position obeys the piece
         * 'BoardSpecificMovementRule' and 'MovementRule' rules.
         * 
         * @note This function does not care about game logic such claiming king or check mate logic
         *       this again is handled by a higher level class @see 'ChessGame'.
         * 
         * @param new_position Position to test.
         * @param chess_board ChessBoard big mask.
         * 
         * @returns Returns true if the new position obeys both 'BoardSpecificMovementRule' and 'MovementRule'.
         */
        bool CanSelectedPieceMove(BoardPosition new_position, ChessBoard& chess_board);
    private:
        /**
         * @brief Retrieves the index of the chess piece at the given board position.
         *
         * This function searches the player's collection of chess pieces and returns the array index
         * of the piece located at the specified `BoardPosition`. If no piece is found at the given
         * position, the function returns an `OptionalIndex` indicating that the piece is not present.
         *
         * @param piece_board_position The position on the board where the piece is located.
         * @return OptionalIndex The index of the piece in the player's array if found, or an empty
         *         `OptionalIndex` if no piece is located at the specified position.
         *
         * @note The `OptionalIndex` type is used to handle cases where the piece may not exist at the
         *       given position, providing a safer alternative to returning invalid or out-of-bounds indices.
         */
        OptionalIndex GetPieceArrayIndex(BoardPosition piece_board_position) const;
    private:
        std::vector<std::shared_ptr<ChessPiece>> m_PlayerPieces; ///< Player pieces.
        std::shared_ptr<King> m_PlayerKing; ///< Ref to the player king for fast access.
        std::weak_ptr<ChessPiece> m_SelectedPiece; ///< Weak ref to the selected piece.If no selected this is null.
        size_t m_PlayerScore{}; ///< Current player score.
        KingCheckData m_PlayerKingCheckData{}; ///< Player current king check data.
    };
}