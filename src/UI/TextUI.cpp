#include "D:/c++/OpenGl/Chess-OpenGL/build/CMakeFiles/Chess.dir/Debug/cmake_pch.hxx"
#include "TextUI.h"
#include "UI/UIManager.h"
Chess_Game::TextUI::TextUI(std::weak_ptr<UIManager> ui_manager_ref,
    std::shared_ptr<DrawableCreator> drawable_creator,
    const glm::vec2& position, const glm::vec2& element_size)
    : Element(ui_manager_ref, drawable_creator, position, element_size)
{
    //m_DrawableCreator = drawable_creator.shared_from_this();
    SetTextFont(ui_manager_ref.lock()->GetDefaultTextFont());

}
void Chess_Game::TextUI::Draw(BatchRenderer& renderer)
{
    if (!m_WidgetDrawable->m_IsEnabled)
        return;

    glm::vec3 final_text_pos = glm::vec3(this->GetScreenPos(),m_DepthLayer);

    renderer.PushText(m_Text, final_text_pos, m_WidgetDrawable->m_Scale, glm::vec3(1),
        m_TextFont);
}


void Chess_Game::TextUI::EnableDrawable(bool is_visible)
{
}
