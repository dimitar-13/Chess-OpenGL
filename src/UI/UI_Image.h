#pragma once
#include "Pch/Chess_pch.h"
#include "Core/Drawable.h"
#include "UI/UIElement.h"
namespace Chess_Game
{
    class UIManager;
    /**
     * @brief Class representing a UI based Image.
     *
     * The class is really basic providing a way to change the texture for the Image.
     */
    class Image : public Element
    {
        friend class UIManager;
    private:
        /**
         * @brief Creates a 'Image' class and calls the 'Element' constructor.
         *
         * @param ui_manager_ref Weak ref to the UIManager.
         * @param drawable_creator Ref to drawable creator.
         * @param relative_offset Offset from element panel pivot point.
         * @param element_size Half Size of the element.
         */
        Image(std::weak_ptr<UIManager> ui_manager_ref,
            std::shared_ptr<DrawableCreator> drawable_creator,
            const glm::vec2& relative_offset,
            const glm::vec2& element_size)
            :Element(ui_manager_ref, drawable_creator, relative_offset, element_size)
        {}       
    public:
        /**
         * @brief Changes the texture used for drawing the image.
         *
         * @param name_of_texture Texture enum, of type 'TextureName_', of the new texture to use.
         */
        void SetImageTexture(TextureName_ name_of_texture) { m_WidgetDrawable->m_TextureName = name_of_texture; }
    };
}