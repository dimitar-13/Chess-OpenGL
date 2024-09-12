#pragma once
#include "Core/SceneObject.h"
namespace Chess_Game
{
    class MainMenuScene : public SceneObject
    {
    public:
        MainMenuScene(std::weak_ptr<Application> main_application):
            SceneObject(main_application)
        {}
    private:
        // Inherited via SceneObject
        void InitScene() override;
        void DrawScene() override;
        void OnUpdate() override;
        void DestroyScene() override;
    };
}