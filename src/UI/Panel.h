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
        void ResizeElement(glm::vec2 new_size)override;
        void OnElementPositionChange(glm::vec2 new_position)override;
        void SetVisibility(bool is_visible)override;
        void EnablePanelBackground(bool is_enabled) { m_HasBackground = is_enabled;this->m_UIDrawable->EnableDrawable(m_HasBackground && m_IsElementEnabled); }
    private:
        friend class UIManager;
        Panel(size_t element_id, std::weak_ptr<UIManager> ui_manager_ref,DrawableCreator& drawable_creator,
            const Margin& element_margin);
        void UpdatePanelChilds();
    private:
        std::vector<std::weak_ptr<Element>> m_Childs;
        bool m_HasBackground = false;
    };
}