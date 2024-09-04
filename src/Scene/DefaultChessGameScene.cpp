#include "DefaultChessGameScene.h"
#include "Core/Application.h"

void Chess_Game::DefaultChessScene::InitScene()
{
    if (auto application = m_Application.lock())
    {
        m_PositionHelper = 
            std::make_shared<ScreenPositionHelper>(application->GetApplicationProjection().GetProjectionSize());

        application->AddEventListener(shared_from_this());
    }
    std::vector<std::shared_ptr<ChessPiece>> white_team_vector;


    // Setup White Pieces
    white_team_vector.push_back(std::make_shared<Rook>(BoardPosition{ 'a', 1 }));

    white_team_vector.push_back(std::make_shared<Knight>(BoardPosition{ 'b', 1 }));

    white_team_vector.push_back(std::make_shared<Bishop>(BoardPosition{ 'c', 1 }));

    white_team_vector.push_back(std::make_shared<Queen>(BoardPosition{ 'd', 1 }));

    white_team_vector.push_back(std::make_shared<King>(BoardPosition{ 'e', 1 }));

    white_team_vector.push_back(std::make_shared<Bishop>(BoardPosition{ 'f', 1 }));

    white_team_vector.push_back(std::make_shared<Knight>(BoardPosition{ 'g', 1 }));

    white_team_vector.push_back(std::make_shared<Rook>(BoardPosition{ 'h', 1 }));

    // White Pawns
    for (char file = 'a'; file <= 'h'; ++file) {
        auto pawn = std::make_shared<Pawn>(BoardPosition{ file, 2 });
        white_team_vector.push_back(pawn);
    }
    for (auto& piece : white_team_vector)
    {
        piece->AttachDrawable(std::make_shared<Drawable>(
            m_PositionHelper->BoardToScreenPosition(piece->GetPiecePosition()),glm::vec3(1)));
    }

    m_WhitePlayer = std::make_shared<ChessPlayer>(white_team_vector);

    std::vector<std::shared_ptr<ChessPiece>> black_team_vector;

    black_team_vector.push_back(std::make_shared<Rook>(BoardPosition{ 'a', 8 }));

    black_team_vector.push_back(std::make_shared<Knight>(BoardPosition{ 'b', 8 }));

    black_team_vector.push_back(std::make_shared<Bishop>(BoardPosition{ 'c', 8 }));

    black_team_vector.push_back(std::make_shared<Queen>(BoardPosition{ 'd', 8 }));

    black_team_vector.push_back(std::make_shared<King>(BoardPosition{ 'e', 8 }));

    black_team_vector.push_back(std::make_shared<Bishop>(BoardPosition{ 'f', 8 }));

    black_team_vector.push_back(std::make_shared<Knight>(BoardPosition{ 'g', 8 }));

    black_team_vector.push_back(std::make_shared<Rook>(BoardPosition{ 'h', 8 }));

    // Black Pawns
    for (char file = 'a'; file <= 'h'; ++file) {
        auto pawn = std::make_shared<Pawn>(BoardPosition{ file, 7 });
        black_team_vector.push_back(pawn);
    }

    for (auto& piece : black_team_vector)
    {
        piece->AttachDrawable(std::make_shared<Drawable>(
           m_PositionHelper->BoardToScreenPosition(piece->GetPiecePosition()), glm::vec3(0)));
    }
    m_BlackPlayer = std::make_shared<ChessPlayer>(black_team_vector);


    for (const auto& piece_attachable : m_WhitePlayer->GetPlayerPieces())
    {
        m_SceneObjects.push_back(piece_attachable->GetDrawable());
    }

    for (const auto& piece_attachable : m_BlackPlayer->GetPlayerPieces())
    {
        m_SceneObjects.push_back(piece_attachable->GetDrawable());
    }

    m_ChessGame = std::make_unique<ChessGame>(m_WhitePlayer, m_BlackPlayer);
    m_ChessBoard = std::make_shared<Drawable>(glm::vec2(0), glm::vec3(0, 1, 0));
    m_SceneObjects.push_back(m_ChessBoard);
}

void Chess_Game::DefaultChessScene::DrawScene()
{
    for (const auto& drawable_weak_ptr : m_SceneObjects)
    {
        if (auto drawable = drawable_weak_ptr.lock())
        {
            m_BatchRenderer.Push(drawable->GetPosition(), drawable->GetScale(), drawable->GetColor());
        }
    }
    m_BatchRenderer.Flush();
}

void Chess_Game::DefaultChessScene::OnUpdate()
{
    //if (Mouse is pressed)
    //{
        //Get mouse position
        //Convert it to board position
        //Move if needed
  //  }

}

void Chess_Game::DefaultChessScene::DestroyScene()
{
}

void Chess_Game::DefaultChessScene::OnEvent(const Event& e)
{
    if (e.GetEventType() == EventType_kWindowResize)
    {
        if(auto app = m_Application.lock())
            m_PositionHelper->UpdateProjectionBorder(app->GetApplicationProjection().GetProjectionSize());

        for (const auto& piece_attachable : m_WhitePlayer->GetPlayerPieces())
        {
            if(auto drawable = piece_attachable->GetDrawable().lock())
                drawable->SetPosition(
                    m_PositionHelper->BoardToScreenPosition(piece_attachable->GetPiecePosition()));
        }

        for (const auto& piece_attachable : m_BlackPlayer->GetPlayerPieces())
        {
            if (auto drawable = piece_attachable->GetDrawable().lock())
                drawable->SetPosition(
                    m_PositionHelper->BoardToScreenPosition(piece_attachable->GetPiecePosition()));
        }

    }
}
