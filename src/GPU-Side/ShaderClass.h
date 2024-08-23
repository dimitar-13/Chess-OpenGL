#pragma once
#include<GL/glew.h>
#include<vector>
#include<unordered_map>
#include<string>
#include<glm/glm.hpp>
namespace Chess_Game
{
    class ShaderClass
    {
    private:
        enum ShaderType_
        {
            ShaderType_kVertexShader = GL_VERTEX_SHADER,
            ShaderType_kFragmentShader = GL_FRAGMENT_SHADER,
        };
    public:
        ShaderClass(const char* shader_file_path);
        ShaderClass(const std::string& vertex_shader_source, const std::string& fragment_shader_source);
        ~ShaderClass() { glDeleteProgram(m_shaderProgramHandle); }
        bool IsProgramCreatedSuccessfully()const { return m_shaderProgramHandle != 0; }
        void SetUniformInt(const char* uniform_name, int value) { glUniform1i(GetUniformByName(uniform_name), value); }
        void SetUniformFloat(const char* uniform_name, float value) { glUniform1f(GetUniformByName(uniform_name), value); }
        void SetUniform4x4Matrix(const char* uniform_name, const glm::mat4& matrix) { glUniformMatrix4fv(GetUniformByName(uniform_name),1,GL_FALSE,&matrix[0][0]); }
        void UseProgram()const { glUseProgram(m_shaderProgramHandle); }
    private:
        std::unordered_map<ShaderType_,std::string> ReadShaderSources(const char* shader_file_path);
        GLuint CompileShader(const char* shader_source, ShaderType_ type_of_shader);
        bool IsShaderCompiledSuccessfully(GLuint shader_handle);
        bool IsProgramLinked();
        void ExtractUniforms();
        int GetUniformByName(const char* uniform_name);
    private:
        GLuint m_shaderProgramHandle{};
        std::unordered_map <std::string, GLuint> m_uniformHash{};
    };


}