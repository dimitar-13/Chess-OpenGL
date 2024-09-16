#pragma once
#include "Core/Chess_pch.h"
#include "GPU-Side/TextureBatcher.h"
#include "GPU-Side/Framebuffer.h"
namespace Chess_Game
{
    struct Vertex
    {
        GLuint object_index{};
        glm::vec3 local_position{};
        glm::vec3 world_position{};
        glm::vec2 uv{};
        glm::vec3 color{};
        float texture_sampler_index{};
    };

    struct BatchRendererData
    {
        static constexpr size_t kPerBatchQuadSize = 100;
        static constexpr size_t kSingleQuadIndexCount = 6;
        static constexpr size_t kSingleQuadVertexCount = 4;
        static constexpr size_t kBatchVertexArraySize = kSingleQuadVertexCount * kPerBatchQuadSize;
        static constexpr size_t kBatchIndexArraySize = kSingleQuadIndexCount * kPerBatchQuadSize;

        Vertex* vertex_batch_pointer = nullptr;
        Vertex* vertex_batch_array = nullptr;

        GLuint* index_batch_pointer = nullptr;
        GLuint* index_batch_array = nullptr;
    };

    struct BatchGpuData
    {
        GLuint vertex_buffer_handle{};
        GLuint vertex_attribute_array_handle{};
        GLuint index_buffer_handle{};
    };
    struct BatchData
    {
        BatchGpuData gpu_data{};
        BatchRendererData render_data{};
        std::unique_ptr<ShaderClass> batch_shader{};
    };

    class BatchRenderer
    {
    public:
        BatchRenderer(Size2D window_size);
        void Push(size_t object_id,
            const glm::vec3& position,const glm::vec2& scale,const glm::vec3& object_color, Texture texture_index = 0);
        void PushCircle(const glm::vec3& position, const glm::vec2& scale, const glm::vec3& object_color);
        void DrawCircleBatch(const glm::mat4& projection);
        void DrawTextureQuadBatch(const glm::mat4& projection);
        void DrawTextureQuadBatchToIndexBuffer(IntFramebuffer& output_index_buffer,
            const glm::mat4& projection);
        ~BatchRenderer();
    private:
        void SetupBatch(BatchData& batch_to_setup);
        void BeginBatch(BatchData& batch_to_begin);
        void FreeBatchMemory(BatchData& batch_to_free);
    private:
        BatchData m_TexturedQuadBatch{};
        BatchData m_CircleQuadBatch{};
        TextureBatcher m_TextureBatcher{};
        std::unique_ptr<ShaderClass> m_MousePickingShader{};
    };

}