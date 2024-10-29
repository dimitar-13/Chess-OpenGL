#pragma once
namespace Chess_Game
{
    /**
     * @brief Represents a position on a chessboard.
     *
     * The BoardPosition struct defines a position on a standard 8x8 chessboard
     * using horizontal (file) and vertical (rank) coordinates. It includes methods
     * to compare, manipulate, and normalize positions.
     */
    struct BoardPosition
    {
        char horizontal_position{}; ///< Represents the file ('a' to 'h') on the chessboard.
        char vertical_position{};   ///< Represents the rank ('1' to '8') on the chessboard.

        bool operator==(const BoardPosition& other) const {
            return horizontal_position == other.horizontal_position && vertical_position == other.vertical_position;
        }
        BoardPosition operator-(const BoardPosition& other)const
        {
            return BoardPosition{ 
                static_cast<char>(horizontal_position - other.horizontal_position),
                static_cast<char>(vertical_position - other.vertical_position)};
        }
        BoardPosition& operator-=(const BoardPosition& other)
        {
            this->horizontal_position =- other.horizontal_position;
            this->vertical_position =- other.vertical_position;
            return *this;
        }

        /**
         * @brief Normalize a position.
         *
         * Reduces the values of a BoardPosition to their signs (-1, 0, or 1),
         * representing the direction of movement on the chessboard.
         *
         * @param position_to_normalize The BoardPosition to normalize.
         * @return A new BoardPosition where each coordinate is either -1, 0, or 1.
         */
        static BoardPosition Normalize(BoardPosition position_to_normalize)
        {
            position_to_normalize.vertical_position = position_to_normalize.vertical_position == 0 ? 0 :
                position_to_normalize.vertical_position / abs(position_to_normalize.vertical_position);

            position_to_normalize.horizontal_position = position_to_normalize.horizontal_position == 0 ? 0 :
                position_to_normalize.horizontal_position / abs(position_to_normalize.horizontal_position);

            return position_to_normalize;
        }

    };


}