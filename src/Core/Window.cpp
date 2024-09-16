#include "Window.h"
#include "Logging/Logger.h"
#include "Event/MouseEvents.h"

Chess_Game::Window::Window(const WindowCreateInfo& window_create_info)
{
    if (!glfwInit()) {
        CHESS_LOG_FATAL("[GLFW] GLFW failed to initialize.");
        return;
    }

    m_glfwWindowHandle = glfwCreateWindow(window_create_info.windowWidth, window_create_info.windowHeight,
        window_create_info.windowTittle, nullptr, nullptr);

    if (!this->IsWindowValid())
    {
        CHESS_LOG_FATAL("[GLFW] Window failed to be created.");
        return;
    }

    m_WindowEventCallback = window_create_info.windowOnEventCallback;
    glfwMakeContextCurrent(m_glfwWindowHandle);
    BindWindowClassToCallbackFunctions();
    m_windowSize = Size2D{ window_create_info.windowWidth, window_create_info.windowHeight };
}

Chess_Game::Window::~Window()
{
    glfwDestroyWindow(m_glfwWindowHandle);
    glfwTerminate();
}

void Chess_Game::Window::OnUpdate()
{
    glfwPollEvents();
    glfwSwapBuffers(m_glfwWindowHandle);
}

void Chess_Game::Window::BindWindowClassToCallbackFunctions()
{
    glfwSetWindowUserPointer(m_glfwWindowHandle, reinterpret_cast<void*>(this));
    auto WindowResizeCallback = [](GLFWwindow* window, int width, int height)
        {
            static_cast<Window*>(glfwGetWindowUserPointer(window))->WindowResizeCallback(window, width, height);
        };
    glfwSetWindowSizeCallback(m_glfwWindowHandle, WindowResizeCallback);

    auto WindowMouseButtonPressCallback = [](GLFWwindow* window, int button, int action, int mod)
        {
            static_cast<Window*>(glfwGetWindowUserPointer(window))->WindowMouseInputCallback(window, button,action,mod);
        };
    glfwSetMouseButtonCallback(m_glfwWindowHandle, WindowMouseButtonPressCallback);

    auto WindowShouldCloseCallback = [](GLFWwindow* window)
        {
            static_cast<Window*>(glfwGetWindowUserPointer(window))->WindowShouldCloseCallback(window);
        };

    glfwSetWindowCloseCallback(m_glfwWindowHandle, WindowShouldCloseCallback);

    auto MousePositionChangedCallback = [](GLFWwindow* window, double mouse_x, double mouse_y)
        {
            static_cast<Window*>
                (glfwGetWindowUserPointer(window))->WindowMousePositionChangedCallback(window, mouse_x, mouse_y);
        };

    glfwSetCursorPosCallback(m_glfwWindowHandle, MousePositionChangedCallback);


    glfwSetErrorCallback(WindowErrorCallback);
}

void Chess_Game::Window::WindowResizeCallback(GLFWwindow* window, int new_width, int new_height)
{
    m_windowSize.width = new_width;
    m_windowSize.height = new_height;

    WindowResizeEvent e(m_windowSize);
    m_WindowEventCallback(e);
}

void Chess_Game::Window::WindowMouseInputCallback(GLFWwindow* window, int button, int action, int mod)
{
    MouseButtonEvent e(static_cast<MouseButton_>(button), static_cast<InputAction_>(action));
    m_WindowEventCallback(e);
}

void Chess_Game::Window::WindowShouldCloseCallback(GLFWwindow* window)
{
    WindowCloseEvent e;
    m_WindowEventCallback(e);
}

void Chess_Game::Window::WindowMousePositionChangedCallback(GLFWwindow* window, double mouse_x, double mouse_y)
{
    MousePositionChangedEvent e(MousePos{ mouse_x,mouse_y },
        MousePos{ mouse_x,m_windowSize.height - mouse_y });
    m_WindowEventCallback(e);
}

void Chess_Game::Window::WindowErrorCallback(int error, const char* description)
{
    CHESS_LOG_ERROR("[GLFW] Error message :{0}", description);
}
