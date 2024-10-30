#include "PawnPromotionUIManager.h"

Chess_Game::PawnPromotionSelectionUI::PawnPromotionSelectionUI(
    std::shared_ptr<UIManager> ui_manager):
    m_UIManager(ui_manager)
{

    auto queen_callback_bind = [this]()
        {
            m_OnUserSelection(ChessPieceType_kQueen);
        };
       
    auto bishop_callback_bind = [this]()
        {
            m_OnUserSelection(ChessPieceType_kBishop);
        };
      
    auto rook_callback_bind = [this]()
        {
            m_OnUserSelection(ChessPieceType_kRook);
        };

    auto knight_callback_bind = [this]()
        {
            m_OnUserSelection(ChessPieceType_kKnight);
        };

    float piece_selection_size = 30.f;
    
    m_PawnPromotionPanel = 
        m_UIManager->CreateUIElement<Panel>(glm::vec2(0, 0), glm::vec2(150,40)); 
    m_PawnPromotionPanel->EnableElement(false);
    
    m_QueenSelectionButton =  m_UIManager->CreateUIElement<Button>(
            glm::vec2(110.0f, 0), glm::vec2(piece_selection_size));
    m_QueenSelectionButton->SetButtonOnClickCallback(queen_callback_bind);
    m_QueenSelectionButton->SetElementDepth(.1f);
    m_PawnPromotionPanel->AddChildElement(m_QueenSelectionButton);

    m_RookSelectionButton = m_UIManager->CreateUIElement<Button>(
            glm::vec2(45.0f, 0), glm::vec2(piece_selection_size));  
    m_RookSelectionButton->SetButtonOnClickCallback(rook_callback_bind);
    m_RookSelectionButton->SetElementDepth(.1f);
    m_PawnPromotionPanel->AddChildElement(m_RookSelectionButton);

    m_BishopSelectionButton = m_UIManager->CreateUIElement<Button>(
            glm::vec2(-30.0f , 0), glm::vec2(piece_selection_size));  
    m_BishopSelectionButton->SetButtonOnClickCallback(bishop_callback_bind);
    m_BishopSelectionButton->SetElementDepth(.1f);
    m_PawnPromotionPanel->AddChildElement(m_BishopSelectionButton);
    
    m_KnightSelectionButton = m_UIManager->CreateUIElement<Button>(
            glm::vec2((piece_selection_size - 140.0f), 0), glm::vec2(piece_selection_size));
    m_KnightSelectionButton->SetButtonOnClickCallback(knight_callback_bind);
    m_KnightSelectionButton->SetElementDepth(.1f);
    m_PawnPromotionPanel->AddChildElement(m_KnightSelectionButton);
}


void Chess_Game::PawnPromotionSelectionUI::TogglePawnPromotionUI(bool is_enabled, bool is_white_team)
{
    m_QueenSelectionButton->SetButtonCustomTexture(is_white_team ? "chess_game_queen_w.png":
                                                                   "chess_game_queen_b.png");

    m_RookSelectionButton->SetButtonCustomTexture(is_white_team ? "chess_game_rook_w.png":
                                                                  "chess_game_rook_b.png");

    m_BishopSelectionButton->SetButtonCustomTexture(is_white_team ? "chess_game_bishop_w.png":
                                                                    "chess_game_bishop_b.png");

    m_KnightSelectionButton->SetButtonCustomTexture(is_white_team ? "chess_game_knight_w.png":
                                                                    "chess_game_knight_b.png");

    m_PawnPromotionPanel->EnableElement(is_enabled);

}
