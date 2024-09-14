#pragma once
#include "Event.h"
#include <GLFW/glfw3.h>

namespace Chess_Game
{
    struct MousePos
    {
        double x{};
        double y{};
    };
    enum MouseButton_
    {
        MouseButton_kLeftMouseButton = GLFW_MOUSE_BUTTON_1,
        MouseButton_kRightMouseButton = GLFW_MOUSE_BUTTON_2,
        MouseButton_kScrollWheelButton = GLFW_MOUSE_BUTTON_3,
        MouseButton_kX1 = GLFW_MOUSE_BUTTON_4,
        MouseButton_kX2 = GLFW_MOUSE_BUTTON_5,
        MouseButton_kAdditionalButton_1 = GLFW_MOUSE_BUTTON_6,
        MouseButton_kAdditionalButton_2 = GLFW_MOUSE_BUTTON_7,
        MouseButton_kAdditionalButton_3 = GLFW_MOUSE_BUTTON_8
    };
    enum InputAction_
    {
        InputAction_kUnknown,
        InputAction_kReleased = GLFW_RELEASE,
        InputAction_kPressed = GLFW_PRESS,
    };

    class MouseButtonEvent : public Event
    {
    public:
        MouseButtonEvent(MouseButton_ button_input, InputAction_ input_type) :
            m_MouseInput(button_input), m_InputType(input_type) {}
        EventType_ GetEventType()const override { return EventType_kMouseInputEvent; }
        MouseButton_ GetInputMouseButton()const { return m_MouseInput; }
        InputAction_ GetInputType()const { return m_InputType; }
    private:
        MouseButton_ m_MouseInput{};
        InputAction_ m_InputType{};
    };

    class MousePositionChangedEvent : public Event
    {
    public:
        MousePositionChangedEvent(const MousePos& new_mouse_position_upper_left,
            const MousePos& new_mouse_position_lower_left) :
            m_NewMousePositionUpperLeft(new_mouse_position_upper_left),
            m_NewMousePositionBottomLeft(new_mouse_position_lower_left)
        {}
        EventType_ GetEventType()const override { return EventType_kMousePositionChangedEvent; }
        const MousePos& GetMousePositionUpperLeft()const { return m_NewMousePositionUpperLeft; }
        const MousePos& GetMousePositionBottomLeft()const { return m_NewMousePositionBottomLeft; }

    private:
        MousePos m_NewMousePositionUpperLeft{};
        MousePos m_NewMousePositionBottomLeft{};

    };
}
