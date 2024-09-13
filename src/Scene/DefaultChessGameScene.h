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
        { }
    private:
        void InitScene() override;
        void DrawScene(std::shared_ptr<BatchRenderer> application_batch_renderer) override;
        void OnUpdate() override;
        BoardPosition GetMouseInputBoardPosition(std::shared_ptr<Chess_Game::Application>& application);
        void DestroyScene() override;
        void OnEvent(const Event& e) override;
    private:
        std::unique_ptr<ChessGame> m_ChessGame{};
        std::shared_ptr<ChessPlayer> m_WhitePlayer{};
        std::shared_ptr<ChessPlayer> m_BlackPlayer{};
        std::shared_ptr<Drawable> m_ChessBoard{};
        std::weak_ptr<Drawable> m_SelectedPieceDrawable{};
        std::shared_ptr<ScreenPositionHelper> m_PositionHelper;
        std::vector<BoardPosition> m_SelectedPiecePossiblePositions{};

    };

}
