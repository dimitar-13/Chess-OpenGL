#pragma once
#include "Core/Drawable.h"
namespace Chess_Game
{
    class UIElement
    {
    public:
        std::weak_ptr<Drawable> GetDrawable() { return m_UIDrawable; }
    protected:
        UIElement(size_t element_id, const glm::vec2& button_position,
            const glm::vec2& button_scale, float texture) :
            m_UIElementID(element_id)
        {
            //m_UIDrawable = std::make_shared<Drawable>();
        }
        virtual void SetVisibility(bool is_visible) { m_IsVisible = is_visible; }
    protected:
        std::shared_ptr<Drawable> m_UIDrawable{};
        size_t m_UIElementID{};
        bool m_IsVisible = true;
    };
}