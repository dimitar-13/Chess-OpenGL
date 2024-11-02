#pragma once
#include "Pch/Chess_pch.h"
#include "GPU-Side/TextureBatcher.h"
#include "GPU-Side/Framebuffer.h"
#include "Core/Drawable.h"
#include "Core/AssetLoader.h"
#include "Core/TextFont.h"

namespace Chess_Game
{
    constexpr size_t kSingleQuadIndexCount = 6;
    constexpr size_t kSingleQuadVertexCount = 4;
    /**
     * @brief Struct representing a vertex used by the batch renderer.
     * 
     * Vertex mainly used for textured quads. 
     * The struct contains 2 types of information:
     * 1) Per vertex information
     *    - position
     *    - uv
     * 2) For the whole primitive/quad
     *    - Object ID.
     *    - Texture sampler index.
     *    - Quad color.
     * 
     */
    struct Vertex
    {
        GLuint object_index{};          ///< Quad index.
        glm::vec3 local_position{};     ///< Vertex local position(Position before the transform usually [-1 : 1]).
        glm::vec3 world_position{};     ///< Vertex world position(Position after the transform ususally in the orthographic matrix range).
        glm::vec2 uv{};                 ///< Vertex UV position.
        glm::vec3 color{};              ///< Quad color.
        float texture_sampler_index{};  ///< The texture bind index asigned by the 'TextureBatcher'.
    };
    /**
     * @brief Simple struct representing GPU resources.
     *
     * The struct holds handles to GPU resources used by the batch renderer for indexed based drawing.
     * Resource handles:
     * - Vertex buffer object(VBO) handle.
     * - Index buffer object(IBO) handle.
     * - Vertex attribute object(VAO) handle.
     */
    struct BatchGpuData
    {
        GLuint vertex_buffer_handle{};          ///< Vertex buffer object handle.
        GLuint vertex_attribute_array_handle{}; ///< Index buffer object handle.
        GLuint index_buffer_handle{};           ///< Index buffer object handle.
    };

