#pragma once
#include "Core/Drawable.h"
#include "UI/UIElement.h"
namespace Chess_Game
{
    class UIManager;
    /**
     * @brief Class for representing a UI button.
     *
     * The class provides way of setting on click button callback using 'SetButtonCallback()'.
     * The class also provides a way of setting custom button texture by default is a white square.
     */
    class Button : public Element
    {
        friend class UIManager;
    private:
        /**
         * @brief Creates a 'Button' class and calls the 'Element' constructor.
         *
         * @param ui_manager_ref Weak ref to the UIManager.
         * @param drawable_creator Ref to drawable creator.
         * @param relative_offset Offset from element panel pivot point.
         * @param element_size Half Size of the element.
         */
        Button(std::weak_ptr<UIManager> ui_manager_ref,
            std::shared_ptr<DrawableCreator> drawable_creator,
            const glm::vec2& position,
            const glm::vec2& element_size)
            :Element(ui_manager_ref, drawable_creator,
                position, element_size)
        {}
        /**
         * @brief Called on button press.
         * 
         * The function overrides the'Element:OnElementPressed()'.
         * @note This can be extracted into its own interface.
         * 
         */
        void OnElementPressed()override
        {
            if (m_OnButtonClick != nullptr)
                m_OnButtonClick();
        }
    public:
        /**
         * @brief Changes the texture used for drawing the button.
         *
         * @param name_of_texture Texture enum, of type 'TextureName_', of the new texture to use.
         */
        void SetButtonCustomTexture(const std::string& name_of_texture) { m_WidgetDrawable->m_TextureName = name_of_texture; }
        /**
         * @brief Sets the button on click callback.
         *
         * @param callback The new callback to use.
         */
        void SetButtonOnClickCallback(const std::function<void()>& callback) { m_OnButtonClick = callback; }
    private:
        std::function<void()> m_OnButtonClick{}; ///< Button on click callback.
    };
 
}