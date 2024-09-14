#include "Input.h"
#include "Logging/Logger.h"
bool Chess_Game::MouseInput::IsMouseButtonPressed(MouseButton_ button_to_check)const
{
    if (m_MouseButtonsHash.find(button_to_check) == m_MouseButtonsHash.end())
    {
        //CHESS_LOG_WARN("[Mouse Input] Was not able to find mouse button with enum code:'{0}'", button_to_check);
        return false;
    }
    InputAction_ current_action = m_MouseButtonsHash.at(button_to_check);

    return  current_action == InputAction_kPressed;
}

//Temporary solution
void Chess_Game::MouseInput::FlushInputPoll()
{
    for (auto& [key, value] : m_MouseButtonsHash)
    {
        value = InputAction_kReleased;
    }

}

void Chess_Game::MouseInput::OnEvent(const Event& e)
{
    if (e.GetEventType() == EventType_kMouseInputEvent)
    {
        OnMouseButtonInputEvent(dynamic_cast<const MouseButtonEvent&>(e));
    }
    else if (e.GetEventType() == EventType_kMousePositionChangedEvent)
    {
        OnMousePositionChangedEvent(dynamic_cast<const MousePositionChangedEvent&>(e));
    }
}

void Chess_Game::MouseInput::OnMousePositionChangedEvent(const MousePositionChangedEvent& e)
{
    m_CurrentMousePositionUpperLeft = e.GetMousePositionUpperLeft();
    m_CurrentMousePositionBottomLeft = e.GetMousePositionBottomLeft();
}

void Chess_Game::MouseInput::OnMouseButtonInputEvent(const MouseButtonEvent& e)
{
    MouseButton_ event_mouse_button = e.GetInputMouseButton();

    if (m_MouseButtonsHash.find(event_mouse_button) == m_MouseButtonsHash.end())
    {
        //CHESS_LOG_WARN("[Mouse Input] Was not able to find mouse button with enum code:'{0}'", event_mouse_button);
        return;
    }
    m_MouseButtonsHash.at(event_mouse_button) = e.GetInputType();
}
