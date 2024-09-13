#include "D:/c++/OpenGl/Chess-OpenGL/build/CMakeFiles/Chess.dir/Debug/cmake_pch.hxx"
#include "UIElement.h"

void Chess_Game::UIElement::SetPosition(const glm::vec2& new_position)
{
    m_ElementWindowPos = new_position;
    //UpdateWindowPosition(m_);
}

void Chess_Game::UIElement::SetScale(const glm::vec2& new_scale)
{
    m_ElementScale = new_scale;

}

void Chess_Game::UIElement::SetMargin(const Margin& new_margin)
{
}

Chess_Game::UIElement::UIElement(size_t element_id, const Margin& element_margin,
    Size2D window_size, const glm::vec2& element_scale) :
    m_UIElementID(element_id), m_UIElementMargin(element_margin)
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

void Chess_Game::UIElement::UpdateWindowPosition(Size2D new_window_size)
{
    CalculateMarginPosition(new_window_size);
    m_UIDrawable->SetPosition(glm::vec3(m_ElementWindowPos,1));
    CalculateBoundingBox();
}

void Chess_Game::UIElement::CalculateMarginPosition(Size2D window_size)
{
    glm::vec2 middle_of_screen = { window_size.width / 2.0f, window_size.height / 2.0f };

    glm::vec2 position{};

    position.x = middle_of_screen.x + m_UIElementMargin.left - m_UIElementMargin.right;
    position.y = middle_of_screen.y + m_UIElementMargin.bottom - m_UIElementMargin.top;

    m_ElementWindowPos = position;

}

void Chess_Game::UIElement::CalculateBoundingBox()
{
    m_ElementBoundingBox.x = m_ElementWindowPos.x - m_ElementScale.x;
    m_ElementBoundingBox.y = m_ElementWindowPos.y - m_ElementScale.y;
    m_ElementBoundingBox.width = m_ElementWindowPos.x + m_ElementScale.x;
    m_ElementBoundingBox.height = m_ElementWindowPos.y + m_ElementScale.y;
}
