#include "D:/c++/OpenGl/Chess-OpenGL/build/CMakeFiles/Chess.dir/Debug/cmake_pch.hxx"
#include "UI_Group.h"

//void Chess_Game::UI_Group::ResizeGroup(Size2D group_size)
//{
//    m_GroupSize = group_size;
//
//    RecalculateGroupElementsPosition();
//}
//
//void Chess_Game::UI_Group::ChangeGroupPosition(glm::vec2 new_group_start_position)
//{
//    m_GroupPosition = new_group_start_position;
//
//    RecalculateGroupElementsPosition();
//}
//void Chess_Game::UI_Group::RecalculateGroupElementsPosition()
//{
//    for (auto& weak_ref : m_GroupElements)
//    {
//        if (auto shared_ref = weak_ref.lock())
//        {
//            CalculateElementPosition(shared_ref);
//        }
//    }
//}
//
//void Chess_Game::UI_Group::AddUIElement(std::weak_ptr<UIElement> element)
//{
//    if (auto shared_ptr = element.lock())
//    {
//        m_GroupElements.push_back(element);
//        CalculateElementPosition(shared_ptr);
//        shared_ptr->ChangeElementGroup(shared_from_this());
//   }
//   
//
//}
//
//void Chess_Game::UI_Group::CalculateElementPosition(std::shared_ptr<UIElement> new_element)
//{
//    glm::vec2 anchor_point{};
//
//    switch (new_element->GetElementAnchorPoint())
//    {
//    case Chess_Game::AnchorPoint_kMiddle:
//        anchor_point = m_GroupPosition + glm::vec2{ m_GroupSize.width / 2.0f,m_GroupSize.height / 2.0f};
//        break;
//    case Chess_Game::AnchorPoint_kTopLeft:
//        anchor_point = { m_GroupPosition.x,m_GroupSize.height };
//        break;
//    case Chess_Game::AnchorPoint_kBottomLeft:
//        anchor_point = m_GroupPosition;
//        break;
//    case Chess_Game::AnchorPoint_kTopRight:
//        anchor_point = { m_GroupSize.width,m_GroupSize.height };
//        break;
//    case Chess_Game::AnchorPoint_kBottomRight:
//        anchor_point = { m_GroupSize.width ,m_GroupPosition.y };
//        break;
//    default:
//        break;
//    }
//
//    const Margin& element_margin = new_element->GetMargin();
//
//    glm::vec2 position{};
//
//    position.x = anchor_point.x + element_margin.left - element_margin.right;
//    position.y = anchor_point.y + element_margin.bottom - element_margin.top;
//
//    new_element->SetPosition(position);
//}

