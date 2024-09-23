#pragma once
#include "Core/Drawable.h"
#include "UI/UIElement.h"
namespace Chess_Game
{
    class UIManager;
    class Button : public Element
    {
        friend class UIManager;
    private:
        Button(size_t element_id, std::weak_ptr<UIManager> ui_manager_ref,
            DrawableCreator& drawable_creator,
            const Margin& element_margin)
            :Element(element_id,ui_manager_ref,
                drawable_creator,
                element_margin)
        {}
        void OnElementPressed()override
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