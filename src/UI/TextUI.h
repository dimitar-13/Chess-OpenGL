#pragma once
#include "Core/Drawable.h"
#include "UI/UIElement.h"
#include "Core/TextFont.h"
namespace Chess_Game
{
    class UIManager;
    class TextUI : public Element
    {
        friend class UIManager;
    private:
        TextUI(size_t element_id, std::weak_ptr<UIManager> ui_manager_ref,
            DrawableCreator& drawable_creator, const glm::vec2& position,
            const glm::vec2& element_size)
            :Element(element_id, ui_manager_ref,
                drawable_creator,
                position, element_size)
        {}
    public:
        void SetTextFont(std::shared_ptr<TextFront> font_to_set) {m_TextFont = font_to_set;}
        void SetText(const std::string& text) { m_Text = text; }
        void Draw(BatchRenderer& renderer)override;
    private:
        std::shared_ptr<TextFront> m_TextFont;
        glm::vec2 m_TextScale = glm::vec2(1.0f);
        std::string m_Text = "Test text.";
    };

}