#pragma once
#include<GLFW/glfw3.h>
#include"Core/ApplicationData.h"
#include<functional>
namespace Chess_Game
{
    struct WindowCreateInfo
    {
        size_t windowWidth;
        size_t windowHeight;
        const char* windowTittle;
        std::function<void(int)> windowOnEventCallback;
    };

    enum GLFWErrorCode_
    {
        GLFWErrorCode_kGLFWAPIUnavailable = GLFW_API_UNAVAILABLE,
        GLFWErrorCode_kGLFWFormatUnavailable = GLFW_FORMAT_UNAVAILABLE,
        GLFWErrorCode_kGLFWInvalidEnum = GLFW_INVALID_ENUM,
        GLFWErrorCode_kGLFWInvalidValue = GLFW_INVALID_VALUE,
        GLFWErrorCode_kGLFWNoCurrentContext = GLFW_NO_CURRENT_CONTEXT,
        GLFWErrorCode_kGLFWNotInitialized = GLFW_NOT_INITIALIZED,
        GLFWErrorCode_kGLFWOutOfMemory = GLFW_OUT_OF_MEMORY,
        GLFWErrorCode_kGLFWPlatformError = GLFW_PLATFORM_ERROR,
        GLFWErrorCode_kGLFWVersionUnavailable = GLFW_VERSION_UNAVAILABLE
    };

    class Window
    {
    public:
        Window(const WindowCreateInfo& window_create_info);
        ~Window() { glfwDestroyWindow(m_glfwWindowHandle);}
        Window(const Window& other) = delete;
        Window(Window&& move) = delete;
        Window& operator =(const Window& other) = delete;
        Window& operator =(Window&& other) = delete;

        bool IsWindowValid()const { return m_glfwWindowHandle != nullptr; }
        bool ShouldWindowClose() { return glfwWindowShouldClose(m_glfwWindowHandle); }
        void OnUpdate();
        Size2D& GetWindowSize() { return m_windowSize; }
        const Size2D& GetWindowSize()const { return m_windowSize; }
    private:
        void BindWindowClassToCallbackFunctions();
        void WindowResizeCallback(GLFWwindow* window, int new_width,int new_height);
        void WindowMouseInputCallback(GLFWwindow* window, int button, int action, int mod);
        void WindowShouldCloseCallback(GLFWwindow* window);
        static void WindowErrorCallback(int error, const char* description);
    private:
        Size2D m_windowSize{};
        GLFWwindow* m_glfwWindowHandle = nullptr;
        std::function<void(int)> m_WindowEventCallback;
    };

}