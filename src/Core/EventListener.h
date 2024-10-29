#pragma once
#include "Core/Event/Event.h"
namespace Chess_Game
{
    /**
     * @brief Interface used for event listening.
     *
     * The interface provides only 1 pure virtual function 'OnEvent'.
     * The user of this interface must override the function with its own custom data.
     * 'OnEvent()' its called for every event both app or input event.
     * 
     * To be notified of events the implementer of the interface must use the 'Application'
     * 'AddEventListener()' function.
     */
    class Listener : public std::enable_shared_from_this<Listener>
    {
    public:
        /**
         * @brief Virtual function called when event happens.
         *
         * The function is called on every type of event.
         * @param e The event information.
         */
        virtual void OnEvent(const Event& e) = 0;
    };
}