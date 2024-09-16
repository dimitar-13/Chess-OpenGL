#pragma once
#include "Core/Drawable.h"
#include "UI/UIElement.h"
namespace Chess_Game
{
    class UIManager;
    class Image : public UIElement
    {
        friend class UIManager;
    private:
        Image(size_t element_id, std::weak_ptr<UIManager> ui_manager_ref,
            DrawableCreator& drawable_creator,const Margin& button_margin,
            AnchorPoint_ element_margin_anchor_point,
            Size2D window_size, const glm::vec2& image_scale)
            :UIElement(element_id, ui_manager_ref, drawable_creator, button_margin, element_margin_anchor_point, window_size, image_scale)
        {}

    public:
        void SetImageTexture(TextureName_ name_of_texture) { m_UIDrawable->SetDrawableTextureName(name_of_texture); }
    };
}