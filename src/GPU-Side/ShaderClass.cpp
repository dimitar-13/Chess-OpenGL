#include "ShaderClass.h"
#include <fstream>
#include <array>
#include "Logging/Logger.h"
Chess_Game::ShaderClass::ShaderClass(const char* shader_file_path)
{
    constexpr size_t kExpectedShaderSize = 2;

    std::unordered_map<ShaderType_,std::string> shaders_source_hash = ReadShaderSources(shader_file_path);
    std::array<GLuint, kExpectedShaderSize> shader_handles{};

    for (auto& [key, source] : shaders_source_hash)
    {
        int shader_handles_index = key - GL_FRAGMENT_SHADER;

        shader_handles[shader_handles_index] = CompileShader(source.c_str(), key);
    }
    m_shaderProgramHandle = glCreateProgram();

    for (GLuint shader_handle : shader_handles)
    {
        if(shader_handle != 0)
            glAttachShader(m_shaderProgramHandle, shader_handle);
    }
    glLinkProgram(m_shaderProgramHandle);

    for (GLuint shader_handle : shader_handles)
    {
        if (shader_handle != 0)
        {
            glDetachShader(m_shaderProgramHandle, shader_handle);
            glDeleteShader(shader_handle);
        }
    }

    if (!IsProgramLinked())
    {
        glDeleteProgram(m_shaderProgramHandle);

        m_shaderProgramHandle = 0;
        return;
    }
    ExtractUniforms();


}
Chess_Game::ShaderClass::ShaderClass(const std::string& vertex_shader_source, const std::string& fragment_shader_source)
{
    if (vertex_shader_source.empty() || fragment_shader_source.empty())
    {
        CHESS_LOG_ERROR("[Shader class] Must provide at least vertex and fragment shader to create a valid program.");
        return;
    }

    GLuint vertex_shader_handle = CompileShader(vertex_shader_source.c_str(), ShaderType_kVertexShader);
    GLuint fragment_shader_handle = CompileShader(fragment_shader_source.c_str(), ShaderType_kFragmentShader);

    if (vertex_shader_handle == 0 || fragment_shader_handle == 0)
    {
        CHESS_LOG_ERROR("[Shader class] One of 2 required shaders failed to compile.");
        return;
    }

    m_shaderProgramHandle = glCreateProgram();

    glAttachShader(m_shaderProgramHandle, vertex_shader_handle);
    glAttachShader(m_shaderProgramHandle, fragment_shader_handle);

    glLinkProgram(m_shaderProgramHandle);

  
    glDetachShader(m_shaderProgramHandle, vertex_shader_handle);
    glDeleteShader(vertex_shader_handle);

    glDetachShader(m_shaderProgramHandle, fragment_shader_handle);
    glDeleteShader(fragment_shader_handle);

    if (!IsProgramLinked())
    {
        glDeleteProgram(m_shaderProgramHandle);

        m_shaderProgramHandle = 0;
        return;
    }
    ExtractUniforms();


}
std::unordered_map<Chess_Game::ShaderClass::ShaderType_, std::string>Chess_Game::ShaderClass::ReadShaderSources(const char* shader_file_path)
{
    std::unordered_map<ShaderType_, std::string> result{};

    std::fstream shader_file(shader_file_path);
    if (!shader_file.is_open())
    {
        CHESS_LOG_ERROR("[ShaderClass]: Failed to open file at {0}:", shader_file_path);
        return std::unordered_map<ShaderType_, std::string>();
    }

    std::string line;
    ShaderType_ current_reading_shader;
    while (std::getline(shader_file, line))
    {
        if (line.compare("#Shader:Vertex") == 0 )
        {
            current_reading_shader = ShaderType_kVertexShader;
            continue;
        }
        else if(line.compare("#Shader:Fragment") == 0)
        {
            current_reading_shader = ShaderType_kFragmentShader;
            continue;
        }
        result[current_reading_shader] += line.append("\n");
    }

    return result;
}

