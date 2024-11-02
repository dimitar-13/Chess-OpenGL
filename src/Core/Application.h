#pragma once
#include "Pch/Chess_pch.h"
#include "AssetLoader.h"
#include "Window.h"
#include "Core/SceneObject.h"
#include "Core/EventListener.h"
#include "Core/OrthographicApplicationMatrix.h"
#include "Core/Input.h"
#include "UI/UIManager.h"
#include "DrawableCreator.h"

namespace Chess_Game
{
    /**
     * @brief Centralized class for creating and managing application shared information.
     *
     * This class is the root class from where every other class is instantiated. The class have the job
     * of managing shared application resources like (assets,UI manager etc.) and providing way of getting
     * those resources.
     * 
     * Additionally the class contains the logic for scene and the rendering loop.
     */
    class Application : public std::enable_shared_from_this<Application>
    {
    public:
        /**
         * @brief Initializes GLEW library and creates the application resources.
         * 
         * Constructor creates a valid GLFW window. After that it initializes the GLEW lib 
         * and if all succeed it creates the application resources.
         */
        Application();
        /**
         * @brief Starts the application rendering loop.
         * The function calls the current scene Init function and starts the render loop.
         */
        void StartRenderLoop();
        /**
         * @brief Checks if the application was initialized successfully.
         * @returns Returns true if the application was initialized successfully.
         */
        bool IsApplicationUnitizedSuccessfully()const { return m_ApplicationInitStatus; }
        /**
         * @brief Gets the application window class.
         * @returns Returns a const ref to the application window class.
         */
        const Window& GetApplicationWindow()const { return *m_ApplicationWindow; }
        /**
         * @brief Adds a listener object to be notified when events happen.
         * The function receives a weak ref to the listener object and adds it to the listeners list.
         * If the object is destroyed at any point the class handles it by not calling the object.
         * 
         * @param listener A weak ref to the listener to be added.
         */
        void AddEventListener(std::weak_ptr<Listener> listener) { m_RegisteredEventListeners.push_back(listener); }
        /**
         * @brief Gets the application projection manager class.
         * @returns Returns the application projection manager class.
         */
        const OrthoViewportHandler& GetApplicationProjection() { return m_ApplicationProjection; }
        /**
         * @brief Gets the application mouse input manager.
         * @returns Returns the application mouse input manager.
         */
        MouseInput& GetMouseInputManager() { return m_ApplicationMouseInput; }
        /**
         * @brief Gets the application UI manager.
         * @returns Returns a reference to the application UI manager.
         */
        UIManager& GetUIManager() { return *m_ApplicationUIManager; }
        /**
         * @brief Gets the application UI manager.
         * @returns Returns strong ref to the application UI manager.
         */
        std::shared_ptr<UIManager> GetApplicationUIManager() { return m_ApplicationUIManager; }
        /**
         * @brief Gets the application asset loader.
         * @returns Returns the application asset loader.
         */
        std::shared_ptr<AssetLoader> GetAssetLoader(){ return m_TextureAssetLoader; }
        /**
         * @brief Gets the application drawable creator.
         * @returns Returns strong ref to the application drawable creator.
         */
        std::shared_ptr<DrawableCreator> GetDrawableCreator() { return m_ApplicationDrawableCreator; }
        /**
        * @brief Gets the application batch render.
        * @returns Returns a strong ref to the application batch renderer.
        */
        std::shared_ptr<BatchRenderer> GetBatchRenderer() { return m_ApplicationBatchRenderer; }
        /**
         * @brief Switches the current scene with the provided scene object.
         * The current scene is switched with the new one after the current frame ends.
         * 
         * @param new_scene The new scene object to be used.
         */
        void SwitchCurrentApplicationScene(std::shared_ptr<SceneObject> new_scene) { m_ToLoadScene = new_scene; }
    private:
        /**
         * @brief Creates and initializes all of the app resources.
         * 
         * Creates and initializes all of the app resources. Additionally it add some manager
         * classes to the be notified for events.
         */
        void InitAppResource();
        /**
         * @brief Function called on event.
         *
         * Unlike the pure virtual function in the 'Listener' interface this one is not from the implementation.
         * Events are publish either from the 'Application' class(the one in the current file) or the 
         * 'Window' class.
         * 
         * The function handles the event the 'Application' class is interested in and then it notifies
         * all of the saved event listeners.
         * 
         * @param e The event that was fired.
         */
        void OnEvent(const Event& e);
        /**
         * @brief Function for handling window resize events.
         *
         * The function handles the event by delegating the work to the 'OrthoViewportHandler'.
         * @param e The resize event data.
         */
        void OnWindowResizeEvent(const WindowResizeEvent& e);
    private:
        bool m_ApplicationInitStatus = false;                    ///< True if the application is initialized successfully.
        bool m_isApplicationRunning = true;                      ///< True if the application is running.   
        std::shared_ptr<SceneObject> m_CurrentApplicationScene{};///< Current scene used by the application.
        std::shared_ptr<SceneObject> m_ToLoadScene{};   ///< Scene to be loaded after the current 'OnUpdate' ends.
        std::vector<std::weak_ptr<Listener>> m_RegisteredEventListeners{}; ///< Event listeners.
        OrthoViewportHandler m_ApplicationProjection;   ///< Application orthographic & viewport helper.
        std::unique_ptr<Window> m_ApplicationWindow;    ///< Application GLFW window manager.
#pragma region Application resources and systems
        std::shared_ptr<AssetLoader> m_TextureAssetLoader{};             ///< Asset loader resource.
        std::shared_ptr<UIManager> m_ApplicationUIManager{};             ///< UI manager register/creator.
        std::shared_ptr<BatchRenderer> m_ApplicationBatchRenderer{};     ///< Batch renderer.
        std::shared_ptr<DrawableCreator> m_ApplicationDrawableCreator{}; ///< Drawable creator/register.
        MouseInput m_ApplicationMouseInput{};                            ///< Mouse input manager.
#pragma endregion
    };


}