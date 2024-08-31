#pragma once
#include"Window.h"
#include<memory>
#include<glm/glm.hpp>
#include<Core/Event/Event.h>
#include "Core/SceneObject.h"
namespace Chess_Game
{


    class Application 
    {
    public:
        Application();
        void RenderLoop();
        bool IsApplicationUnitizedSuccessfully()const { return m_ApplicationInitStatus; }
        void OnEvent(const Event& e);
    private:
        void CalculateOrthoProjection(const Size2D& window_size);
    private:
        bool m_ApplicationInitStatus = true;
        bool m_isApplicationRunning = true;
        std::unique_ptr<Window> m_ApplicationWindow;
        glm::mat4 m_ApplicationOrthographicProjection{};
        std::unique_ptr<SceneObject> m_CurrentApplicationScene{};
    };


}