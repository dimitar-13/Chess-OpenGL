#include "DefaultChessGameScene.h"
#include "Core/Application.h"
#include "Logging/Logger.h"
#include "Scene/MainMenuScene.h"
#include "Core/DrawableCreator.h"
static constexpr float kBoardDepth = -.5f;
static constexpr float kChessPieceVisualSelectionDepth = .0f;
static constexpr float kChessPieceDepth = .5f;
static constexpr float kPossibleMoveVisualDepth = 1.f;




void Chess_Game::DefaultChessScene::InitScene()
{
    auto GetTextureNameByPieceType = [](ChessPieceType_ type_of_piece)
        {
            switch (type_of_piece)
            {       
            case Chess_Game::ChessPieceType_kPawn:
                return TextureName_kPawn;
            case Chess_Game::ChessPieceType_kBishop:
                return TextureName_kBishop;
            case Chess_Game::ChessPieceType_kRook:
                return TextureName_kRook;
            case Chess_Game::ChessPieceType_kKnight:
                return TextureName_kKnight;
            case Chess_Game::ChessPieceType_kQueen:
                return TextureName_kQueen;
            case Chess_Game::ChessPieceType_kKing:
                return TextureName_kKing;
            default:
                return TextureName_kPawn;
            }
        };

    if (auto application = m_Application.lock())
    {
        m_PositionHelper =
            std::make_shared<ScreenPositionHelper>(application->GetApplicationProjection().GetProjectionSize());

        CreateSceneUI();

        application->AddEventListener(shared_from_this());

        std::shared_ptr<DrawableCreator> main_drawable_creator = application->GetDrawableCreator();
        
        m_ChessBoard = main_drawable_creator->CreateDrawable();
        
        m_ChessBoard->SetPosition(glm::vec3(0.0f, 0.0f, kBoardDepth));
        m_ChessBoard->SetDrawableTextureName(TextureName_kBoard);
        m_ChessBoard->SetScale(m_PositionHelper->GetBoardSize());
        m_ChessBoard->SetColor(glm::vec3(1));
        
        
        m_SceneObjects.push_back(m_ChessBoard);
        
        std::vector<std::shared_ptr<ChessPiece>> white_team_vector;
        
        // Setup White Pieces
        white_team_vector.push_back(std::make_shared<Rook>(BoardPosition{ 'a', 1 }, main_drawable_creator->CreateDrawable()));
        
        white_team_vector.push_back(std::make_shared<Knight>(BoardPosition{ 'b', 1 }, main_drawable_creator->CreateDrawable()));
        
        white_team_vector.push_back(std::make_shared<Bishop>(BoardPosition{ 'c', 1 }, main_drawable_creator->CreateDrawable()));
        
        white_team_vector.push_back(std::make_shared<Queen>(BoardPosition{ 'd', 1 }, main_drawable_creator->CreateDrawable()));
        
        white_team_vector.push_back(std::make_shared<King>(BoardPosition{ 'e', 1 }, main_drawable_creator->CreateDrawable()));
        
        white_team_vector.push_back(std::make_shared<Bishop>(BoardPosition{ 'f', 1 }, main_drawable_creator->CreateDrawable()));
        
        white_team_vector.push_back(std::make_shared<Knight>(BoardPosition{ 'g', 1 }, main_drawable_creator->CreateDrawable()));
        
        white_team_vector.push_back(std::make_shared<Rook>(BoardPosition{ 'h', 1 }, main_drawable_creator->CreateDrawable()));
        
        m_SelectedPieceVisualIndicatorDrawable = main_drawable_creator->CreateDrawable();
        m_SelectedPieceVisualIndicatorDrawable->SetColor(glm::vec3{ 1,0.843,0 });
        m_SelectedPieceVisualIndicatorDrawable->EnableDrawable(false);
        // White Pawns
        for (char file = 'a'; file <= 'h'; ++file) {
            auto pawn = std::make_shared<Pawn>(BoardPosition{ file, 2 }, main_drawable_creator->CreateDrawable());
            white_team_vector.push_back(pawn);
        }
        for (auto& piece : white_team_vector)
        {
            glm::vec3 piece_position = glm::vec3(m_PositionHelper->BoardToScreenPosition(piece->GetPiecePosition()), kChessPieceDepth);
        
            glm::vec2 scale = m_PositionHelper->SingleSquareSize();
            auto piece_drawable = piece->GetPieceDrawable().lock();
            m_SceneObjects.push_back(piece_drawable);
        
            piece_drawable->SetPosition(piece_position);
            piece_drawable->SetColor(glm::vec3(1));
            piece_drawable->SetDrawableTextureName(GetTextureNameByPieceType(piece->GetChessPieceType()));
        
        }
        
        auto white_team_player = std::make_shared<ChessPlayer>(white_team_vector);
        
        std::vector<std::shared_ptr<ChessPiece>> black_team_vector;
        
        black_team_vector.push_back(std::make_shared<Rook>(BoardPosition{ 'a', 8 }, main_drawable_creator->CreateDrawable()));
        
        black_team_vector.push_back(std::make_shared<Knight>(BoardPosition{ 'b', 8 }, main_drawable_creator->CreateDrawable()));
        
        black_team_vector.push_back(std::make_shared<Bishop>(BoardPosition{ 'c', 8 }, main_drawable_creator->CreateDrawable()));
        
        black_team_vector.push_back(std::make_shared<Queen>(BoardPosition{ 'd', 8 }, main_drawable_creator->CreateDrawable()));
        
        black_team_vector.push_back(std::make_shared<King>(BoardPosition{ 'e', 8 }, main_drawable_creator->CreateDrawable()));
        
        black_team_vector.push_back(std::make_shared<Bishop>(BoardPosition{ 'f', 8 }, main_drawable_creator->CreateDrawable()));
        
        black_team_vector.push_back(std::make_shared<Knight>(BoardPosition{ 'g', 8 }, main_drawable_creator->CreateDrawable()));
        
        black_team_vector.push_back(std::make_shared<Rook>(BoardPosition{ 'h', 8 }, main_drawable_creator->CreateDrawable()));
        
        // Black Pawns
        for (char file = 'a'; file <= 'h'; ++file) {
            auto pawn = std::make_shared<Pawn>(BoardPosition{ file, 7 }, main_drawable_creator->CreateDrawable());
            black_team_vector.push_back(pawn);
        }
        
        for (auto& piece : black_team_vector)
        {
            glm::vec3 piece_position =
                glm::vec3(m_PositionHelper->BoardToScreenPosition(piece->GetPiecePosition()), kChessPieceDepth);
        
            glm::vec2 scale = m_PositionHelper->SingleSquareSize();
        
            auto piece_drawable = piece->GetPieceDrawable().lock();
            m_SceneObjects.push_back(piece_drawable);
        
            piece_drawable->SetPosition(piece_position);
            piece_drawable->SetColor(glm::vec3(0));
            piece_drawable->SetDrawableTextureName(GetTextureNameByPieceType(piece->GetChessPieceType()));
        
        }
        auto black_team_player = std::make_shared<ChessPlayer>(black_team_vector);
        

        auto Bind_OnSelectedPieceChanged = [this](std::weak_ptr<ChessPiece> new_selected_piece)
            {
                this->OnSelectedPieceChanged(new_selected_piece);
            };
        auto Bind_OnSelectedPieceMoved = [this](std::shared_ptr<ChessPiece> new_selected_piece)
            {
                this->OnSelectedPieceMoved(new_selected_piece);
            };

        m_ChessGameController = 
            std::make_unique<ChessGameController>(white_team_player, black_team_player,
                Bind_OnSelectedPieceChanged, Bind_OnSelectedPieceMoved);
    }
}

