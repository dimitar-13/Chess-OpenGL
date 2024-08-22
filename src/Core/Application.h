#pragma once
#include"Window.h"
#include<memory>
namespace Chess_Game
{
    class Application 
    {
    public:
        Application();
        void RenderLoop();
        bool IsApplicationUnitizedSuccessfully()const { return m_ApplicationInitStatus; }
        void OnEvent(int test_value);
    private:
        bool m_ApplicationInitStatus = true;
        bool m_isApplicationRunning = true;
        std::unique_ptr<Window> m_ApplicationWindow;
    };


}