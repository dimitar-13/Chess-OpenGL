#pragma once
#include "Core/SceneObject.h"
#include "UI/Button.h"
#include "UI/UI_Image.h"
#include "UI/TextUI.h"
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
        std::shared_ptr<Button> m_StartButton{};
        std::shared_ptr<TextUI> m_StartText{};
        std::shared_ptr<Image> m_GameLogoImage{};
        std::shared_ptr<Panel> m_StartMenuPanel{};
        std::shared_ptr<TextUI> m_GameNameUIText{};
    };
}