void Chess_Game::DefaultChessScene::CreateSceneUI()
{
    if (auto application = m_Application.lock())
    {
        constexpr glm::vec2 kMenuButtonSize = glm::vec2{200,60.0f };
      
       m_PauseButton = 
           application->GetUIManager().CreateUIElement<Button>(
           glm::vec2(-70),glm::vec2(50));
      
       auto invert_visibility = [this]()
           {
               m_PauseMenuGroup->SetVisibility(!m_PauseMenuGroup->GetElementVisibility());
      
           };
       m_PauseButton->SetButtonCallback(invert_visibility);
      
       m_PauseButton->SetButtonCustomTexture(TextureName_kPauseButton);
       m_PauseButton->SetPositionPivot(PositionPivot_kTopRight);
       
       m_PauseMenuGroup = 
           application->GetUIManager().CreateUIElement<Panel>(
           glm::vec2(0),glm::vec2(400));
      
       m_PauseMenuGroup->SetVisibility(false);
       m_PauseMenuGroup->EnablePanelBackground(true);
      
    
       m_ResetButton =
           application->GetUIManager().CreateUIElement<Button>(
           glm::vec2(0), kMenuButtonSize);
      
       auto button_reset_callback_test = [this]() {
           if (auto application = m_Application.lock())
           {
      
               std::shared_ptr<DefaultChessScene> new_scene = std::make_shared<DefaultChessScene>(m_Application);
               application->SwitchCurrentApplicationScene(new_scene);
           }
           };
      
       m_ResetButton->SetButtonCallback(button_reset_callback_test);
      
       m_ResetButton->SetButtonCustomTexture(TextureName_kButton);
      
       m_ResetButton->SetElementDepth(.5);

       m_PauseMenuGroup->AddChildElement(m_ResetButton);
      
      
   
       m_MainMenuButton =
           application->GetUIManager().CreateUIElement<Button>(
           glm::vec2(0, 200.0f), kMenuButtonSize);
      
       auto to_main_menu_button_callback = [this]() {
           if (auto application = m_Application.lock())
           {
               std::shared_ptr<MainMenuScene> main_menu_scene = std::make_shared<MainMenuScene>(m_Application);
               application->SwitchCurrentApplicationScene(main_menu_scene);
           }
           };
      
      
       m_MainMenuButton->SetButtonCallback(to_main_menu_button_callback);

       m_MainMenuButton->SetElementDepth(.5);

       m_MainMenuButton->SetButtonCustomTexture(TextureName_kButton);
      
       m_PauseMenuGroup->AddChildElement(m_MainMenuButton);
      
       m_PauseMenuGroup->SetPanelCustomTexture(TextureName_kUIGroupBackground);
     
       m_ResumeButton = 
           application->GetUIManager().CreateUIElement<Button>(
               glm::vec2(0,-200.0f), kMenuButtonSize);
       
       m_ResumeButton->SetButtonCallback(invert_visibility);
       
       m_PauseMenuGroup->SetElementDepth(.5);

       m_ResumeButton->SetButtonCustomTexture(TextureName_kButton);
      
       m_PauseMenuGroup->AddChildElement(m_ResumeButton);

      //static std::shared_ptr<Panel> score_board_test_pane =
      //    application->GetUIManager().CreateUIElement<Panel>(glm::vec2(100.f, -100.0f),
      //        glm::vec2(100.0f));
      //score_board_test_pane->SetPositionPivot(PositionPivot_kTopLeft);
      //score_board_test_pane->EnablePanelBackground(true);
      //
      //static std::shared_ptr<Button> test_button_1 =
      //    application->GetUIManager().CreateUIElement<Button>(
      //        glm::vec2(-40,0), glm::vec2(30));
      //
      //test_button_1->SetButtonCustomTexture(TextureName_kPauseButton);
      //
      //static std::shared_ptr<Button> test_button_2 =
      //    application->GetUIManager().CreateUIElement<Button>(
      //        glm::vec2(40,0), glm::vec2(30));
      //
      //test_button_2->SetButtonCustomTexture(TextureName_kPauseButton);
      //
      //
      //score_board_test_pane->AddChildElement(test_button_1);
      //score_board_test_pane->AddChildElement(test_button_2);
      //
      //
      //
      //
      //float piece_selection_size = 30.f;
      //
      //test_pawn_selection_group = application->GetUIManager().CreateUIElement<Panel>(
      //    glm::vec2(0, 0), glm::vec2(150,40));
      //
      //test_pawn_selection_group->EnablePanelBackground(true);
      //
      //static std::shared_ptr<Button> queen_selection_button = 
      //    application->GetUIManager().CreateUIElement<Button>(
      //        glm::vec2((piece_selection_size + 100.0f), 0), glm::vec2(piece_selection_size));
      //queen_selection_button->SetButtonCustomTexture(TextureName_kQueen);
      //test_pawn_selection_group->AddChildElement(queen_selection_button);
      //
      //static std::shared_ptr<Button> rook_selection_button =
      //    application->GetUIManager().CreateUIElement<Button>(
      //        glm::vec2((piece_selection_size + 15.0f), 0), glm::vec2(piece_selection_size));
      //
      //rook_selection_button->SetButtonCustomTexture(TextureName_kRook);
      //test_pawn_selection_group->AddChildElement(rook_selection_button);
      //
      //static std::shared_ptr<Button> bishop_selection_button =
      //    application->GetUIManager().CreateUIElement<Button>(
      //        glm::vec2(0, 0), glm::vec2(piece_selection_size));
      //
      //bishop_selection_button->SetButtonCustomTexture(TextureName_kBishop);
      //test_pawn_selection_group->AddChildElement(bishop_selection_button);
      //
      //static std::shared_ptr<Button> knight_selection_button =
      //    application->GetUIManager().CreateUIElement<Button>(
      //        glm::vec2((piece_selection_size - 100.0f), 0), glm::vec2(piece_selection_size));
      //knight_selection_button->SetButtonCustomTexture(TextureName_kKnight);
      //
      //test_pawn_selection_group->AddChildElement(knight_selection_button);
      //
    }
}

