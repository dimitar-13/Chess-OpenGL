#pragma once
#include "Core/Event/Event.h"
namespace Chess_Game
{
    class Listener : public std::enable_shared_from_this<Listener>
    {
    public:
        virtual void OnEvent(const Event& e) = 0;
    };
}