GLuint Chess_Game::ShaderClass::CompileShader(const char* shader_source, ShaderType_ type_of_shader)
{
    constexpr GLuint kInvalidShaderHandle = 0;
    GLuint shader_handle{};

    shader_handle = glCreateShader(type_of_shader);  
    glShaderSource(shader_handle, 1,&shader_source, NULL);
    glCompileShader(shader_handle);

    if(IsShaderCompiledSuccessfully(shader_handle))
        return shader_handle;

    glDeleteShader(shader_handle);
    return kInvalidShaderHandle;
}

bool Chess_Game::ShaderClass::IsShaderCompiledSuccessfully(GLuint shader_handle)
{
    constexpr size_t kInfoLogBufferSize = 256;
    auto GetShaderTypeStringFromEnum = [](ShaderType_ enum_to_get_the_string)
        {
            switch (enum_to_get_the_string)
            {
            case Chess_Game::ShaderClass::ShaderType_kVertexShader:
                return "Vertex shader";
            case Chess_Game::ShaderClass::ShaderType_kFragmentShader:
                return "Fragment shader";
            default:
                return "Unknown shader";
                break;
            }
        };

    int result;
    int shader_type{};
    glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &result);
    
    if (!result)
    {
        glGetShaderiv(shader_handle, GL_SHADER_TYPE, &shader_type);
        ShaderType_ shader_type_enum = static_cast<ShaderType_>(shader_type);

        char info_log[kInfoLogBufferSize] = {};
        int info_log_true_size{};
        glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &info_log_true_size);

        glGetShaderInfoLog(shader_handle, kInfoLogBufferSize, &info_log_true_size, &info_log[0]);

        CHESS_LOG_ERROR(
            "[Shader class] Shader failed to compile successfully.\n Shader type '{0}'.\n Compile error:{1}",
            GetShaderTypeStringFromEnum(shader_type_enum), &info_log[0]);
        
        return false;
    }
    return true;
}

bool Chess_Game::ShaderClass::IsProgramLinked()
{
    constexpr size_t kInfoLogBufferSize = 256;

    int result{};
    int shader_type{};
    glGetProgramiv(m_shaderProgramHandle, GL_LINK_STATUS, &result);

    if (!result)
    {
        char info_log[kInfoLogBufferSize] = {};
        int info_log_true_size{};
        glGetProgramiv(m_shaderProgramHandle, GL_INFO_LOG_LENGTH, &info_log_true_size);

        glGetProgramInfoLog(m_shaderProgramHandle, kInfoLogBufferSize, &info_log_true_size, &info_log[0]);

        CHESS_LOG_ERROR("[Shader class] Shader program failed to link successfully.Link error:{0}", &info_log[0]);

        return false;
    }
    return true;
}

void Chess_Game::ShaderClass::ExtractUniforms()
{
    constexpr size_t kUniformNameBufferSize = 50;
    int uniform_count{};
    glGetProgramiv(m_shaderProgramHandle, GL_ACTIVE_UNIFORMS, &uniform_count);
    m_uniformHash.reserve(uniform_count);

    for (size_t i = 0; i < uniform_count; i++)
    {   
        char uniform_name[kUniformNameBufferSize];
        int true_uniform_name_size{};
        int uniform_by_size{};
        GLenum uniform_value_type{};
        glGetActiveUniform(m_shaderProgramHandle,
            (GLuint)i, kUniformNameBufferSize, &true_uniform_name_size, &uniform_by_size, &uniform_value_type, &uniform_name[0]);
        GLuint uniform_index = glGetUniformLocation(m_shaderProgramHandle, &uniform_name[0]);
        m_uniformHash.emplace(uniform_name, uniform_index);
    }

}

int Chess_Game::ShaderClass::GetUniformByName(const char* uniform_name)
{
    if(m_uniformHash.find(uniform_name) != m_uniformHash.end())
        return m_uniformHash.at(uniform_name);
    return -1;
}
