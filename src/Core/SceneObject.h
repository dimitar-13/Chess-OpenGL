#pragma once
#include <vector>
#include <memory>
#include "Core/Drawable.h"
namespace Chess_Game
{
    class SceneObject 
    {
    public:
        virtual void InitScene() = 0;
        virtual void DrawScene() = 0;
        virtual void OnUpdate() = 0;
        virtual void DestroyScene() = 0;
    protected:
        std::vector<std::weak_ptr<Drawable>> m_SceneObjects{};
    };
}