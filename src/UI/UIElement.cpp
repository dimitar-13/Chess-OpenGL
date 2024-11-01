#include "UIElement.h"
#include "UIManager.h"
#include "Panel.h"

Chess_Game::Element::Element(std::weak_ptr<UIManager> ui_manager_ref,
    std::shared_ptr<DrawableCreator> drawable_creator,
    const glm::vec2& relative_offset,const glm::vec2& element_size):
    m_ElementRelativePos(relative_offset),
    m_UIManager(ui_manager_ref)
{
    m_WidgetDrawable = drawable_creator->CreateDrawable();

    m_WidgetDrawable->m_Scale = element_size;
}

void Chess_Game::Element::ResizeElement(const glm::vec2& new_size)
{
    m_WidgetDrawable->m_Scale = new_size;
    this->OnElementChanged();
}

void Chess_Game::Element::SetElementDepth(float depth_layer_value)
{
    m_DepthLayer = depth_layer_value;
    this->OnElementChanged();
}

glm::vec2 Chess_Game::Element::GetParentPivotPos(PositionPivot_ pivot)const
{
    glm::vec2 parent_pos{};
    if (m_Parent)
        parent_pos = m_Parent->GetParentPivotPos(m_ParentPosPivot);

    switch (pivot)
    {
    case Chess_Game::PositionPivot_kMiddle:
        parent_pos += m_ElementRelativePos;
        break;
    case Chess_Game::PositionPivot_kTopRight:
        parent_pos += m_ElementRelativePos + m_WidgetDrawable->m_Scale;
        break;
    case Chess_Game::PositionPivot_kTopLeft:
        parent_pos += glm::vec2(m_ElementRelativePos.x - m_WidgetDrawable->m_Scale.x,m_ElementRelativePos.y + m_WidgetDrawable->m_Scale.y);
        break;
    case Chess_Game::PositionPivot_kBottomRight:
        parent_pos += glm::vec2(m_ElementRelativePos.x + m_WidgetDrawable->m_Scale.x,m_ElementRelativePos.y - m_WidgetDrawable->m_Scale.y);
        break;
    case Chess_Game::PositionPivot_kBottomLeft:
        parent_pos += glm::vec2(m_ElementRelativePos.x - m_WidgetDrawable->m_Scale.x,m_ElementRelativePos.y - m_WidgetDrawable->m_Scale.y);
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

    glm::vec2 result = m_Parent->GetParentPivotPos(m_ParentPosPivot) + m_ElementRelativePos;
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
    m_WidgetDrawable->m_IsEnabled = is_visible;
    this->OnElementChanged();
}

void Chess_Game::Element::EnableElement(bool is_enabled)
{
    m_IsElementEnabled = is_enabled;
    this->OnElementChanged();
}

Chess_Game::Element::~Element()
{
    if (m_Parent)
    {
        m_Parent->RemoveChildElement(this->GetElementID());
    }

    if (auto ui_manger = m_UIManager.lock())
    {
        ui_manger->RemoveWidget(this->GetElementID());
    }
}

void Chess_Game::Element::CalculateElementBoundingBox(
    const glm::vec2& screen_pos, const glm::vec2& element_size)
{
    m_BoundingBox.x = screen_pos.x - element_size.x;
    m_BoundingBox.y = screen_pos.y - element_size.y;
    m_BoundingBox.width = screen_pos.x + element_size.x;
    m_BoundingBox.height = screen_pos.y + element_size.y;
}

void Chess_Game::Element::Draw(BatchRenderer& batch_renderer)
{
    if (!(m_WidgetDrawable->m_IsEnabled && m_IsElementEnabled))
        return;

    //glm::vec3 final_widget_pos = glm::vec3(this->GetScreenPos(),m_DepthLayer);
    //m_WidgetDrawable->m_Position = final_widget_pos;

    batch_renderer.PushTexturedQuad(
        m_WidgetDrawable->GetDrawableID(), m_WidgetDrawable->m_Position, m_WidgetDrawable->m_Scale,
        glm::vec3(1), m_WidgetDrawable->m_TextureName);
}

void Chess_Game::Element::UpdateElement()
{
    float accumulated_depth = m_DepthLayer;
    if (m_Parent)
    {
        accumulated_depth += m_Parent->GetElementDepth();
        this->m_WidgetDrawable->m_IsEnabled = m_Parent->IsElementEnabled();
    }
    glm::vec2 element_screen_pos = GetScreenPos();
    m_WidgetDrawable->m_Position = glm::vec3(element_screen_pos, accumulated_depth);
    CalculateElementBoundingBox(element_screen_pos, m_WidgetDrawable->m_Scale);

}