void Chess_Game::DefaultChessScene::OnGameOver()
{
   
}


void Chess_Game::DefaultChessScene::DrawScene(std::shared_ptr<BatchRenderer> application_batch_renderer)
{
    if (auto application = m_Application.lock())
    {
        std::shared_ptr<AssetLoader> kApplicationAssets = application->GetAssetLoader();

        for (const auto& drawable_weak_ptr : m_SceneObjects)
        {
            if (auto drawable = drawable_weak_ptr.lock())
            {
                application_batch_renderer->PushTexturedQuad(drawable);
            }
        }

        if(m_SelectedPieceVisualIndicatorDrawable->IsDrawableEnabled())
            application_batch_renderer->PushTexturedQuad(m_SelectedPieceVisualIndicatorDrawable);

        application_batch_renderer->DrawTextureQuadBatch(application->GetApplicationProjection().GetMatrix());

        for (auto board_pos : m_SelectedPiecePossiblePositions)
        {
            glm::vec3 to_screen_pos =
                glm::vec3(m_PositionHelper->BoardToScreenPosition(board_pos), kPossibleMoveVisualDepth);
            application_batch_renderer->PushCircle(to_screen_pos, glm::vec2{15}, glm::vec3(0,0,1));
        }
        application_batch_renderer->DrawCircleBatch(application->GetApplicationProjection().GetMatrix());

    }
}

