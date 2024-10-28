#pragma once
#include "Pch/Chess_pch.h"
#include "Core/Drawable.h"
#include "Gpu-Side/BatchRenderer.h"
namespace Chess_Game
{
    class Application;

    class SceneObject
    {
    public:
        SceneObject(std::weak_ptr<Application> main_application) :
            m_Application(main_application)
        {}
        virtual void InitScene() = 0;
        virtual void DrawScene(std::shared_ptr<BatchRenderer> application_batch_renderer) = 0;
        virtual void OnUpdate() = 0;
        virtual void DestroyScene() = 0;
    protected:
        std::vector<std::weak_ptr<Drawable>> m_SceneObjects{};
        std::weak_ptr<Application> m_Application{};
    };
}