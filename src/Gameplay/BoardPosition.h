#pragma once
#include <iostream>
#include <unordered_map>
#include <memory>
#include <array>
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
    };
}