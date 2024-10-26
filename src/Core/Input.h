#pragma once
#include "Core/EventListener.h"
#include "Core/Event/MouseEvents.h"
#include "Core/Chess_pch.h"

namespace Chess_Game
{
    /**
     * @brief Handles mouse input events and stores the current mouse state.
     *
     * The `MouseInput` class is responsible for managing and processing mouse input events such as
     * mouse position changes and button presses. It provides methods to query the current state of
     * the mouse, including the position and the status of mouse buttons.
     *
     * This class implements the 'Listener' interface so that it can listen for events.
     *
     */
    class MouseInput : public Listener
    {
    public:
        /**
         * @brief Checks if a specific mouse button is currently pressed.
         *
         * @param button_to_check The mouse button to check, represented by the `MouseButton_` enum.
         * @return `true` if the specified mouse button is currently pressed, otherwise `false`.
         */
        bool IsMouseButtonPressed(MouseButton_ button_to_check) const;
        /**
         * @brief Retrieves the current mouse position relative to the 
         * upper-left corner of the screen.
         *
         * @return A constant reference to a `MousePos` structure representing the 
         * current mouse position.
         */
        const MousePos& GetMousePositionUpperLeft() const { return m_CurrentMousePositionUpperLeft; }
        /**
         * @brief Retrieves the current mouse position relative to the 
         *        bottom-left corner of the screen.
         *
         * @return A constant reference to a `MousePos` structure representing 
         * the current mouse position.
         */
        const MousePos& GetMousePositionBottomLeft() const { return m_CurrentMousePositionBottomLeft; }
        /**
         * @brief Clears mouse input states to 'InputAction_kReleased'.
         *
         * The method resets every mouse button input state that is 'InputAction_kPressed' 
         * to 'InputAction_kReleased'.
         */
        void FlushInputPoll();
    private:
        /**
         * @brief Handles incoming events and processes relevant mouse events.
         *
         * This method overrides `Listener::OnEvent()` and is responsible for processing mouse-specific
         * events, such as mouse button presses and mouse position changes.
         *
         * @param e The event to process.
         */
        void OnEvent(const Event& e) override;

        /**
         * @brief Processes mouse position change events.
         *
         * Updates the internal mouse position whenever the mouse is moved. This function ensures that
         * both the upper-left and bottom-left representations of the mouse position are updated.
         *
         * @param e The `MousePositionChangedEvent` containing the new mouse position.
         */
        void OnMousePositionChangedEvent(const MousePositionChangedEvent& e);

        /**
         * @brief Processes mouse button input events.
         *
         * Updates the state of a mouse button (pressed or released) when a button event occurs. This
         * function updates the internal state of the `m_MouseButtonsHash` map based on the input action.
         *
         * @param e The `MouseButtonEvent` containing the button event data.
         */
        void OnMouseButtonInputEvent(const MouseButtonEvent& e);

    private:
        MousePos m_CurrentMousePositionUpperLeft{}; ///< Mouse position relative to the upper-left corner.
        MousePos m_CurrentMousePositionBottomLeft{}; ///< Mouse position relative to the bottom-left corner.

        /**
         * @brief A hash map that tracks the state of each mouse button.
         *
         * This map associates each mouse button (`MouseButton_` enum) with its current state, represented
         * by the `InputAction_` enum (e.g., pressed or released).
         */
        std::unordered_map<MouseButton_, InputAction_> m_MouseButtonsHash =
        {
            {MouseButton_kLeftMouseButton,    InputAction_kReleased},
            {MouseButton_kRightMouseButton,   InputAction_kReleased},
            {MouseButton_kScrollWheelButton,  InputAction_kReleased},
            {MouseButton_kX1,                 InputAction_kReleased},
            {MouseButton_kX2,                 InputAction_kReleased},
            {MouseButton_kAdditionalButton_1, InputAction_kReleased},
            {MouseButton_kAdditionalButton_2, InputAction_kReleased},
            {MouseButton_kAdditionalButton_3, InputAction_kReleased}
        };
    };


}
