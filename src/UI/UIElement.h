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
            const glm::vec2& button_scale, TextureName_ texture) :
            m_UIElementID(element_id)
        {
            DrawableData data;
            data.color = glm::vec3(1);
            data.position = glm::vec3(button_position,1);
            data.scale = button_scale;
            data.texture_name = texture;
            m_UIDrawable = std::make_shared<Drawable>(data);
        }
        virtual void SetVisibility(bool is_visible) { m_IsVisible = is_visible; }
    protected:
        std::shared_ptr<Drawable> m_UIDrawable{};
        size_t m_UIElementID{};
        bool m_IsVisible = true;
    };
}