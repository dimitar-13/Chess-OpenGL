#pragma once
#include "Core/SceneObject.h"
#include "Gameplay/ChessGame.h"
#include "GPU-Side/BatchRenderer.h"
namespace Chess_Game
{
    class DefaultChessScene : public SceneObject
    {
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
        BatchRenderer m_BatchRenderer;
    };

}
