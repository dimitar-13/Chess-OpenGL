#include "D:/c++/OpenGl/Chess-OpenGL/build/CMakeFiles/Chess.dir/Debug/cmake_pch.hxx"
#include "UIElement.h"
#include "UIManager.h"

void Chess_Game::UIElement::SetScale(const glm::vec2& new_scale)
{
    m_ElementScale = new_scale;
}

void Chess_Game::UIElement::SetMargin(const Margin& new_margin)
{
    m_UIElementMargin = new_margin;
    if (auto ui_manager = m_UIManager.lock())
    {
        CalculateMarginPosition(ui_manager->GetCurrentWindowSize());
        m_UIDrawable->SetPosition(glm::vec3(m_ElementWindowPos, 1));
        CalculateBoundingBox();
    }
}

Chess_Game::UIElement::UIElement(size_t element_id, std::weak_ptr<UIManager> ui_manager_ref,
    const Margin& element_margin,
    AnchorPoint_ element_margin_anchor_point,
    Size2D window_size, const glm::vec2& element_scale) :
    m_UIElementID(element_id), m_UIElementMargin(element_margin),
    m_ElementAnchorPoint(element_margin_anchor_point),
    m_UIManager(ui_manager_ref)
{

    CalculateMarginPosition(window_size);

    DrawableData data;
    data.color = glm::vec3(1);
    data.position = glm::vec3(m_ElementWindowPos, 1);
    data.scale = element_scale;
    m_ElementScale = element_scale;

    CalculateBoundingBox();

    m_UIDrawable = std::make_shared<Drawable>(data);
}

Chess_Game::UIElement::~UIElement()
{
    if (auto ui_manager = m_UIManager.lock())
    {
        ui_manager->RemoveWidget(m_UIElementID);
    }
}

void Chess_Game::UIElement::UpdateWindowPosition(Size2D new_window_size)
{
    CalculateMarginPosition(new_window_size);
    m_UIDrawable->SetPosition(glm::vec3(m_ElementWindowPos,1));
    CalculateBoundingBox();
}

void Chess_Game::UIElement::CalculateMarginPosition(Size2D window_size)
{
    glm::vec2 anchor_point{};

    switch (m_ElementAnchorPoint)
    {
    case Chess_Game::AnchorPoint_kMiddle:
        anchor_point = { window_size.width / 2.0f, window_size.height / 2.0f };
        break;
    case Chess_Game::AnchorPoint_kTopLeft:
        anchor_point = { 0,window_size.height };
        break;
    case Chess_Game::AnchorPoint_kBottomLeft:
        anchor_point = { window_size.width,0};
        break;
    case Chess_Game::AnchorPoint_kTopRight:
        anchor_point = { window_size.width,window_size.height };
        break;
    case Chess_Game::AnchorPoint_kBottomRight:
        anchor_point = {0,0 };
        break;
    default:
        break;
    }


    glm::vec2 position{};

    position.x = anchor_point.x + m_UIElementMargin.left - m_UIElementMargin.right;
    position.y = anchor_point.y + m_UIElementMargin.bottom - m_UIElementMargin.top;

    m_ElementWindowPos = position;

}

void Chess_Game::UIElement::CalculateBoundingBox()
{
    m_ElementBoundingBox.x = m_ElementWindowPos.x - m_ElementScale.x;
    m_ElementBoundingBox.y = m_ElementWindowPos.y - m_ElementScale.y;
    m_ElementBoundingBox.width = m_ElementWindowPos.x + m_ElementScale.x;
    m_ElementBoundingBox.height = m_ElementWindowPos.y + m_ElementScale.y;
}
