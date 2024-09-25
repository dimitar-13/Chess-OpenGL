#include "D:/c++/OpenGl/Chess-OpenGL/build/CMakeFiles/Chess.dir/Debug/cmake_pch.hxx"
#include "UIElement.h"
#include "UIManager.h"
#include "Panel.h"
Chess_Game::Element::Element(size_t element_id,
    std::weak_ptr<UIManager> ui_manager_ref,
    DrawableCreator& drawable_creator,
    const glm::vec2& position,const glm::vec2& element_size):
    m_ElementID(element_id),
    m_ElementRelativePos(position),
    m_ElementSize(element_size),
    m_UIManager(ui_manager_ref)
{
    m_UIDrawable = drawable_creator.CreateDrawable();
    m_UIDrawable->SetColor(glm::vec3(1));
}

void Chess_Game::Element::ResizeElement(const glm::vec2& new_size)
{
    m_ElementSize = new_size;
    this->OnElementChanged();
}

void Chess_Game::Element::SetElementDepth(float depth_layer_value)
{
    m_DepthLayer = depth_layer_value;
    this->OnElementChanged();
}


glm::vec2 Chess_Game::Element::GetPivotPos(PositionPivot_ pivot)const
{
    glm::vec2 parent_pos{};
    if (m_Parent)
        parent_pos = m_Parent->GetPivotPos(m_ParentPosPivot);

    switch (pivot)
    {
    case Chess_Game::PositionPivot_kMiddle:
        parent_pos += m_ElementRelativePos;
        break;
    case Chess_Game::PositionPivot_kTopRight:
        parent_pos += m_ElementRelativePos + m_ElementSize;
        break;
    case Chess_Game::PositionPivot_kTopLeft:
        parent_pos += glm::vec2(m_ElementRelativePos.x - m_ElementSize.x,m_ElementRelativePos.y + m_ElementSize.y);
        break;
    case Chess_Game::PositionPivot_kBottomRight:
        parent_pos += glm::vec2(m_ElementRelativePos.x + m_ElementSize.x,m_ElementRelativePos.y - m_ElementSize.y);
        break;
    case Chess_Game::PositionPivot_kBottomLeft:
        parent_pos += glm::vec2(m_ElementRelativePos.x - m_ElementSize.x,m_ElementRelativePos.y - m_ElementSize.y);
        break;
    default:
        break;
    }

    return parent_pos;
}

glm::vec2 Chess_Game::Element::GetScreenPos()
{
    if(!m_Parent)
        return m_ElementRelativePos;

    glm::vec2 result = m_Parent->GetPivotPos(m_ParentPosPivot) + m_ElementRelativePos;
    return result;
}

void Chess_Game::Element::SetPositionPivot(PositionPivot_ new_pivot)
{
    m_ParentPosPivot = new_pivot;
    this->OnElementChanged();
}

void Chess_Game::Element::SetRelativePosition(const glm::vec2& new_pos)
{
    m_ElementRelativePos = new_pos;
    this->OnElementChanged();
}

void Chess_Game::Element::SetVisibility(bool is_visible)
{
    m_IsElementEnabled = is_visible;
    this->OnElementChanged();

    //m_UIDrawable->EnableDrawable(is_visible);
}

Chess_Game::Element::~Element()
{
    if (auto ui_manger = m_UIManager.lock())
    {
        ui_manger->RemoveWidget(m_ElementID);
    }
}

void Chess_Game::Element::CalculateElementBoundingBox(
    glm::vec2 screen_pos, glm::vec2 size)
{
    m_BoundingBox.x = screen_pos.x - size.x;
    m_BoundingBox.y = screen_pos.y - size.y;
    m_BoundingBox.width = screen_pos.x + size.x;
    m_BoundingBox.height = screen_pos.y + size.y;
}

void Chess_Game::Element::UpdateElement()
{
    bool parent_visibility = true;
    float absolute_depth_value = m_DepthLayer;
    if (m_Parent)
    {
        parent_visibility = m_Parent->GetElementVisibility();
        absolute_depth_value += m_Parent->GetElementDepth();
    }
    
    glm::vec2 element_screen_pos = GetScreenPos();

    m_UIDrawable->SetPosition(glm::vec3(element_screen_pos, absolute_depth_value));
    m_UIDrawable->SetScale(m_ElementSize);
    EnableDrawable(m_IsElementEnabled && parent_visibility);

    CalculateElementBoundingBox(element_screen_pos, m_ElementSize);

}
