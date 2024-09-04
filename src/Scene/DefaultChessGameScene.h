#pragma once
#include "Core/SceneObject.h"
#include "Gameplay/ChessGame.h"
#include "GPU-Side/BatchRenderer.h"
#include "Helpers/PositionHelper.h"
#include "Core/EventListener.h"
namespace Chess_Game
{
    class DefaultChessScene : public SceneObject, public Listener
    {
    public:
        DefaultChessScene(std::weak_ptr<Application> main_application) :
            SceneObject(main_application)
        {}
    private:
        // Inherited via SceneObject
        void InitScene() override;
        void DrawScene() override;
        void OnUpdate() override;
        void DestroyScene() override;
    private:
        std::unique_ptr<ChessGame> m_ChessGame{};
        std::shared_ptr<ChessPlayer> m_WhitePlayer{};
        std::shared_ptr<ChessPlayer> m_BlackPlayer{};
        std::shared_ptr<Drawable> m_ChessBoard{};

        std::shared_ptr<ScreenPositionHelper> m_PositionHelper;
        BatchRenderer m_BatchRenderer;

        // Inherited via Listener
        void OnEvent(const Event& e) override;
    };

}
