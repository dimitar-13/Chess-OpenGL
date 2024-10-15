#include "D:/c++/OpenGl/Chess-OpenGL/build/CMakeFiles/Chess.dir/Debug/cmake_pch.hxx"
#include "Panel.h"
#include "Core/Chess_pch.h"

void Chess_Game::Panel::AddChildElement(std::weak_ptr<Element> child_to_add)
{
    if (auto shared_ptr_child = child_to_add.lock())
    {
        std::shared_ptr<Element> child_parent = shared_ptr_child->m_Parent;
        if(child_parent)
            m_Parent->RemoveChildElement(child_to_add);

        shared_ptr_child->m_Parent = std::dynamic_pointer_cast<Panel>(shared_from_this());
        shared_ptr_child->UpdateElement();

        m_Childs.push_back(child_to_add);
    }

}

void Chess_Game::Panel::RemoveChildElement(std::weak_ptr<Element> child_to_remove)
{
    
    if (auto child_to_remove_shared_ptr = child_to_remove.lock())
    {
        for (size_t i = 0; i < m_Childs.size(); i++)
        {
            if (auto child_shared = m_Childs[i].lock())
            {
                if (child_to_remove_shared_ptr->GetElementID() == child_shared->GetElementID())
                {
                    m_Childs.erase(m_Childs.begin() + i);
                    return;
                }
            }
        }
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
    //m_PanelDrawable = drawable_creator.CreateDrawable();
    glm::vec2 screen_pos = this->GetRelativePos();

   // m_PanelDrawable->SetPosition(glm::vec3(screen_pos, m_DepthLayer));

};


void Chess_Game::Panel::UpdatePanelChilds()
{
    for (auto& child_weak_ref : m_Childs)
    {
        if (auto child_shared_ref = child_weak_ref.lock())
        {
            child_shared_ref->OnElementChanged();
        }
    }
}
