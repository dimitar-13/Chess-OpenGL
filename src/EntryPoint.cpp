#include "Pch/Chess_pch.h"
#include "Logging/Logger.h"
#include "Core/Application.h"

void APIENTRY OpenGLDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
    GLsizei length, const char* message, const void* userParam);

int main() {

    Chess_Game::Logger::Init();

    auto application = std::make_shared<Chess_Game::Application>();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);


    if (!application->IsApplicationUnitizedSuccessfully()) {
        CHESS_LOG_FATAL("Failed to initialize the application");
        return -1;
    }

#ifdef CHESS_DEBUG
    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OpenGLDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
#endif

    CHESS_LOG_INFO("Application was started successfully.");

    application->StartRenderLoop();
   

    CHESS_LOG_INFO("Application exited successfully.");

    return 0;
}

void OpenGLDebugOutput(GLenum source, GLenum type, 
    unsigned int id, GLenum severity, GLsizei length,
    const char* message, const void* userParam)
{
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::string_view formatted_debug_output = "[{0}][{1}] Debug message:{2}.";
    std::string_view debug_source_info_string;
    std::string_view debug_type;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             debug_source_info_string = "API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   debug_source_info_string = "Window"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: debug_source_info_string = "Shader"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     debug_source_info_string = "Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     debug_source_info_string = "Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           debug_source_info_string = "Other"; break;
    }

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               debug_type = "Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: debug_type = "Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  debug_type = "Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         debug_type = "Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         debug_type = "Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              debug_type = "Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          debug_type = "Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           debug_type = "Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               debug_type = "Other"; break;
    }

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        CHESS_LOG_FATAL(formatted_debug_output.data(),debug_source_info_string.data(),debug_type.data(),message); break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        CHESS_LOG_WARN (formatted_debug_output.data(),debug_source_info_string.data(),debug_type.data(),message);  break;
    case GL_DEBUG_SEVERITY_LOW:
        CHESS_LOG_WARN (formatted_debug_output.data(),debug_source_info_string.data(),debug_type.data(),message);  break;
    }
}
