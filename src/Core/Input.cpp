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

void Chess_Game::MouseInput::FlushInputPoll()
{
    /**
     * Why this might be the best solution for now.
     *
     * The problem with input is that if a frame is rendered 60 times a second, each update may cause every system that
     * checks if the mouse is pressed to receive a "true" value 3-5 times. This is problematic because relying on a single
     * input check can lead to inconsistent behavior. One solution is to change the state after detecting a press, but this
     * approach is flawed since the mouse state will depend on the order of access rather than the state itself. Another
     * method is a timer-based approach where you set a timer after receiving the state, but this also has issues similar
     * to the previous suggestion. Flushing the input poll might be the easiest fix for now, as it ensures that a state like
     * "press" is registered only once per frame.
     * 
     * One consideration could be that you can create a array to hold all of the 'InputAction' states and iterate
     * then instead. To improve cache locality but this is a pre-mature optimization.
     */

    for (auto& [key, value] : m_MouseButtonsHash)
    {
        if(value == InputAction_kPressed)
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
