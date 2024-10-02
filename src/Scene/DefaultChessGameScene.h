#pragma once
#include "Core/SceneObject.h"
#include "Gameplay/ChessGame.h"
#include "Gameplay/GameControllers/ChessGameController.h"
#include "GPU-Side/BatchRenderer.h"
#include "Helpers/PositionHelper.h"
#include "Core/EventListener.h"
#include "UI/UI_Image.h"
#include "Gameplay/GameControllers/PawnPromotionUIManager.h"
#include "Helpers/PausseMenuUIHelper.h"
#include <future>
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
        void DestroyScene() override;
        void OnEvent(const Event& e) override;
        void OnSelectedPieceChanged(std::weak_ptr<ChessPiece> new_selected_piece);
        void OnSelectedPieceMoved(std::shared_ptr<ChessPiece> selected_piece);
        void OnGameOver();
        std::shared_ptr<ChessPiece>OnPawnPromotion(bool is_white_team_pawn,BoardPosition new_pawn_pos);
    private:
        std::unique_ptr<ChessGameController> m_ChessGameController;
        std::shared_ptr<Drawable> m_ChessBoard{};
        std::shared_ptr<Drawable> m_SelectedPieceVisualIndicatorDrawable{};
        std::weak_ptr<Drawable> m_SelectedPieceDrawable{};
        std::shared_ptr<ScreenPositionHelper> m_PositionHelper;
        std::vector<BoardPosition> m_SelectedPiecePossiblePositions{};
        std::unique_ptr<PawnPromotionSelectionUI> m_PawnPromotionUIManager;
        std::unique_ptr<PauseMenuUIHelper> m_PauseMenuUIHelper;
        std::future<void> m_GameProcessInputThread{};
    };

}
