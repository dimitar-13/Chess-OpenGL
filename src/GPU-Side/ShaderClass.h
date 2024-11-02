#pragma once
#include "Pch/Chess_pch.h"
namespace Chess_Game
{
    /**
     * @brief A class for handling OpenGL shader programs.
     *
     * Class for managing a GPU shader. The class creates a valid GPU shader program from provided
     * shader file. The shader file contains both vertex and fragment shader separated 
     * by #Shader:<Shader-Name>(Vertex,Fragment) directive. The class compiles and links the final
     * program and provides way of updating shader uniform states via 'SetUniform<Type-Of-Uniform>'.
     *
     * The class supports two types of shaders:
     * - `Vertex Shader`: Responsible for processing individual vertices.
     * - `Fragment Shader`: Handles the processing of fragments (pixels).
     *
     */
    class ShaderClass
    {
    private:
        /**
         * @brief Internal enum for identifying shader types.
         *
         * This enum maps shader types (vertex and fragment shaders) to their corresponding
         * OpenGL shader types (e.g., `GL_VERTEX_SHADER`, `GL_FRAGMENT_SHADER`).
         * Its used for debug purposes.
         */
        enum ShaderType_
        {
            ShaderType_kVertexShader = GL_VERTEX_SHADER,   ///< Vertex shader identifier 'GL_VERTEX_SHADER'.
            ShaderType_kFragmentShader = GL_FRAGMENT_SHADER ///< Fragment shader identifier 'GL_FRAGMENT_SHADER'.
        };
    public:
        /**
         * @brief Constructs a `ShaderClass` object by loading shaders from a file.
         *
         * Reads shader sources from the specified file and compiles them into a shader program.
         * The file is expected to contain both vertex and fragment shaders separated 
         * by #Shader:<Shader-Name>(Vertex,Fragment) directive. The order of shaders in the file
         * doesn't matter.
         *
         * @param shader_file_path The path to the file containing the shader source code.
         */
        ShaderClass(const char* shader_file_path);
        /**
         * @brief Constructs a `ShaderClass` object using provided vertex and fragment shader sources.
         *
         * Compiles and links shaders from the provided vertex and fragment shader source strings.
         * @note This was intended if we decided to switch from single file.
         * 
         * @param vertex_shader_source The source code for the vertex shader.
         * @param fragment_shader_source The source code for the fragment shader.
         */
        ShaderClass(const std::string& vertex_shader_source, const std::string& fragment_shader_source);
        /**
         * @brief Destructor that cleans up the shader program.
         *
         * Deletes the shader program using `glDeleteProgram` to free up GPU resources.
         */
        ~ShaderClass() { glDeleteProgram(m_shaderProgramHandle); }
        /**
         * @brief Checks if the shader program was successfully created.
         *
         * If there are any problems with compilation or linking this will return false.
         * 
         * @return `true` if the shader program was created successfully, `false` otherwise.
         */
        bool IsProgramCreatedSuccessfully() const { return m_shaderProgramHandle != 0; }
        /**
         * @brief Sets an integer uniform variable in the shader program.
         *
         * @param uniform_name The name of the uniform variable in the shader.
         * @param value The integer value to set.
         */
        void SetUniformInt(const char* uniform_name, int value) { glUniform1i(GetUniformByName(uniform_name), value); }
        /**
         * @brief Sets a 1D array of integers as a uniform variable in the shader program.
         *
         * @param uniform_name The name of the uniform variable in the shader.
         * @param array A pointer to the integer array.
         * @param size The size of the array.
         */
        void SetSampler2DArray(const char* uniform_name, const int* array, const size_t size) { glUniform1iv(GetUniformByName(uniform_name), size, array); }
        /**
         * @brief Sets a float uniform variable in the shader program.
         *
         * @param uniform_name The name of the uniform variable in the shader.
         * @param value The float value to set.
         */
        void SetUniformFloat(const char* uniform_name, float value) { glUniform1f(GetUniformByName(uniform_name), value); }
        /**
         * @brief Sets a 4x4 matrix uniform variable in the shader program.
         *
         * @param uniform_name The name of the uniform variable in the shader.
         * @param matrix The 4x4 matrix to set, provided as a `glm::mat4`.
         */
        void SetUniform4x4Matrix(const char* uniform_name, const glm::mat4& matrix) { glUniformMatrix4fv(GetUniformByName(uniform_name), 1, GL_FALSE, &matrix[0][0]); }
        /**
         * @brief Activates the shader program for use in the rendering pipeline.
         *
         * Calls `glUseProgram` to bind the shader program for use.
         */
        void UseProgram() const { glUseProgram(m_shaderProgramHandle); }
    private:
        /**
         * @brief Reads and parses the shader source code from a file.
         *
         * This function extracts the vertex and fragment shader sources from the specified file.
         *
         * The file is expected to contain both vertex and fragment shaders separated 
         * by #Shader:<Shader-Name>(Vertex,Fragment) directive. The order of shaders in the file
         * doesn't matter.
         * 
         * @param shader_file_path The path to the shader source file.
         * @return A map of `ShaderType_` to their corresponding shader source code.
         */
        std::unordered_map<ShaderType_, std::string> ReadShaderSources(const char* shader_file_path);
        /**
         * @brief Compiles the shader source code into a shader object.
         * 
         * Creates and compiles the provided shader.If compilation fails it returns 
         * 0(invalid shader object index).
         *
         * @param shader_source The source code of the shader.
         * @param type_of_shader The type of shader (`ShaderType_`).
         * @return The OpenGL handle to the compiled shader.
         */
        GLuint CompileShader(const char* shader_source, ShaderType_ type_of_shader);
        /**
         * @brief Checks if the shader was compiled successfully.
         *
         * By a given shader handle the function uses the shader compile status and info log to check 
         * if the shader was compiled successfully. If not it logs the info log error and returns
         * false.
         * 
         * @param shader_handle The OpenGL handle of the compiled shader.
         * @return `true` if the shader compiled successfully, `false` otherwise.
         */
        bool IsShaderCompiledSuccessfully(GLuint shader_handle);
        /**
         * @brief Checks if the shader program was successfully linked.
         * 
         * The function uses the program link status and info log to check
         * if the program was linked successfully. If not it logs the info log error and returns
         * false.
         * @return `true` if the shader program linked successfully, `false` otherwise.
         */
        bool IsProgramLinked();
        /**
         * @brief Extracts all uniform variables in the shader program.
         *
         * The function gets all of the shader uniforms and hashes them in the 'm_uniformHash'.
         * @note The function also hashes individual array elements.
         */
        void ExtractUniforms();
        /**
         * @brief Retrieves the location of a uniform variable by its name.
         *
         * The function checks to find the uniform in the 'm_uniformHash'. If its not found it
         * uses the 'glGetUniformLocation' to retrieve it.
         * 
         * @note If it cant find uniform the function returns '-1'.
         * 
         * @param uniform_name The name of the uniform variable.
         * @return The location of the uniform in the shader program if it cant find it returns '-1'.
         */
        int GetUniformByName(const char* uniform_name);

    private:
        GLuint m_shaderProgramHandle{};  ///< OpenGL handle for the shader program.
        std::unordered_map<std::string, GLuint> m_uniformHash{};  ///< Cache of uniform variable locations.
    };


}