#pragma once
#include "UIElement.h"
namespace Chess_Game
{

    class UIManager;

    class Panel : public Element
    {
    public:
        void SetPanelCustomTexture(TextureName_ name_of_texture) { m_WidgetDrawable->m_TextureName = name_of_texture; }
        void AddChildElement(std::weak_ptr<Element> child_to_add);
        void RemoveChildElement(std::weak_ptr<Element> child_to_remove);
        void OnElementChanged()override;
    private:
        friend class UIManager;
        Panel(std::weak_ptr<UIManager> ui_manager_ref,
            std::shared_ptr<DrawableCreator> drawable_creator,
            const glm::vec2& element_pos,const glm::vec2& element_size);
        void UpdatePanelChilds();
    private:
        std::vector<std::weak_ptr<Element>> m_Childs;
        bool m_IsEnabled = true;
    };
}