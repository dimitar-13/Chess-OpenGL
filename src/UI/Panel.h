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
        void OnElementChanged()override;
        void RemoveChildElement(std::weak_ptr<Element> child_to_remove);
        void RemoveChildElement(size_t child_drawable_id);
        ~Panel()
        {
            m_ChildDrawableIDHash.clear();
        }
    private:
        friend class UIManager;
        Panel(std::weak_ptr<UIManager> ui_manager_ref,
            std::shared_ptr<DrawableCreator> drawable_creator,
            const glm::vec2& element_pos,const glm::vec2& element_size);
        void UpdatePanelChilds();
    private:
        std::unordered_map<size_t, std::weak_ptr<Element>> m_ChildDrawableIDHash{};
        bool m_IsEnabled = true;
    };
}