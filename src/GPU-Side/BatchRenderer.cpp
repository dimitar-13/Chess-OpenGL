#include "BatchRenderer.h"

Chess_Game::BatchRenderer::BatchRenderer()
{
    glGenVertexArrays(1, &m_BatchVertexAttributeObject);

    glGenBuffers(1, &m_BatchVertexArrayBufferObject);
    glGenBuffers(1, &m_BatchIndexArrayBufferObject);

    glBindVertexArray(m_BatchVertexAttributeObject);

    glBindBuffer(GL_ARRAY_BUFFER, m_BatchVertexArrayBufferObject);
    glBufferData(GL_ARRAY_BUFFER, BatchRendererData::kBatchVertexArraySize*sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BatchIndexArrayBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, BatchRendererData::kBatchIndexArraySize*sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, vertex_position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, vertex_color));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    m_batchData.vertex_batch_array = new Vertex[BatchRendererData::kBatchVertexArraySize];
    m_batchData.vertex_batch_pointer = m_batchData.vertex_batch_array;

    m_batchData.index_batch_array = new GLuint[BatchRendererData::kBatchIndexArraySize];
    m_batchData.index_batch_pointer = m_batchData.index_batch_array;

 }

void Chess_Game::BatchRenderer::Push(const glm::vec2& position, const glm::vec3& object_color)
{
    constexpr float kScaleFactor = 50.0f;
    GLuint quad_index_data[6] = {
        0,1,2,
        2,3,0
    };

    Vertex quad_vertex_data[] = {
        //First triangle
        {{-1.0f,1.0f}, {1.0f,1.0f,1.0f}},
        {{-1.0f,-1.0f},{1.0f,1.0f,1.0f}},
        //Second triangle
        {{1.0f,-1.0f}, {1.0f,1.0f,1.0f}},
        {{1.0f,1.0f} , {1.0f,1.0f,1.0f}},
   
    };
    for (auto & vertex : quad_vertex_data)
    {
        vertex.vertex_position += position;
        vertex.vertex_position /= kScaleFactor;
        vertex.vertex_color = object_color;
    }
 
    memcpy(m_batchData.vertex_batch_pointer, quad_vertex_data, sizeof(quad_vertex_data));
     
    m_batchData.vertex_batch_pointer += BatchRendererData::kSingleQuadVertexCount;

    size_t index_data_offset = (m_batchData.index_batch_pointer - m_batchData.index_batch_array)/ BatchRendererData::kSingleQuadIndexCount;
    for (auto& index : quad_index_data)
    {
        index += 4 * index_data_offset;
    }
    memcpy(m_batchData.index_batch_pointer, quad_index_data, sizeof(quad_index_data));


    m_batchData.index_batch_pointer += BatchRendererData::kSingleQuadIndexCount;

}

void Chess_Game::BatchRenderer::Flush()
{
    size_t vertex_data_size = (m_batchData.vertex_batch_pointer - m_batchData.vertex_batch_array) * sizeof(Vertex);
    size_t index_data_size = (m_batchData.index_batch_pointer - m_batchData.index_batch_array) * sizeof(GLuint);
    size_t index_count = (m_batchData.index_batch_pointer - m_batchData.index_batch_array);

    if (index_count == 0) return;

    glBindBuffer(GL_ARRAY_BUFFER, m_BatchVertexArrayBufferObject);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_data_size, m_batchData.vertex_batch_array);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BatchIndexArrayBufferObject);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, index_data_size, m_batchData.index_batch_array);

    glBindVertexArray(m_BatchVertexAttributeObject);
    glDrawElements(GL_TRIANGLES, index_count,GL_UNSIGNED_INT,NULL);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    BeginBatch();

}


Chess_Game::BatchRenderer::~BatchRenderer()
{
    delete[] m_batchData.vertex_batch_array;
    delete[] m_batchData.index_batch_array;

    glDeleteBuffers(1, &m_BatchVertexArrayBufferObject);
    glDeleteBuffers(1, &m_BatchIndexArrayBufferObject);
    glDeleteVertexArrays(1, &m_BatchVertexArrayBufferObject);
}

void Chess_Game::BatchRenderer::BeginBatch()
{
    m_batchData.vertex_batch_pointer = m_batchData.vertex_batch_array;
    m_batchData.index_batch_pointer = m_batchData.index_batch_array;
}
