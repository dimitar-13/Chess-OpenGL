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
        Button(size_t element_id, const Margin& button_margin, Size2D window_size, const glm::vec2& button_scale)
            :UIElement(element_id, button_margin, window_size, button_scale)
        {}
        void OnWidgetPressed()override
        {
            if (m_OnButtonClick != nullptr)
                m_OnButtonClick();
        }
    public:
        void SetButtonCustomTexture(TextureName_ name_of_texture) { m_UIDrawable->SetDrawableTextureName(name_of_texture); }
        void SetButtonCallback(const std::function<void()>& callback) { m_OnButtonClick = callback; }
    private:
        std::function<void()> m_OnButtonClick{};
    };
 
}