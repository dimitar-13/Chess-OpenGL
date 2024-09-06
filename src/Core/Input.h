#pragma once
#include "Core/EventListener.h"
#include "Core/Event/MouseEvents.h"
#include <unordered_map>
namespace Chess_Game
{
    class MouseInput : public Listener
    {
    public:
        MouseInput() = default;
        bool IsMouseButtonPressed(MouseButton_ button_to_check);
        const MousePos& GetMousePosition()const { return m_CurrentMousePosition; }
    private:
        void OnEvent(const Event& e) override;
        void OnMousePositionChangedEvent(const MousePositionChangedEvent& e);
        void OnMouseButtonInputEvent(const MouseButtonEvent& e);
    private:
        MousePos m_CurrentMousePosition{};
        std::unordered_map<MouseButton_, InputAction_> m_MouseButtonsHash =
        {
            {MouseButton_kLeftMouseButton,    InputAction_kReleased},
            {MouseButton_kRightMouseButton,   InputAction_kReleased },
            {MouseButton_kScrollWheelButton,  InputAction_kReleased },
            {MouseButton_kX1,                 InputAction_kReleased },
            {MouseButton_kX2,                 InputAction_kReleased },
            {MouseButton_kAdditionalButton_1, InputAction_kReleased },
            {MouseButton_kAdditionalButton_2, InputAction_kReleased },
            {MouseButton_kAdditionalButton_3, InputAction_kReleased }
        };
    };

}
