#pragma once
#include "Core/SceneObject.h"
#include "UI/Button.h"
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
        void DrawScene(std::shared_ptr<BatchRenderer> application_batch_renderer) override;
        void OnUpdate() override;
        void DestroyScene() override;
    private:
        std::shared_ptr<Drawable> m_GameLogo{};
        std::shared_ptr<Button> m_StartButton{};
    };
}