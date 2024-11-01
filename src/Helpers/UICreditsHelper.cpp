#include "UICreditsHelper.h"

Chess_Game::UICreditsHelper::UICreditsHelper(std::shared_ptr<UIManager> ui_manager)
{
    m_CreditUIPanel = ui_manager->CreateUIElement<Panel>(glm::vec2(0), glm::vec2(500));
    m_CreditUIPanel->SetElementDepth(.4);
    m_CreditUIPanel->SetVisibility(true);
    m_CreditUIPanel->EnableElement(false);
    m_CreditUIPanel->SetPanelCustomTexture("chess_game_panel_background.png");

    m_GameUICredit = ui_manager->CreateUIElement<TextUI>(glm::vec2(70.0f,-200.0), glm::vec3(.7));
    m_GameUICredit->SetText("UI textures\n Author: tiopalada \n OpenGameArt name: Tiny RPG - Dragon Regalia GUI");
    m_GameUICredit->SetPositionPivot(PositionPivot_kTopLeft);
    m_GameUICredit->SetElementDepth(0.1);
    m_CreditUIPanel->AddChildElement(m_GameUICredit);
     
    m_GameChessPiecesCredit = ui_manager->CreateUIElement<TextUI>(glm::vec2(70.0f, -350.0), glm::vec3(.7));
    m_GameChessPiecesCredit->SetText("Chess pieces textures\n Author: drummyfish \n OpenGameArt name: Chess pieces and a board");
    m_GameChessPiecesCredit->SetPositionPivot(PositionPivot_kTopLeft);
    m_GameChessPiecesCredit->SetElementDepth(0.1);
    m_CreditUIPanel->AddChildElement(m_GameChessPiecesCredit);

    m_CloseButton = ui_manager->CreateUIElement<Button>(glm::vec2(-130,-160), glm::vec2(40));
    m_CloseButton->SetPositionPivot(PositionPivot_kTopRight);
    m_CloseButton->SetElementDepth(0.1);
    m_CloseButton->SetButtonCustomTexture("chess_game_close_button.png");
    m_CloseButton->SetButtonOnClickCallback([this] {EnableUICreditUI(false); });
    m_CreditUIPanel->AddChildElement(m_CloseButton);
}

void Chess_Game::UICreditsHelper::EnableUICreditUI(bool is_enabled)
{
    m_CreditUIPanel->EnableElement(is_enabled);

}
