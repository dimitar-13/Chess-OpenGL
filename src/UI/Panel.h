#pragma once
#include "UIElement.h"
namespace Chess_Game
{

    class UIManager;

    class Panel : public Element
    {
    public:
        void SetPanelCustomTexture(TextureName_ name_of_texture) { m_UIDrawable->SetDrawableTextureName(name_of_texture); }
        void AddChildElement(std::weak_ptr<Element> child_to_add);
        void RemoveChildElement(std::weak_ptr<Element> child_to_remove);
        void OnElementChanged()override;
        void SetVisibility(bool is_enabled)override;
        void EnablePanelBackground(bool is_enabled) { m_HasBackground = is_enabled; EnableDrawable(m_IsElementEnabled); }
    private:
        friend class UIManager;
        Panel(size_t element_id, std::weak_ptr<UIManager> ui_manager_ref,
            DrawableCreator& drawable_creator,
            const glm::vec2& element_pos,const glm::vec2& element_size);
        void UpdatePanelChilds();
        void EnableDrawable(bool is_visible)override{ this->m_UIDrawable->EnableDrawable(m_HasBackground && is_visible); }
    private:
        std::vector<std::weak_ptr<Element>> m_Childs;
        bool m_HasBackground = false;

    };
}