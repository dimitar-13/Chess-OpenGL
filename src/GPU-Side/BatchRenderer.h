#pragma once
#include<GL/glew.h>
#include<glm/glm.hpp>
#include<array>
namespace Chess_Game
{
    struct Vertex
    {
        //size_t batch_index{};
        glm::vec2 vertex_position{};
        glm::vec3 vertex_color{};
    };

    struct BatchRendererData
    {
        static constexpr size_t kMaxBatchQuadSize = 50;
        static constexpr size_t kSingleQuadIndexCount = 6;
        static constexpr size_t kSingleQuadVertexCount = 4;
        static constexpr size_t kSingleQuadBatch = (kSingleQuadVertexCount * sizeof(Vertex));
        static constexpr size_t kBatchVertexArraySize = kSingleQuadVertexCount * kMaxBatchQuadSize;
        static constexpr size_t kBatchIndexArraySize = kSingleQuadIndexCount * kMaxBatchQuadSize;

        Vertex* vertex_batch_pointer = nullptr;
        Vertex* vertex_batch_array = nullptr;

        GLuint* index_batch_pointer = nullptr;
        GLuint* index_batch_array = nullptr;
    };

    class BatchRenderer
    {
    public:
        BatchRenderer();
        void Push(const glm::vec2& position,const glm::vec2& scale,const glm::vec3& object_color);
        void Flush();
        ~BatchRenderer();
    private:
        void BeginBatch();
    private:
        GLuint m_BatchVertexAttributeObject{};
        GLuint m_BatchVertexArrayBufferObject{};
        GLuint m_BatchIndexArrayBufferObject{};
        BatchRendererData m_batchData{};
    };

}