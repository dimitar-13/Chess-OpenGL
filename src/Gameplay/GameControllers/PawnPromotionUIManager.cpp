#include "D:/c++/OpenGl/Chess-OpenGL/build/CMakeFiles/Chess.dir/Debug/cmake_pch.hxx"
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
    
    m_PawnPromotionPanel = m_UIManager->CreateUIElement<Panel>(glm::vec2(0, 0), glm::vec2(150,40));
    
    m_PawnPromotionPanel->EnableElement(false);
    
    m_QueenSelectionButton =  m_UIManager->CreateUIElement<Button>(
            glm::vec2((piece_selection_size + 100.0f), 0), glm::vec2(piece_selection_size));

    m_QueenSelectionButton->SetButtonCustomTexture(TextureName_kQueen);
    m_QueenSelectionButton->SetButtonCallback(queen_callback_bind);

    m_PawnPromotionPanel->AddChildElement(m_QueenSelectionButton);


    m_RookSelectionButton = m_UIManager->CreateUIElement<Button>(
            glm::vec2((piece_selection_size + 15.0f), 0), glm::vec2(piece_selection_size));
    
    m_RookSelectionButton->SetButtonCustomTexture(TextureName_kRook);
    m_RookSelectionButton->SetButtonCallback(rook_callback_bind);

    m_PawnPromotionPanel->AddChildElement(m_RookSelectionButton);

    m_BishopSelectionButton = m_UIManager->CreateUIElement<Button>(
            glm::vec2(0, 0), glm::vec2(piece_selection_size));
    
    m_BishopSelectionButton->SetButtonCustomTexture(TextureName_kBishop);
    m_BishopSelectionButton->SetButtonCallback(bishop_callback_bind);
    m_PawnPromotionPanel->AddChildElement(m_BishopSelectionButton);
    
    m_KnightSelectionButton = m_UIManager->CreateUIElement<Button>(
            glm::vec2((piece_selection_size - 100.0f), 0), glm::vec2(piece_selection_size));

    m_KnightSelectionButton->SetButtonCustomTexture(TextureName_kKnight);
    m_KnightSelectionButton->SetButtonCallback(knight_callback_bind);

    m_PawnPromotionPanel->AddChildElement(m_KnightSelectionButton);

}

void Chess_Game::PawnPromotionSelectionUI::SetPromotionPosition()
{
}

void Chess_Game::PawnPromotionSelectionUI::TogglePawnPromotionUI(bool is_enabled)
{
    m_PawnPromotionPanel->EnableElement(is_enabled);

    //m_CurrentlySelectedType = ChessPieceType_kUnknown;
}

void Chess_Game::PawnPromotionSelectionUI::SwitchPromotionPieceTeamCol(bool is_white_team)
{
    glm::vec3 current_team_col = is_white_team ? glm::vec3(1) : glm::vec3(0);

    //m_QueenSelectionButton->GetDrawable()->SetColor(current_team_col);
    //m_BishopSelectionButton->GetDrawable()->SetColor(current_team_col);
    //m_RookSelectionButton->GetDrawable()->SetColor(current_team_col);
    //m_KnightSelectionButton->GetDrawable()->SetColor(current_team_col);
}
