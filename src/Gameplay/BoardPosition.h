#pragma once
namespace Chess_Game
{
    struct BoardPosition
    {
        char horizontalPosition{};
        char VerticalPosition{};
        BoardPosition() = default;

        bool operator==(const BoardPosition& other) const {
            return horizontalPosition == other.horizontalPosition && VerticalPosition == other.VerticalPosition;
        }
        BoardPosition operator-(const BoardPosition& other)const
        {
            return BoardPosition{ 
                static_cast<char>(horizontalPosition - other.horizontalPosition),
                static_cast<char>(VerticalPosition - other.VerticalPosition)};
        }
        BoardPosition& operator-=(const BoardPosition& other)
        {
            this->horizontalPosition =- other.horizontalPosition;
            this->VerticalPosition =- other.VerticalPosition;
            return *this;
        }

        static BoardPosition Normalize(BoardPosition position_to_normalize)
        {
            position_to_normalize.VerticalPosition = position_to_normalize.VerticalPosition == 0 ? 0 :
                position_to_normalize.VerticalPosition / abs(position_to_normalize.VerticalPosition);

            position_to_normalize.horizontalPosition = position_to_normalize.horizontalPosition == 0 ? 0 :
                position_to_normalize.horizontalPosition / abs(position_to_normalize.horizontalPosition);

            return position_to_normalize;
        }

    };


}