#pragma once
#include "Core/Drawable.h"
#include "UI/UIElement.h"
namespace Chess_Game
{
    class UIManager;
    class Image : public Element
    {
        friend class UIManager;
    private:
        Image(size_t element_id, std::weak_ptr<UIManager> ui_manager_ref,
            DrawableCreator& drawable_creator,
            const glm::vec2& position,
            const glm::vec2& element_size)
            :Element(element_id,ui_manager_ref,
                drawable_creator,
                position, element_size)
        {}       

    public:
        void SetImageTexture(TextureName_ name_of_texture) { m_UIDrawable->SetDrawableTextureName(name_of_texture); }
    };
}