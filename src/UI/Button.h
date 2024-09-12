#pragma once
#include "Core/Drawable.h"
#include "UI/UIElement.h"
namespace Chess_Game
{
    class UIManager;
    class Button : public UIElement
    {
        friend class UIManager;
    private:
        Button(size_t element_id,const glm::vec2& button_position, const glm::vec2& button_scale, float texture)
            :UIElement(element_id, button_position, button_scale, texture)
        {}
        
        std::weak_ptr<Drawable> GetButtonDrawable() { return button_drawable; }
    public:
        void SetButtonCallback(const std::function<void()>& callback) { m_OnButtonClick = callback; }
    private:
        std::shared_ptr<Drawable> button_drawable{};
        std::function<void()> m_OnButtonClick{};
    };
 
}