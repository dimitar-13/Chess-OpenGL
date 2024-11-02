#pragma once
#include "Pch/Chess_pch.h"
#include "Core/ApplicationData.h"
#include "Core/Event/Event.h"
namespace Chess_Game
{
    /**
     * @brief Enum representing the GLFW error codes.
     *
     * Doing it this way makes it easier to debug if there is a need.
     */
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

    /**
     * @brief A window class for creating and working with a GLFW window/library.
     *
     * The class attempts to create a valid GLFW handle. Its goal is to abstract the work with 
     * the GLFW library so that the project depends on the class and not on the library. You can say
     * its like a wrapper class.
     * 
     * It provides a functions for getting GLFW window info like:
     * @see 'GetWindowSize()' For getting the current size.
     * @see 'ShouldWindowClose()' For getting if the window should close.
     * 
     */
    class Window
    {
    public:
        /**
         * @brief Construct a GLFW window wrapper for working with a GLFW window.
         *
         * Attempts to create a valid GLFW window and bind it to the current window context.
         * It initializes the GLFW library and tries to create a valid window.
         * If it fails it logs it. In case of failure you can use the  'IsWindowValid()' to see
         * if the window creation was successful.
         * 
         * If window creation is valid it binds the onEvent functions to the window functions 
         * @see 'BindWindowClassToCallbackFunctions()'.
         * 
         * @param start_window_size The start window size.
         * @param window_title Title of the window.
         * @param on_event_callback Function ptr for event output.
         */
        Window(Size2D start_window_size,const char* window_title,std::function<void(const Event&)> on_event_callback);
        /**
         * @brief Destroys the GLFW window handle and terminates the GLFW lib.
         */
        ~Window();
        Window(const Window& other) = delete; ///< For avoiding bugs since window coping is not a thing in this project.
        Window(Window&& move) = delete; ///< For avoiding bugs since window coping is not a thing in this project.
        Window& operator =(const Window& other) = delete; ///< For avoiding bugs since window coping is not a thing in this project.
        Window& operator =(Window&& other) = delete; ///< For avoiding bugs since window coping is not a thing in this project.
        /**
         * @brief Checks if the window creation was successful.
         * 
         * @return Returns if the created window was valid.
         */
        bool IsWindowValid()const { return m_glfwWindowHandle != nullptr; }
        /**
         * @brief Checks to see if the window should close using the GLFW API.
         *
         * @return Returns true if window is closed.
         */
        bool ShouldWindowClose()const { return glfwWindowShouldClose(m_glfwWindowHandle); }
        /**
         * @brief Handles the per-frame update logic for the application.
         *
         * This function is typically called once per frame to process input events
         * and update the window's contents. It uses `glfwPollEvents()` to gather
         * all pending input events (such as keyboard, mouse, and window events)
         * and ensures they are handled. It then swaps the front and back buffers
         * using `glfwSwapBuffers()` to display the rendered content from the current
         * frame to the screen.
         *
         * - `glfwPollEvents()`: Polls for and processes events.
         * - `glfwSwapBuffers()`: Swaps the front and back buffers, effectively displaying
         *    the rendered frame.
         */
        void OnUpdate();
        /**
         * @brief Returns the current window size.
         * @return Returns the current window size.
         */
        Size2D& GetWindowSize() { return m_windowSize; }
        /**
         * @brief Returns the current window size.
         * @return Returns the current window size.
         */
        const Size2D& GetWindowSize()const { return m_windowSize; }
    private:
        /**
         * @brief Binds the GLFW on<EventName> functions.
         * 
         * Binds the GLFW onEvent functions with the wrapper class functions.
         */
        void BindWindowClassToCallbackFunctions();
        /**
         * @brief Callback function triggered when the window is resized.
         *
         * It fires a "WindowResizeEvent" event with the new width and height of the window.
         * 
         * @param window GLFW window handle.
         * @param new_width The new width of the window in pixels.
         * @param new_height The new height of the window in pixels.
         */
        void WindowResizeCallback(GLFWwindow* window, int new_width,int new_height);
        /**
         * @brief Callback function triggered by mouse input events.
         *
         * This function is called whenever a mouse button is pressed or released.
         * It processes mouse input events, such as clicks, and fires an "MouseButtonEvent" event    
         * with the button code, action type (press/release), and any associated modifiers
         * (e.g., Shift, Ctrl).
         *
         * @param window GLFW window handle.
         * @param button The mouse button that was pressed or released (GLFW_MOUSE_BUTTON_*).
         * @param action The action performed (GLFW_PRESS, GLFW_RELEASE).
         * @param mod The bit field describing any modifier keys held (GLFW_MOD_*).
         */
        void WindowMouseInputCallback(GLFWwindow* window, int button, int action, int mod);
        /**
         * @brief Callback function triggered when the window should close.
         *
         * This function is called when the user attempts to close the window (e.g., by
         * clicking the close button). It fires a "WindowCloseEvent" event.
         *
         * @param window  GLFW window handle.
         */
        void WindowShouldCloseCallback(GLFWwindow* window);
        /**
         * @brief Callback function triggered when the mouse position changes.
         *
         * This function is called whenever the mouse cursor moves within the window.
         * It  fires an "MousePositionChangedEvent" event with the new x and y
         * coordinates of the mouse within the window's coordinate system.
         *
         * @param window GLFW window handle.
         * @param mouse_x The new x-coordinate of the mouse cursor.
         * @param mouse_y The new y-coordinate of the mouse cursor.
         */
        void WindowMousePositionChangedCallback(GLFWwindow* window,double mouse_x, double mouse_y);
        /**
         * @brief Callback function for handling GLFW errors.
         *
         * Logs the GLFW error using the "CHESS_LOG_ERROR" macro.
         * 
         * @param error The error code corresponding to the GLFW error.
         * @param description A description of the error.
         */
        static void WindowErrorCallback(int error, const char* description);
    private:
        Size2D m_windowSize{}; ///< The current window size.
        GLFWwindow* m_glfwWindowHandle = nullptr; ///< The GLFW window handle.
        std::function<void(const Event&)> m_WindowEventCallback; ///< Function pointer for the event output.
    };

}