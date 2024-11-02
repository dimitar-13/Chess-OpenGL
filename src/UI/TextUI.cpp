#include "TextUI.h"
#include "UI/UIManager.h"

Chess_Game::TextUI::TextUI(std::weak_ptr<UIManager> ui_manager_ref,
    std::shared_ptr<DrawableCreator> drawable_creator, const glm::vec2& relative_offset,
    const glm::vec2& element_size)

    : Element(ui_manager_ref, drawable_creator, relative_offset, element_size)
{
    SetTextFont(ui_manager_ref.lock()->GetDefaultTextFont());
}

void Chess_Game::TextUI::Draw(BatchRenderer& renderer)
{
    if (!m_WidgetDrawable->m_IsEnabled)
        return;

    glm::vec3 final_text_pos = glm::vec3(this->GetScreenPos(),m_DepthLayer);

    renderer.PushText(m_TextToDraw, final_text_pos, m_WidgetDrawable->m_Scale, glm::vec3(1),
        m_TextFont);
}
