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
    /**
     * @brief Enum representing different mouse buttons.
     *
     * The 'MouseButton_' enum maps mouse button identifiers to their corresponding GLFW constants.
     * It covers standard buttons (left, right, middle) as well as additional buttons like X1, X2, and
     * extra buttons.
     *
     * - 'MouseButton_kLeftMouseButton': Left mouse button (GLFW_MOUSE_BUTTON_1).
     * - 'MouseButton_kRightMouseButton': Right mouse button (GLFW_MOUSE_BUTTON_2).
     * - 'MouseButton_kScrollWheelButton': Middle (scroll wheel) mouse button (GLFW_MOUSE_BUTTON_3).
     * - 'MouseButton_kX1': X1 (extra button) mouse button (GLFW_MOUSE_BUTTON_4).
     * - 'MouseButton_kX2': X2 (extra button) mouse button (GLFW_MOUSE_BUTTON_5).
     * - 'MouseButton_kAdditionalButton_1': Additional button 1 (GLFW_MOUSE_BUTTON_6).
     * - 'MouseButton_kAdditionalButton_2': Additional button 2 (GLFW_MOUSE_BUTTON_7).
     * - 'MouseButton_kAdditionalButton_3': Additional button 3 (GLFW_MOUSE_BUTTON_8).
     */
    enum MouseButton_
    {
        MouseButton_kLeftMouseButton = GLFW_MOUSE_BUTTON_1,    ///< Left mouse button.
        MouseButton_kRightMouseButton = GLFW_MOUSE_BUTTON_2,   ///< Right mouse button.
        MouseButton_kScrollWheelButton = GLFW_MOUSE_BUTTON_3,  ///< Middle (scroll wheel) mouse button.
        MouseButton_kX1 = GLFW_MOUSE_BUTTON_4,                 ///< X1 (extra) mouse button.
        MouseButton_kX2 = GLFW_MOUSE_BUTTON_5,                 ///< X2 (extra) mouse button.
        MouseButton_kAdditionalButton_1 = GLFW_MOUSE_BUTTON_6, ///< Additional mouse button 1.
        MouseButton_kAdditionalButton_2 = GLFW_MOUSE_BUTTON_7, ///< Additional mouse button 2.
        MouseButton_kAdditionalButton_3 = GLFW_MOUSE_BUTTON_8  ///< Additional mouse button 3.
    };
    /**
     * @brief Enum representing possible input actions for mouse buttons.
     *
     * The 'InputAction_' enum defines the possible states of a mouse button (pressed, released)
     * based on GLFW input actions. It also includes a special case for unknown or unused actions.
     *
     * - `InputAction_kUnknown`: The action is unknown or not specified for debug.
     * - `InputAction_kReleased`: The mouse button has been released (`GLFW_RELEASE`).
     * - `InputAction_kPressed`: The mouse button is pressed down (`GLFW_PRESS`).
     * - `InputAction_kHeld`: A reserved value for future use to represent a held-down button (currently not implemented).
     */
    enum InputAction_
    {
        InputAction_kUnknown,                    ///< Unknown or undefined input action for debug.
        InputAction_kReleased = GLFW_RELEASE,    ///< Mouse button released.
        InputAction_kPressed = GLFW_PRESS,       ///< Mouse button pressed.
        InputAction_kHeld                        ///< Reserved for future use (not currently implemented).
    };
    /**
     * @brief Event representing a mouse button press or release.
     *
     * The `MouseButtonEvent` class encapsulates information about a mouse button event,
     * including which button was pressed or released and what the action was (pressed or released).
     * It extends the base `Event` class and is used for handling mouse button input in event-driven systems.
     */
    class MouseButtonEvent : public Event
    {
    public:
        /**
         * @brief Constructs a 'MouseButtonEvent' with the specified mouse button and input action.
         *
         * @param button_input The mouse button that triggered the event (from 'MouseButton_' enum).
         * @param input_type The action performed on the button (pressed or released, from `InputAction_` enum).
         */
        MouseButtonEvent(MouseButton_ button_input, InputAction_ input_type) :
            m_MouseInput(button_input), m_InputType(input_type) {}
        /**
         * @brief Returns the type of event.
         *
         * @return `EventType_kMouseInputEvent`, indicating this is a mouse input event.
         */
        EventType_ GetEventType() const override { return EventType_kMouseInputEvent; }
        /**
         * @brief Gets the mouse button associated with the event.
         *
         * @return The mouse button (from 'MouseButton_' enum) that triggered the event.
         */
        MouseButton_ GetInputMouseButton() const { return m_MouseInput; }
        /**
         * @brief Gets the action (pressed or released) associated with the event.
         *
         * @return The input action (from 'InputAction_' enum) performed on the mouse button.
         */
        InputAction_ GetInputType() const { return m_InputType; }
    private:
        MouseButton_ m_MouseInput{}; ///< Mouse button that triggered the event.
        InputAction_ m_InputType{};  ///< Action (pressed/released) associated with the event.
    };
    /**
     * @brief Event representing a change in the mouse position.
     *
     * The 'MousePositionChangedEvent' class encapsulates information about changes in the mouse position.
     * It stores the new mouse coordinates relative to both the upper-left and bottom-left corners of the screen.
     */
    class MousePositionChangedEvent : public Event
    {
    public:
        /**
         * @brief Constructs a `MousePositionChangedEvent` with the specified mouse positions.
         *
         * @param new_mouse_position_upper_left The new mouse position relative to the upper-left corner of the screen.
         * @param new_mouse_position_lower_left The new mouse position relative to the bottom-left corner of the screen.
         */
        MousePositionChangedEvent(const MousePos& new_mouse_position_upper_left, const MousePos& new_mouse_position_lower_left) :
            m_NewMousePositionUpperLeft(new_mouse_position_upper_left),
            m_NewMousePositionBottomLeft(new_mouse_position_lower_left) {}
        /**
         * @brief Returns the type of event.
         *
         * @return `EventType_kMousePositionChangedEvent`, indicating this is a mouse position change event.
         */
        EventType_ GetEventType() const override { return EventType_kMousePositionChangedEvent; }

        /**
         * @brief Gets the new mouse position relative to the upper-left corner of the screen.
         *
         * @return A constant reference to the `MousePos` representing the new position.
         */
        const MousePos& GetMousePositionUpperLeft() const { return m_NewMousePositionUpperLeft; }
        /**
         * @brief Gets the new mouse position relative to the bottom-left corner of the screen.
         *
         * @return A constant reference to the `MousePos` representing the new position.
         */
        const MousePos& GetMousePositionBottomLeft() const { return m_NewMousePositionBottomLeft; }

    private:
        MousePos m_NewMousePositionUpperLeft{};  ///< New mouse position relative to the upper-left corner.
        MousePos m_NewMousePositionBottomLeft{}; ///< New mouse position relative to the bottom-left corner.
    };

}
