#include "D:/c++/OpenGl/Chess-OpenGL/build/CMakeFiles/Chess.dir/Debug/cmake_pch.hxx"
#include "Panel.h"
#include "Core/Chess_pch.h"

void Chess_Game::Panel::AddChildElement(std::weak_ptr<Element> child_to_add)
{
    if (auto shared_ptr_child = child_to_add.lock())
    {
        std::shared_ptr<Panel> child_parent = shared_ptr_child->m_Parent;
        if(child_parent)
            child_parent->RemoveChildElement(child_to_add);

        shared_ptr_child->m_Parent = std::dynamic_pointer_cast<Panel>(shared_from_this());
        shared_ptr_child->UpdateElement();

        m_ChildDrawableIDHash.emplace(shared_ptr_child->GetElementID(),child_to_add);
    }

}

void Chess_Game::Panel::RemoveChildElement(std::weak_ptr<Element> child_to_remove)
{
    if (auto child_to_remove_shared_ptr = child_to_remove.lock())
    {
        RemoveChildElement(child_to_remove_shared_ptr->GetElementID());
    }
}

void Chess_Game::Panel::RemoveChildElement(size_t child_drawable_id)
{
    if (m_ChildDrawableIDHash.find(child_drawable_id) !=
        m_ChildDrawableIDHash.end())
    {
        m_ChildDrawableIDHash.erase(child_drawable_id);
    }
}

void Chess_Game::Panel::OnElementChanged()
{
    Element::OnElementChanged();

    UpdatePanelChilds();
}


Chess_Game::Panel::Panel(std::weak_ptr<UIManager> ui_manager_ref,
    std::shared_ptr<DrawableCreator> drawable_creator,
    const glm::vec2& element_pos, const glm::vec2& element_size):
    Element(ui_manager_ref, drawable_creator, element_pos, element_size)
{
};


void Chess_Game::Panel::UpdatePanelChilds()
{
    for (auto [key, child_weak_ref] : m_ChildDrawableIDHash)
    {
        if (auto child_shared_ref = child_weak_ref.lock())
        {
            child_shared_ref->OnElementChanged();
        }
    }
}
