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
        TextUI(std::weak_ptr<UIManager> ui_manager_ref,
            std::shared_ptr<DrawableCreator> drawable_creator,
            const glm::vec2& position,
            const glm::vec2& element_size); 
    public:
        void SetTextFont(std::shared_ptr<TextFont> font_to_set) {m_TextFont = font_to_set;}
        void SetText(const std::string& text) { m_Text = text;}
        void Draw(BatchRenderer& renderer)override;

    private:
        std::shared_ptr<TextFont> m_TextFont;
        std::string m_Text = "Test text.";
        // Inherited via Element
        void EnableDrawable(bool is_visible) override;
    };

}