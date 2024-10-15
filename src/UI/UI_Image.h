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
        Image(std::weak_ptr<UIManager> ui_manager_ref,
            std::shared_ptr<DrawableCreator> drawable_creator,
            const glm::vec2& position,
            const glm::vec2& element_size)
            :Element(ui_manager_ref, drawable_creator, position, element_size)
        {}       

    public:
        void SetImageTexture(TextureName_ name_of_texture) { m_WidgetDrawable->m_TextureName = name_of_texture; }

    };
}