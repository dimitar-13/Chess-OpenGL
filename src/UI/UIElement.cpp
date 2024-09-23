#include "D:/c++/OpenGl/Chess-OpenGL/build/CMakeFiles/Chess.dir/Debug/cmake_pch.hxx"
#include "UIElement.h"
#include "UIManager.h"

Chess_Game::Element::Element(size_t element_id,
    std::weak_ptr<UIManager> ui_manager_ref,
    DrawableCreator& drawable_creator,
    const Margin& element_margin):
    m_ElementID(element_id), m_ElementMargin(element_margin),
    m_UIManager(ui_manager_ref)
{
    CalculateElementScreenPosition();
    CalculateElementBoundingBox();

    m_UIDrawable = drawable_creator.CreateDrawable();
    m_UIDrawable->SetColor(glm::vec3(1));
    m_UIDrawable->SetScale(m_CurrentElementSize);
    m_UIDrawable->SetPosition(glm::vec3(m_ElementPos, 1));
}

void Chess_Game::Element::ResizeElement(glm::vec2 new_size)
{
    m_CurrentElementSize = new_size;

    CalculateElementScreenPosition();

    CalculateElementBoundingBox();

}

void Chess_Game::Element::SetMargin(const Margin& new_margin)
{
    m_ElementMargin = new_margin;

    CalculateElementScreenPosition();
    CalculateElementBoundingBox();
}

void Chess_Game::Element::SetVisibility(bool is_visible)
{
    m_IsElementEnabled = is_visible;
    m_UIDrawable->EnableDrawable(is_visible);
}

Chess_Game::Element::~Element()
{
}

void Chess_Game::Element::OnParentSizeChanged()
{
    CalculateElementScreenPosition();
    CalculateElementBoundingBox();
}

void Chess_Game::Element::OnElementPositionChange(glm::vec2 new_position)
{
    m_ElementPos = new_position;

}

void Chess_Game::Element::CalculateElementScreenPosition()
{
    if (!m_Parent)
        return;

    glm::vec2 parent_size = m_Parent->GetElementSize();
    glm::vec2 parent_pos = m_Parent->m_ElementPos;

    float left_margin = m_ElementMargin.left * parent_size.x;
    float right_margin = m_ElementMargin.right * parent_size.x;
    float top_margin = m_ElementMargin.top * parent_size.y;
    float bottom_margin = m_ElementMargin.bottom * parent_size.y;

    glm::vec2 final_child_pos;
    final_child_pos.x = parent_pos.x + left_margin; 
    final_child_pos.y = parent_pos.y + bottom_margin;
    
    glm::vec2 available_size;
    available_size.x = parent_size.x - (left_margin + right_margin);
    available_size.y = parent_size.y - (top_margin + bottom_margin);

    m_ElementPos = final_child_pos;

    float widget_aspect = available_size.x / available_size.y;
    float smallest_size = std::min(available_size.x, available_size.y);

    m_CurrentElementSize = glm::vec2(available_size.y);

    //m_UIDrawable->SetScale(glm::vec2(m_ElementSize.width, m_ElementSize.height));
    UpdateDrawable();
    OnElementPositionChange(final_child_pos);
    CalculateElementBoundingBox();
}

void Chess_Game::Element::CalculateElementBoundingBox()
{
    m_BoundingBox.x = m_ElementPos.x;
    m_BoundingBox.y = m_ElementPos.y;
    m_BoundingBox.width = m_ElementPos.x + m_CurrentElementSize.x;
    m_BoundingBox.height = m_ElementPos.y + m_CurrentElementSize.y;
}

void Chess_Game::Element::UpdateDrawable()
{
    glm::vec2 half_size = m_CurrentElementSize / 2.f;

    m_UIDrawable->SetPosition(glm::vec3(m_ElementPos + half_size, 1));
    m_UIDrawable->SetScale(half_size);

}