void Chess_Game::DefaultChessScene::OnUpdate()
{
    if (auto application = m_Application.lock())
    {
        if (m_IsGamePaused)
            return;

        if (application->GetMouseInputManager().IsMouseButtonPressed(MouseButton_kLeftMouseButton))
        {
            BoardPosition mouse_to_board_postion = GetMouseInputBoardPosition(application);
      
            m_ChessGameController->ProcessInput(mouse_to_board_postion);    
        }

        if (m_ChessGameController->IsGameOver())
        {
            OnGameOver();
        }
    }
}

Chess_Game::BoardPosition Chess_Game::DefaultChessScene::GetMouseInputBoardPosition(
    std::shared_ptr<Chess_Game::Application>& application)
{
    MousePos screen_coordinates = application->GetMouseInputManager().GetMousePositionUpperLeft();
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

        for (const auto& chess_board_piece : m_ChessGameController->GetAllBoardPieces())
        {
            if (auto chess_piece_drawable = chess_board_piece->GetPieceDrawable().lock())
            {
                glm::vec2 retrieved_pos = 
                    m_PositionHelper->BoardToScreenPosition(chess_board_piece->GetPiecePosition());

                glm::vec3 piece_position = glm::vec3(retrieved_pos.x, retrieved_pos.y, kChessPieceDepth);
                chess_piece_drawable->SetPosition(piece_position);
            }
        }

        glm::vec3 board_position = glm::vec3(0.0f, 0.0f, kBoardDepth);

        m_ChessBoard->SetPosition(board_position);
        m_ChessBoard->SetScale(m_PositionHelper->GetBoardSize());
    }
}

void Chess_Game::DefaultChessScene::OnSelectedPieceChanged(std::weak_ptr<ChessPiece> new_selected_piece)
{
    m_SelectedPiecePossiblePositions.clear();
    bool is_any_piece_selected = !new_selected_piece.expired();

    if (auto selected_piece = new_selected_piece.lock())
    {
        glm::vec3 piece_drawable_position = selected_piece->GetPieceDrawable().lock()->GetPosition();
        piece_drawable_position.z = kChessPieceVisualSelectionDepth;
        m_SelectedPieceVisualIndicatorDrawable->SetPosition(piece_drawable_position);
        m_SelectedPiecePossiblePositions = m_ChessGameController->GetSelectedPieceAllPossibleMoves();
    }
    m_SelectedPieceVisualIndicatorDrawable->EnableDrawable(is_any_piece_selected);
}

void Chess_Game::DefaultChessScene::OnSelectedPieceMoved(std::shared_ptr<ChessPiece> selected_piece)
{
   auto piece_drawable = selected_piece->GetPieceDrawable().lock();
   glm::vec2 new_position = m_PositionHelper->BoardToScreenPosition(selected_piece->GetPiecePosition());
   piece_drawable->SetPosition(glm::vec3(new_position, kChessPieceDepth));

}

