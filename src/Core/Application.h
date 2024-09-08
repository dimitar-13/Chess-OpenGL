#pragma once
#include "AssetLoader.h"
#include"Window.h"
#include<memory>
#include<glm/glm.hpp>
#include<Core/Event/Event.h>
#include "Core/SceneObject.h"
#include "Core/EventListener.h"
#include "Core/OrthographicApplicationMatrix.h"
#include "Core/Input.h"
#include "gpu-side/ShaderClass.h"

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
        void AddEventListener(std::weak_ptr<Listener> listener) { m_ActiveEventListeners.push_back(listener); }
        const OrthoViewportHandler& GetApplicationProjection() { return m_ApplicationProjection; }
        MouseInput& GetMouseInputManager() { return m_ApplicationMouseInput; }
        AssetLoader& GetAssetLoader(){ return *m_TextureAssetLoader; }
        ShaderClass& GetTestShader() { return *m_TestImageShader; }
    private:
        void OnEvent(const Event& e);
        void OnWindowResizeEvent(const WindowResizeEvent& e);
    private:
        bool m_ApplicationInitStatus = true;
        bool m_isApplicationRunning = true;
        MouseInput m_ApplicationMouseInput{};
        std::unique_ptr<Window> m_ApplicationWindow;
        std::shared_ptr<SceneObject> m_CurrentApplicationScene{};
        std::vector<std::weak_ptr<Listener>> m_ActiveEventListeners{};
        OrthoViewportHandler m_ApplicationProjection;
        std::unique_ptr<AssetLoader> m_TextureAssetLoader{};
        std::unique_ptr<ShaderClass> m_TestImageShader{};
    };


}