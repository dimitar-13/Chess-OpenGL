#pragma once
#include "Core/ApplicationData.h"
namespace Chess_Game
{
    /**
     * @brief Enum representing different types of events in the system.
     *
     * The 'EventType_' enum defines a set of event types that can occur within the application.
     * Each event type corresponds to a specific action or state change, such as window resizing,
     * mouse input, or window closure.
     *
     * - 'EventType_kUnknown': An undefined or unknown event type.
     * - 'EventType_kWindowResize': Indicates a window resize event.
     * - 'EventType_kMouseInputEvent': Represents a mouse button press or release event.
     * - 'EventType_kMousePositionChangedEvent': Indicates a change in the mouse position.
     * - 'EventType_kWindowClosed': Represents a window close event.
     */
    enum EventType_
    {
        EventType_kUnknown,                    ///< Unknown or undefined event type.
        EventType_kWindowResize,               ///< Window resize event.
        EventType_kMouseInputEvent,            ///< Mouse button input event.
        EventType_kMousePositionChangedEvent,  ///< Mouse position change event.
        EventType_kWindowClosed                ///< Window close event.
    };


    /**
     * @brief Base class for all event types in the application.
     *
     * The 'Event' class serves as the base class for all events in the application.
     * It provides an interface for getting the event type, which must be implemented
     * by all derived classes.
     *
     * - `GetEventType()const`: Pure virtual function that must be implemented by derived classes to return the event type.
     * @note The reason `GetEventType()const`is pure virtual is for child classes to be considered polymorphic.
     */
    class Event
    {
    public:
        /**
         * @brief Returns the type of the event.
         *
         * This is a pure virtual function that must be implemented by all derived classes.
         *
         * @return The specific `EventType_` for the event.
         */
        virtual EventType_ GetEventType() const = 0;
    };


   
    /**
     * @brief Event representing a window resize action.
     *
     * The `WindowResizeEvent` class encapsulates information about a window resize event.
     * It stores the new size of the window and provides methods to retrieve this information.
     *
     */
    class WindowResizeEvent : public Event
    {
    public:
        /**
         * @brief Constructs a `WindowResizeEvent` with the new window size.
         *
         * @param new_window_size The new size of the window, stored as a `Size2D`.
         */
        WindowResizeEvent(Size2D new_window_size) :
            m_eventWindowSize(new_window_size)
        {}
        /**
         * @brief Returns the new size of the window.
         *
         * @return The size of the window as a `Size2D` object.
         */
        Size2D GetWindowSize() const { return m_eventWindowSize; }
        /**
         * @brief Returns a constant reference to the new window size.
         *
         * @return A constant reference to the size of the window as a `Size2D` object.
         */
        const Size2D& GetWindowSizeRef() const { return m_eventWindowSize; }
        /**
         * @brief Returns the type of event.
         *
         * @return `EventType_kWindowResize`, indicating this is a window resize event.
         */
        EventType_ GetEventType() const override { return EventType_kWindowResize; }

    private:
        Size2D m_eventWindowSize{}; ///< Stores the new window size.
    };


    /**
     * @brief Event representing a window close action.
     *
     */
    class WindowCloseEvent : public Event
    {
    public:
        /**
         * @brief Returns the type of event.
         *
         * @return `EventType_kWindowClosed`, indicating this is a window close event.
         */
        EventType_ GetEventType() const override { return EventType_kWindowClosed; }
    };

   
}