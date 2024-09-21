#pragma once
#include "UIElement.h"
namespace Chess_Game
{
    class UIManager;

    class Panel : public Element
    {
    public:

        void AddChildElement(std::weak_ptr<Element> child_to_add);
        void RemoveChildElement(std::weak_ptr<Element> child_to_remove);
        void ResizeElement(Size2D new_size) override;
        void OnElementPositionChange(glm::vec2 new_position)override;
    private:
        friend class UIManager;

        Panel(size_t element_id, std::weak_ptr<UIManager> ui_manager_ref,
            DrawableCreator& drawable_creator,
            const Margin& element_margin,
            const glm::vec2& element_scale) :
            Element(element_id, ui_manager_ref,
                drawable_creator,
                element_margin,
                element_scale)
        {};
        void UpdatePanelChilds();
    private:
        std::vector<std::weak_ptr<Element>> m_Childs;
    };
}