#pragma once
#include "Chess_pch.h"
#include "AssetLoader.h"
#include "Window.h"
#include "Core/SceneObject.h"
#include "Core/EventListener.h"
#include "Core/OrthographicApplicationMatrix.h"
#include "Core/Input.h"
#include "UI/UIManager.h"
#include "DrawableCreator.h"
#include "GPU-Side/Framebuffer.h"
namespace Chess_Game
{
    class Application : public std::enable_shared_from_this<Application>
    {
    public:
        Application();
        void StartRenderLoop();
        bool IsApplicationUnitizedSuccessfully()const { return m_ApplicationInitStatus; }
        const Window& GetApplicationWindow()const { return *m_ApplicationWindow; }
        void AddEventListener(std::weak_ptr<Listener> listener) { m_ActiveEventListeners.push_back(listener); }
        const OrthoViewportHandler& GetApplicationProjection() { return m_ApplicationProjection; }
        MouseInput& GetMouseInputManager() { return m_ApplicationMouseInput; }
        UIManager& GetUIManager() { return *m_ApplicationUIManager; }
        std::shared_ptr<AssetLoader> GetAssetLoader(){ return m_TextureAssetLoader; }
        std::shared_ptr<DrawableCreator> GetDrawableCreator() { return m_ApplicationDrawableCreator; }
        std::shared_ptr<BatchRenderer> GetBatchRenderer() { return m_ApplicationBatchRenderer; }
        void SwitchCurrentApplicationScene(std::shared_ptr<SceneObject> new_scene) { m_ToLoadScene = new_scene; }
    private:
        void InitAppResource();
        void OnEvent(const Event& e);
        void OnWindowResizeEvent(const WindowResizeEvent& e);
    private:
        bool m_ApplicationInitStatus = false;
        bool m_isApplicationRunning = true;
        MouseInput m_ApplicationMouseInput{};
        std::unique_ptr<Window> m_ApplicationWindow;
        std::shared_ptr<SceneObject> m_CurrentApplicationScene{};
        std::shared_ptr<SceneObject> m_ToLoadScene{};
        std::vector<std::weak_ptr<Listener>> m_ActiveEventListeners{};
        OrthoViewportHandler m_ApplicationProjection;
        std::shared_ptr<AssetLoader> m_TextureAssetLoader{};
        std::shared_ptr<UIManager> m_ApplicationUIManager{};
        std::shared_ptr<BatchRenderer> m_ApplicationBatchRenderer{};
        std::shared_ptr<DrawableCreator> m_ApplicationDrawableCreator{};
    };


}