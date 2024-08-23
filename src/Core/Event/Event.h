#pragma once
#include "Core/ApplicationData.h"

namespace Chess_Game
{
    enum EventType_
    {
        EventType_kUnknown,
        EventType_kWindowResize,
        EventType_kMouseButtonEvent,
        EventType_kWindowClosed
    };

    class Event
    {
    public:
        virtual EventType_ GetEventType()const = 0;
    };



    class MouseButtonEvent : public Event
    {
    public:
        EventType_ GetEventType()const override { return EventType_kMouseButtonEvent; }

    private:

    };
    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(Size2D new_window_size):
            m_eventWindowSize(new_window_size)
        {}
        Size2D GetWindowSize()const { return m_eventWindowSize; }
        const Size2D& GetWindowSizeRef()const { return m_eventWindowSize; }
        EventType_ GetEventType()const override { return EventType_kWindowResize; }
    private:
        Size2D m_eventWindowSize{};
    };

    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;
        EventType_ GetEventType()const override { return EventType_kWindowClosed; }
    };
}