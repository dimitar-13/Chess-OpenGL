#pragma once
#include "Core/Chess_pch.h"
#include "GPU-Side/TextureBatcher.h"
#include "GPU-Side/Framebuffer.h"
#include "Core/Drawable.h"
#include "Core/AssetLoader.h"
#include "Core/TextFont.h"

namespace Chess_Game
{
    constexpr size_t kSingleQuadIndexCount = 6;
    constexpr size_t kSingleQuadVertexCount = 4;

    struct Vertex
    {
        GLuint object_index{};
        glm::vec3 local_position{};
        glm::vec3 world_position{};
        glm::vec2 uv{};
        glm::vec3 color{};
        float texture_sampler_index{};
    };

    struct BatchGpuData
    {
        GLuint vertex_buffer_handle{};
        GLuint vertex_attribute_array_handle{};
        GLuint index_buffer_handle{};
    };

    template<size_t _QuadCount>
    struct BatchPipeline
    {
        static constexpr size_t kPerBatchQuadSize = _QuadCount;
        static constexpr size_t kBatchVertexArraySize = kSingleQuadVertexCount * kPerBatchQuadSize;
        static constexpr size_t kBatchIndexArraySize = kSingleQuadIndexCount * kPerBatchQuadSize;

        BatchPipeline(const char* batch_pipeline_shader)
        {
            glGenVertexArrays(1, &this->gpu_data.vertex_attribute_array_handle);

            glGenBuffers(1, &this->gpu_data.vertex_buffer_handle);
            glGenBuffers(1, &this->gpu_data.index_buffer_handle);

            glBindVertexArray(this->gpu_data.vertex_attribute_array_handle);

            glBindBuffer(GL_ARRAY_BUFFER, this->gpu_data.vertex_buffer_handle);
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

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->gpu_data.index_buffer_handle);
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

            this->quad_vector.reserve(_QuadCount);
            this->batch_shader = std::make_unique<ShaderClass>(batch_pipeline_shader);

        }
        void Draw()
        {
            size_t vertex_count = this->quad_vector.size() * 4;

            if (this->quad_vector.size() == 0) return;

            std::sort(this->quad_vector.begin(),
                this->quad_vector.end(),
                [](const std::array<Vertex, 4>& first_quad,
                    const std::array<Vertex, 4>& second_quad) {
                        return first_quad[0].world_position.z < second_quad[0].world_position.z;
                });

            glBindVertexArray(this->gpu_data.vertex_attribute_array_handle);

            glBindBuffer(GL_ARRAY_BUFFER, this->gpu_data.vertex_buffer_handle);
            glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_count * sizeof(Vertex), this->quad_vector.data());

            glDrawElements(GL_TRIANGLES, this->quad_vector.size() * kSingleQuadIndexCount, GL_UNSIGNED_INT, NULL);

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        void PushToBatch(const std::array<Vertex, 4>& vertex_data)
        {
            if (this->quad_vector.size() >= _QuadCount)
            {
                assert(false && "Batch renderer out of space: quad_vector size exceeds _QuadCount. Consider increasing batch size.");
                return;
            }
            this->quad_vector.emplace_back(vertex_data);
        }
        void FlushBatch()
        {
            this->quad_vector.clear();
        }
        ~BatchPipeline()
        {
            glDeleteBuffers(1, &this->gpu_data.vertex_buffer_handle);
            glDeleteBuffers(1, &this->gpu_data.index_buffer_handle);
            glDeleteVertexArrays(1, &this->gpu_data.vertex_attribute_array_handle);
        }

        BatchGpuData gpu_data{};
        std::vector<std::array<Vertex, 4>> quad_vector;
        std::unique_ptr<ShaderClass> batch_shader{};
    };
}