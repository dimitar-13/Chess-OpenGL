#pragma once
#include "Core/Chess_pch.h"
#include "UI/UIElement.h"
namespace Chess_Game
{
    class MarginHelper 
    {
        MarginHelper();

    private:
        std::unordered_map<AnchorPoint_, UIElement> m_LastAttach{};
    };


}
