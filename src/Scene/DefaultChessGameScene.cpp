#include "DefaultChessGameScene.h"
#include "Core/Application.h"
#include "Logging/Logger.h"
#include "Scene/MainMenuScene.h"
#include "Core/DrawableCreator.h"
#include <functional>
#include <chrono>
#include <thread>

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

        m_PauseMenuUIHelper = std::make_unique<PauseMenuUIHelper>(application);

        m_PawnPromotionUIManager = 
            std::make_unique<PawnPromotionSelectionUI>(application->UIManagerSharedPtrTest());

        m_PositionHelper =
            std::make_shared<ScreenPositionHelper>(application->GetApplicationProjection().GetProjectionSize());

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
            glm::vec3 piece_position = glm::vec3(m_PositionHelper->BoardToProjectionSpace(piece->GetPiecePosition()), kChessPieceDepth);
        
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
                glm::vec3(m_PositionHelper->BoardToProjectionSpace(piece->GetPiecePosition()), kChessPieceDepth);
        
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

        auto Bind_OnPawnPromotion = [this](bool is_white_team,BoardPosition piece_position)
            {
                return this->OnPawnPromotion(is_white_team, piece_position);
            };

        m_ChessGameController = 
            std::make_unique<ChessGameController>(white_team_player, black_team_player,
                Bind_OnSelectedPieceChanged, Bind_OnSelectedPieceMoved, Bind_OnPawnPromotion);
    }
}

void Chess_Game::DefaultChessScene::OnGameOver()
{
   
}

std::shared_ptr<Chess_Game::ChessPiece> Chess_Game::DefaultChessScene::OnPawnPromotion(
    bool is_white_team_pawn, BoardPosition new_pawn_pos)
{
    std::shared_ptr<ChessPiece> result{};
    constexpr glm::vec2 kOffset = glm::vec2(0,10.0f);
    std::condition_variable pawn_promotion_selection_condition_var;
    std::mutex pawn_promotion_mutex;
    TextureName_ texture_of_new_pice = TextureName_kWhiteTexture;
    ChessPieceType_ selected_type = ChessPieceType_kUnknown;


    if (auto application = m_Application.lock())
    {
        MousePos mouse_pos = application->GetMouseInputManager().GetMousePositionBottomLeft();
        glm::vec2 board_to_screen_pos(mouse_pos.x, mouse_pos.y);
        glm::vec2 screen_to_root_win_pos = 
            application->GetUIManager().ConvertScreenToRootWindowPos(board_to_screen_pos);

        screen_to_root_win_pos += kOffset;

        m_PawnPromotionUIManager->SetPosition(screen_to_root_win_pos);
        m_PawnPromotionUIManager->SwitchPromotionPieceTeamCol(is_white_team_pawn);
        m_PawnPromotionUIManager->TogglePawnPromotionUI(true);

        auto onPieceSelectionCallback = [&pawn_promotion_selection_condition_var, &pawn_promotion_mutex,&selected_type](ChessPieceType_ user_selected_type)
            {
                std::unique_lock<std::mutex> lock(pawn_promotion_mutex);
                selected_type = user_selected_type;
                pawn_promotion_selection_condition_var.notify_one();
            };

        m_PawnPromotionUIManager->SetCallback(onPieceSelectionCallback);

        std::unique_lock<std::mutex> lock(pawn_promotion_mutex);

        pawn_promotion_selection_condition_var.wait(lock,
            [&selected_type] {return selected_type != ChessPieceType_kUnknown; });


        std::shared_ptr<DrawableCreator> main_drawable_creator = application->GetDrawableCreator();

        glm::vec3 piece_position =
            glm::vec3(m_PositionHelper->BoardToProjectionSpace(new_pawn_pos), kChessPieceDepth);

       
;        switch (selected_type)
        {
        case ChessPieceType_kQueen:
            texture_of_new_pice = TextureName_kQueen;
            result = std::make_shared<Queen>(new_pawn_pos, main_drawable_creator->CreateDrawable());
            break;
        case ChessPieceType_kBishop:
            texture_of_new_pice = TextureName_kBishop;
            result = std::make_shared<Bishop>(new_pawn_pos, main_drawable_creator->CreateDrawable());
            break;
        case ChessPieceType_kKnight:
            texture_of_new_pice = TextureName_kKnight;
            result = std::make_shared<Knight>(new_pawn_pos, main_drawable_creator->CreateDrawable());
            break;
        case ChessPieceType_kRook:
            texture_of_new_pice = TextureName_kRook;
            result = std::make_shared<Rook>(new_pawn_pos, main_drawable_creator->CreateDrawable());
            break;
        default:
            break;
        }

        auto piece_drawable = result->GetPieceDrawable().lock();
        m_SceneObjects.push_back(piece_drawable);

        piece_drawable->SetPosition(piece_position);
        piece_drawable->SetColor(is_white_team_pawn ? glm::vec3(1) : glm::vec3(0));
        piece_drawable->SetDrawableTextureName(texture_of_new_pice);

        m_PawnPromotionUIManager->TogglePawnPromotionUI(false);
    }
    return result;

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
                glm::vec3(m_PositionHelper->BoardToProjectionSpace(board_pos), kPossibleMoveVisualDepth);
            application_batch_renderer->PushCircle(to_screen_pos, glm::vec2{15}, glm::vec3(0,0,1));
        }
        application_batch_renderer->DrawCircleBatch(application->GetApplicationProjection().GetMatrix());

    }
}

void Chess_Game::DefaultChessScene::OnUpdate()
{

    auto processInputBind = [this](BoardPosition position)
        {
            this->m_ChessGameController->ProcessInput(position);
        };

    if (auto application = m_Application.lock())
    {
        if (m_PauseMenuUIHelper->IsPauseMenuOn())
            return;

        if (application->GetMouseInputManager().IsMouseButtonPressed(MouseButton_kLeftMouseButton))
        {
            MousePos screen_coordinates = 
                application->GetMouseInputManager().GetMousePositionUpperLeft();

            glm::vec2 converted_screen_coords = 
                glm::vec2{ screen_coordinates.x,screen_coordinates.y };

            glm::vec2 orthographic_coordinates =
                application->GetApplicationProjection().FromScreenToOrthographicCoordinates(converted_screen_coords);

            BoardPosition mouse_to_board_postion = 
                m_PositionHelper->ScreenPositionToBoard(orthographic_coordinates);

            if (!m_GameProcessInputThread.valid() ||
                m_GameProcessInputThread.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
            {
                m_GameProcessInputThread = 
                    std::async(std::launch::async, processInputBind, mouse_to_board_postion);
            }
        }

        if (m_ChessGameController->IsGameOver())
        {
            OnGameOver();
        }
    }
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
                    m_PositionHelper->BoardToProjectionSpace(chess_board_piece->GetPiecePosition());

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
   glm::vec2 new_position = m_PositionHelper->BoardToProjectionSpace(selected_piece->GetPiecePosition());
   piece_drawable->SetPosition(glm::vec3(new_position, kChessPieceDepth));

}

