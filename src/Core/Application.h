#pragma once
#include"Window.h"
#include<memory>
#include<glm/glm.hpp>
#include<Core/Event/Event.h>
#include "Core/SceneObject.h"
#include "Core/EventListener.h"
#include "Core/OrthographicApplicationMatrix.h"
namespace Chess_Game
{


    class Application : public std::enable_shared_from_this<Application>
    {
    public:
        Application();
        void RenderLoop();
        bool IsApplicationUnitizedSuccessfully()const { return m_ApplicationInitStatus; }
        const Window& GetApplicationWindow()const { return *m_ApplicationWindow; }
        Window& GetApplicationWindow() { return *m_ApplicationWindow; }
        void OnEvent(const Event& e);
        void AddEventListener(std::weak_ptr<Listener> listener) { m_ActiveEventListeners.push_back(listener); }
        const ApplicationProjection& GetApplicationProjection() { return m_ApplicationProjection; }
    private:
        void CalculateViewportTransform();
    private:
        bool m_ApplicationInitStatus = true;
        bool m_isApplicationRunning = true;
        glm::mat4 m_ViewportTransform{};
        std::unique_ptr<Window> m_ApplicationWindow;
        std::shared_ptr<SceneObject> m_CurrentApplicationScene{};
        std::vector<std::weak_ptr<Listener>> m_ActiveEventListeners{};
        ApplicationProjection m_ApplicationProjection{};
    };


}