    /**
     * @brief Defines a way of drawing a specific batch.
     *
     * This implementation is really a bare bone impl of a bigger idea.
     * The idea is to define a pipeline in a sense that here it will be known what steps and what data is required to render 
     * something using the following pipeline. 
     * This approach makes everything more self contained and it makes it so that only the pipeline knows what it needs to render 
     * something.
     * 
     * The implementation below is a simple version that suits well enough the needs of the batch renderer used in this project.
     * It creates and manages its shader and its own batch buffer data. It provides functions to 'Draw()','PushToBatch()' and 
     * 'Flush()' the batch data.
     * 
     */
    template<size_t _QuadCount>
    class BatchPipeline
    {
    public:
        static constexpr size_t kBatchVertexArraySize = kSingleQuadVertexCount * _QuadCount;
        static constexpr size_t kBatchIndexArraySize = kSingleQuadIndexCount * _QuadCount;
        /**
         * @brief Constructor allocates all of the necessary GPU resources and creates the pipeline shader using the provided file path. 
         * 
         * The constructor creates 3 types of GPU resources:
         *  - Dynamic Vertex buffer for storing the batch renderer data.
         *  - Static Index buffer for indexed drawing.
         *  - Vertex attribute object for defining the vertex data layout.
         * 
         * After the GPU resources are created they are initalized. The Dynamic vertex buffer is allocated and set to null while the 
         * index buffer is filled with indices.
         * 
         * After everything is set up and bound lastly we create the shader used by the pipeline using the 'ShaderClass'.
         * @param batch_pipeline_shader File path to the shader to be used by the pipeline.
         */
        BatchPipeline(const char* batch_pipeline_shader);
        /**
         * @brief Draws all of the pushed quads using the pipeline shader.
         * 
         * Before drawing anything we sort the buffer data by Z-Depth value. That's the reason why we safe the buffer data in 
         * pair of 4 so that we sort quads and not vertices. After the sort we push the data into the GPU buffer and we invoke
         * the draw call.
         * One thing to note the user class of this must explicitly bind the shader uniforms before issuing a draw call.
         */
        void Draw();
        /**
         * @brief Pushes a quad to the vertex data.
         * @param vertex_data The quad vertex data to push.
         */
        void PushToBatch(const std::array<Vertex, 4>& vertex_data);
        /**
         * @brief Flushes the vertex data batch. 
         */
        void FlushBatch() { this->m_QuadVector.clear(); }
        /**
         * @brief Destroys all of the pipeline GPU resources.
         */
        ~BatchPipeline();
        ShaderClass& GetPipelineShader() { return *m_BatchShader; }
        std::vector<std::array<Vertex, 4>> m_QuadVector;    ///< STL pipeline quad buffer used for pushing quads and sorting them.
    private:
        BatchGpuData m_GPU_Data{};                          ///< The GPU resources data struct.
        std::unique_ptr<ShaderClass> m_BatchShader{};       ///< The pipeline shader used for drawing.
    };
    template<size_t _QuadCount>
    inline BatchPipeline<_QuadCount>::BatchPipeline(const char* batch_pipeline_shader)
    {
        glGenVertexArrays(1, &this->m_GPU_Data.vertex_attribute_array_handle);

        glGenBuffers(1, &this->m_GPU_Data.vertex_buffer_handle);
        glGenBuffers(1, &this->m_GPU_Data.index_buffer_handle);

        glBindVertexArray(this->m_GPU_Data.vertex_attribute_array_handle);

        glBindBuffer(GL_ARRAY_BUFFER, this->m_GPU_Data.vertex_buffer_handle);
        glBufferData(GL_ARRAY_BUFFER, kBatchVertexArraySize * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

        std::array<GLuint, kBatchIndexArraySize> index_array{};
        GLuint index_buffer_index = 0;
        size_t i = 0;
        size_t quad_counter = 0;

        while (quad_counter < _QuadCount)
        {
            index_array[i] = index_buffer_index;
            index_array[i + 1] = index_buffer_index + 1;
            index_array[i + 2] = index_buffer_index + 2;

            index_array[i + 3] = index_buffer_index + 2;
            index_array[i + 4] = index_buffer_index + 3;
            index_array[i + 5] = index_buffer_index;

            quad_counter++;
            index_buffer_index += 4;
            i += 6;
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_GPU_Data.index_buffer_handle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_array), index_array.data(), GL_STATIC_DRAW);

        glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(Vertex), (void*)offsetof(Vertex, object_index));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, local_position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, world_position));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_sampler_index));
        glEnableVertexAttribArray(5);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        this->m_QuadVector.reserve(_QuadCount);
        this->m_BatchShader = std::make_unique<ShaderClass>(batch_pipeline_shader);

    }
    template<size_t _QuadCount>
    inline void BatchPipeline<_QuadCount>::Draw()
    {
        size_t vertex_count = this->m_QuadVector.size() * 4;

        if (this->m_QuadVector.size() == 0) return;

        std::sort(this->m_QuadVector.begin(),
            this->m_QuadVector.end(),
            [](const std::array<Vertex, 4>& first_quad,
                const std::array<Vertex, 4>& second_quad) {
                    return first_quad[0].world_position.z < second_quad[0].world_position.z;
            });

        glBindVertexArray(this->m_GPU_Data.vertex_attribute_array_handle);

        glBindBuffer(GL_ARRAY_BUFFER, this->m_GPU_Data.vertex_buffer_handle);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_count * sizeof(Vertex), this->m_QuadVector.data());

        glDrawElements(GL_TRIANGLES, this->m_QuadVector.size() * kSingleQuadIndexCount, GL_UNSIGNED_INT, NULL);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    template<size_t _QuadCount>
    inline void BatchPipeline<_QuadCount>::PushToBatch(const std::array<Vertex, 4>& vertex_data)
    {
        if (this->m_QuadVector.size() >= _QuadCount)
        {
            assert(false && "Batch renderer out of space: quad_vector size exceeds _QuadCount. Consider increasing batch size.");
            return;
        }
        this->m_QuadVector.emplace_back(vertex_data);
    }
    template<size_t _QuadCount>
    inline BatchPipeline<_QuadCount>::~BatchPipeline()
    {
        glDeleteBuffers(1, &this->m_GPU_Data.vertex_buffer_handle);
        glDeleteBuffers(1, &this->m_GPU_Data.index_buffer_handle);
        glDeleteVertexArrays(1, &this->m_GPU_Data.vertex_attribute_array_handle);
    }
}