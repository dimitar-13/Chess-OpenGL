#pragma once
#include "Pch/Chess_pch.h"
#include "Core/Drawable.h"
#include "Gpu-Side/BatchRenderer.h"
namespace Chess_Game
{
    class Application;

    /**
     * @brief Base class for creating a scene.
     *
     * This is mainly here for ease of testing since every scene is easier to test separately.
     */
    class SceneObject
    {
    public:
        /**
         * @brief Constructor uses Dependency Injection(DI) to resolve the application class dependency. 
         */
        SceneObject(std::weak_ptr<Application> main_application) :
            m_Application(main_application)
        {}
        /**
         * @brief Virtual function called when scene must be initialized. 
         */
        virtual void InitScene() = 0;
        /**
         * @brief Virtual function for drawing the scene. 
         * @param application_batch_renderer The batch renderer to use for drawing.
         */
        virtual void DrawScene(std::shared_ptr<BatchRenderer> application_batch_renderer) { };
        /**
         * @brief Virtual function called every frame update. 
         */
        virtual void OnUpdate() = 0;
        /**
         * @brief Virtual function for destroying the scene. 
         */
        virtual void DestroyScene() { };
    protected:
        std::vector<std::weak_ptr<Drawable>> m_SceneObjects{};  ///< Scene drawable objects array.
        std::weak_ptr<Application> m_Application{};             ///< Weak ref to application.
    };
}