#include "DefaultChessGameScene.h"
#include "Core/Application.h"
#include "Logging/Logger.h"
void Chess_Game::DefaultChessScene::InitScene()
{

    if (auto application = m_Application.lock())
    {
        m_PositionHelper = 
            std::make_shared<ScreenPositionHelper>(application->GetApplicationProjection().GetProjectionSize());

        application->AddEventListener(shared_from_this());
    }
    glm::vec3 board_position = glm::vec3(0.0f, 0.0f, -.5f);

    m_ChessBoard =
        std::make_shared<Drawable>(DrawableData{ TextureName_kBoard,board_position, glm::vec3(1),
            m_PositionHelper->GetBoardSize() });

    m_SceneObjects.push_back(m_ChessBoard);

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
        glm::vec3 piece_position = glm::vec3(m_PositionHelper->BoardToScreenPosition(piece->GetPiecePosition()), 0.f);

        glm::vec2 scale = m_PositionHelper->SingleSquareSize();
        auto piece_drawable = piece->GetPieceDrawable().lock();
        m_SceneObjects.push_back(piece_drawable);

        piece_drawable->SetPosition(piece_position);
        piece_drawable->SetColor(glm::vec3(1));
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
        glm::vec3 piece_position = 
            glm::vec3(m_PositionHelper->BoardToScreenPosition(piece->GetPiecePosition()), 0.f);

        glm::vec2 scale = m_PositionHelper->SingleSquareSize();

        auto piece_drawable = piece->GetPieceDrawable().lock();
        m_SceneObjects.push_back(piece_drawable);

        piece_drawable->SetPosition(piece_position);
        piece_drawable->SetColor(glm::vec3(0));
    }
    m_BlackPlayer = std::make_shared<ChessPlayer>(black_team_vector);

    m_ChessGame = std::make_unique<ChessGame>(m_WhitePlayer, m_BlackPlayer);



}

void Chess_Game::DefaultChessScene::DrawScene()
{
    if (auto application = m_Application.lock())
    {
        AssetLoader& kApplicationAssets = application->GetAssetLoader();

        for (const auto& drawable_weak_ptr : m_SceneObjects)
        {
            if (auto drawable = drawable_weak_ptr.lock())
            {
                m_BatchRenderer.Push(drawable->GetPosition(), drawable->GetScale(), drawable->GetColor(),
                    kApplicationAssets.GetTextureAsset(drawable->GetDrawableTextureName()));
            }
        }
        m_BatchRenderer.DrawTextureQuadBatch(application->GetApplicationProjection().GetMatrix());

        for (auto board_pos : m_SelectedPiecePossiblePositions)
        {
            glm::vec3 to_screen_pos =
                glm::vec3(m_PositionHelper->BoardToScreenPosition(board_pos), 1.f);
            m_BatchRenderer.PushCircle(to_screen_pos, glm::vec2{15}, glm::vec3(0,0,1));
        }
        m_BatchRenderer.DrawCircleBatch(application->GetApplicationProjection().GetMatrix());

    }
}

void Chess_Game::DefaultChessScene::OnUpdate()
{
    if (auto application = m_Application.lock())
    {
        if (application->GetMouseInputManager().IsMouseButtonPressed(MouseButton_kLeftMouseButton))
        {
            CHESS_LOG_INFO("Mouse button was pressed.");
            BoardPosition mouse_to_board_postion = GetMouseInputBoardPosition(application);
            //CHESS_LOG_INFO("The board position: {0} {1}", board_pos.horizontalPosition,char('0' + board_pos.VerticalPosition));


            if (!m_ChessGame->IsPieceSelected())
            {          
                m_ChessGame->SelectPiece(mouse_to_board_postion);            
            }

            if (m_ChessGame->IsSelectedPieceChanged())
            {
                m_SelectedPiecePossiblePositions = std::move(m_ChessGame->GetSelectedPieceAllPossibleMoves());
            }
            else if (m_ChessGame->CanMoveSelectedPiece(mouse_to_board_postion))
            {
                std::shared_ptr<ChessPiece> selected_piece = m_ChessGame->GetSelectedPiece().lock();
                auto piece_drawable = selected_piece->GetPieceDrawable().lock();
                glm::vec2 new_position = m_PositionHelper->BoardToScreenPosition(mouse_to_board_postion);
                piece_drawable->SetPosition(glm::vec3(new_position, .0f));

                m_ChessGame->MoveSelectedPiece(mouse_to_board_postion);
                m_SelectedPiecePossiblePositions.clear();
            }  
      
        }
        if (m_ChessGame->IsGameOver())
        {
            CHESS_LOG_FATAL("Game is over!");
        }
    }
}

Chess_Game::BoardPosition Chess_Game::DefaultChessScene::GetMouseInputBoardPosition(std::shared_ptr<Chess_Game::Application>& application)
{
    MousePos screen_coordinates = application->GetMouseInputManager().GetMousePosition();
    glm::vec2 converted_screen_coords = glm::vec2{ screen_coordinates.x,screen_coordinates.y };
    //Convert from screen to orthographic
    glm::vec2 orthographic_coordinates =
        application->GetApplicationProjection().FromScreenToOrthographicCoordinates(converted_screen_coords);

    return m_PositionHelper->ScreenPositionToBoard(orthographic_coordinates);
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
            if (auto drawable = piece_attachable->GetPieceDrawable().lock())
            {
                glm::vec2 retrieved_pos = 
                    m_PositionHelper->BoardToScreenPosition(piece_attachable->GetPiecePosition());

                glm::vec3 piece_position = glm::vec3(retrieved_pos.x, retrieved_pos.y, 0.0f);
                drawable->SetPosition(piece_position);
            }
        }

        for (const auto& piece_attachable : m_BlackPlayer->GetPlayerPieces())
        {
            if (auto drawable = piece_attachable->GetPieceDrawable().lock())
            {
                glm::vec2 retrieved_pos = 
                    m_PositionHelper->BoardToScreenPosition(piece_attachable->GetPiecePosition());

                glm::vec3 piece_position = glm::vec3(retrieved_pos.x, retrieved_pos.y, 0.0f);
                drawable->SetPosition(piece_position);
            }
        }
        glm::vec3 board_position = glm::vec3(0.0f, 0.0f, -.5f);

        m_ChessBoard->SetPosition(board_position);
        m_ChessBoard->SetScale(m_PositionHelper->GetBoardSize());
    }
}
