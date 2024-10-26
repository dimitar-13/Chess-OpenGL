#pragma once
#include "Core/Drawable.h"
#include "UI/UIElement.h"
#include "Core/TextFont.h"
namespace Chess_Game
{
    class UIManager;
    /**
     * @brief Class representing UI text.
     *
     * The class provides ways of the user to change the:
     *  - Font beeing used to draw the text.
     *  - The text string that is beeing drawn.
     * Every 'TextUI' have a default font provided by the 'UIManager'.
     */
    class TextUI : public Element
    {
        friend class UIManager;
    private:
        /**
         * @brief Creates a 'TextUI' class and calls the 'Element' constructor.
         *
         * Sets the default font provided by the 'UIManager'.
         * 
         * @param ui_manager_ref Weak ref to the UIManager.
         * @param drawable_creator Ref to drawable creator.
         * @param relative_offset Offset from element panel pivot point.
         * @param element_size Half Size of the element.
         */
        TextUI(std::weak_ptr<UIManager> ui_manager_ref,
            std::shared_ptr<DrawableCreator> drawable_creator,
            const glm::vec2& relative_offset,
            const glm::vec2& element_size);
    public:
        /**
         * @brief Sets the text font to use.
         *
         * @param font_to_set New font to be used.
         */
        void SetTextFont(std::shared_ptr<TextFont> font_to_set) {m_TextFont = font_to_set;}
        /**
         * @brief Sets the text string to be drawn.
         *
         * @param text_to_be_drawn The new text to be drawn.
         */
        void SetText(const std::string& text_to_be_drawn) { m_TextToDraw = text_to_be_drawn;}
        /**
         * @brief Draws a text using the 'BatchRenderer' class.
         * 
         * Overrides the 'Element:Draw()' to use the 'BatchRenderer' draw text for drawing.
         * To see them drawing logic see 'BatchRenderer:PushText()' function.
         * 
         * @param renderer 'BatchRenderer' renderer to use for drawing.
         */
        void Draw(BatchRenderer& renderer)override;
    private:
        std::shared_ptr<TextFont> m_TextFont;   ///< Currently set font to use for drawing.
        std::string m_TextToDraw = "Text";            ///< Text string to be drawn.

    };

}