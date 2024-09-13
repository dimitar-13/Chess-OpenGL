#include "BatchRenderer.h"
#include "Core/Chess_pch.h"

static const std::array<Chess_Game::Vertex,4> kBaseQuadVertexData = {
    //First triangle
    Chess_Game::Vertex{{-1.0f,1.0f,0.0f} ,{ -1.0f,1.0f,0.0f } , {0.0f,1.0f}, {1.0f,1.0f,1.0f},0.0f},
    Chess_Game::Vertex{{-1.0f,-1.0f,0.0f},{ -1.0f,-1.0f,0.0f} , {0.0f,0.0f}, {1.0f,1.0f,1.0f},0.0f},
    //Second triangle  
    Chess_Game::Vertex{{1.0f,-1.0f,0.0f} ,{ 1.0f,-1.0f,0.0f} , {1.0f,0.0f} , {1.0f,1.0f,1.0f},0.0f},
    Chess_Game::Vertex{{1.0f,1.0f,0.0f}  ,{1.0f,1.0f,0.0f}   , {1.0f,1.0f} , {1.0f,1.0f,1.0f},0.0f},
};


Chess_Game::BatchRenderer::BatchRenderer()
{
    SetupBatch(m_TexturedQuadBatch);
    SetupBatch(m_CircleQuadBatch);

    m_TexturedQuadBatch.batch_shader =
        std::make_unique<ShaderClass>("D:/c++/OpenGl/Chess-OpenGL/Shaders/TextureShader.glsl");

    m_CircleQuadBatch.batch_shader =
        std::make_unique<ShaderClass>("D:/c++/OpenGl/Chess-OpenGL/Shaders/CircleShader.glsl");
 }

void Chess_Game::BatchRenderer::Push(const glm::vec3& position,
    const glm::vec2& scale, const glm::vec3& object_color, Texture texture_index)
{
    size_t texture_binding_point = m_TextureBatcher.PushTextureForRendering(texture_index);

    GLuint quad_index_data[6] = {
        0,1,2,
        2,3,0
    };

    Vertex quad_vertex_data_copy[kBaseQuadVertexData.size()] = {};
    std::copy(std::begin(kBaseQuadVertexData), std::end(kBaseQuadVertexData), std::begin(quad_vertex_data_copy));
    glm::vec2 min{}, max{};
    float shape_aspect = scale.x/ scale.y;
    for (auto & vertex : quad_vertex_data_copy)
    {
        vertex.world_position *= glm::vec3(scale.x, scale.y,1.0f);
        vertex.world_position += position;
        vertex.color = object_color;
        vertex.texture_sampler_index = static_cast<float>(texture_binding_point);
    }
 
    memcpy(m_TexturedQuadBatch.render_data.vertex_batch_pointer, quad_vertex_data_copy, sizeof(quad_vertex_data_copy));
     
    m_TexturedQuadBatch.render_data.vertex_batch_pointer += BatchRendererData::kSingleQuadVertexCount;

    size_t index_data_offset =
        (m_TexturedQuadBatch.render_data.index_batch_pointer - m_TexturedQuadBatch.render_data.index_batch_array)/ BatchRendererData::kSingleQuadIndexCount;
    for (auto& index : quad_index_data)
    {
        index += 4 * index_data_offset;
    }
    memcpy(m_TexturedQuadBatch.render_data.index_batch_pointer, quad_index_data, sizeof(quad_index_data));


    m_TexturedQuadBatch.render_data.index_batch_pointer += BatchRendererData::kSingleQuadIndexCount;

}

void Chess_Game::BatchRenderer::PushCircle(const glm::vec3& position, const glm::vec2& scale, const glm::vec3& object_color)
{
 
    GLuint quad_index_data[6] = {
        0,1,2,
        2,3,0
    };

    Vertex quad_vertex_data_copy[kBaseQuadVertexData.size()] = {};
    std::copy(std::begin(kBaseQuadVertexData), std::end(kBaseQuadVertexData), std::begin(quad_vertex_data_copy));

    for (auto& vertex : quad_vertex_data_copy)
    {
        vertex.world_position *= glm::vec3(scale.x, scale.y, 1.0f);
        vertex.world_position += position;
        vertex.color = object_color;
    }

    memcpy(m_CircleQuadBatch.render_data.vertex_batch_pointer, quad_vertex_data_copy, sizeof(quad_vertex_data_copy));

    m_CircleQuadBatch.render_data.vertex_batch_pointer += BatchRendererData::kSingleQuadVertexCount;

    size_t index_data_offset = 
        (m_CircleQuadBatch.render_data.index_batch_pointer - m_CircleQuadBatch.render_data.index_batch_array) / BatchRendererData::kSingleQuadIndexCount;
    for (auto& index : quad_index_data)
    {
        index += 4 * index_data_offset;
    }
    memcpy(m_CircleQuadBatch.render_data.index_batch_pointer, quad_index_data, sizeof(quad_index_data));


    m_CircleQuadBatch.render_data.index_batch_pointer += BatchRendererData::kSingleQuadIndexCount;

}

void Chess_Game::BatchRenderer::DrawCircleBatch(const glm::mat4& projection)
{
    const char* projection_uniform_name = "orthographicProjection";

    size_t vertex_data_size = 
        (m_CircleQuadBatch.render_data.vertex_batch_pointer - m_CircleQuadBatch.render_data.vertex_batch_array) * sizeof(Vertex);
    size_t index_data_size = 
        (m_CircleQuadBatch.render_data.index_batch_pointer - m_CircleQuadBatch.render_data.index_batch_array) * sizeof(GLuint);
    size_t index_count = 
        (m_CircleQuadBatch.render_data.index_batch_pointer - m_CircleQuadBatch.render_data.index_batch_array);

    if (index_count == 0) return;

    m_CircleQuadBatch.batch_shader->UseProgram();
    m_CircleQuadBatch.batch_shader->SetUniform4x4Matrix(projection_uniform_name, projection);

    glBindVertexArray(m_CircleQuadBatch.gpu_data.vertex_attribute_array_handle);

    glBindBuffer(GL_ARRAY_BUFFER, m_CircleQuadBatch.gpu_data.vertex_buffer_handle);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_data_size, m_CircleQuadBatch.render_data.vertex_batch_array);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_CircleQuadBatch.gpu_data.index_buffer_handle);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, index_data_size, m_CircleQuadBatch.render_data.index_batch_array);

    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    BeginBatch(m_CircleQuadBatch);
}

void Chess_Game::BatchRenderer::DrawTextureQuadBatch(const glm::mat4& projection)
{
    const char* sampler_array_uniform_name = "u_Textures";
    const char* projection_uniform_name = "orthographicProjection";

    size_t vertex_data_size = 
        (m_TexturedQuadBatch.render_data.vertex_batch_pointer - m_TexturedQuadBatch.render_data.vertex_batch_array) * sizeof(Vertex);
    size_t index_data_size = 
        (m_TexturedQuadBatch.render_data.index_batch_pointer - m_TexturedQuadBatch.render_data.index_batch_array) * sizeof(GLuint);
    size_t index_count = 
        (m_TexturedQuadBatch.render_data.index_batch_pointer - m_TexturedQuadBatch.render_data.index_batch_array);

    if (index_count == 0) return;

    m_TexturedQuadBatch.batch_shader->UseProgram();
    m_TextureBatcher.BindTextures();

    m_TexturedQuadBatch.batch_shader->SetUniform4x4Matrix(projection_uniform_name, projection);
    m_TexturedQuadBatch.batch_shader->SetSampler2DArray(sampler_array_uniform_name, m_TextureBatcher.GetBoundTexturesSlots().data(),
        m_TextureBatcher.GetBoundTexturesCount());

    glBindVertexArray(m_TexturedQuadBatch.gpu_data.vertex_attribute_array_handle);

    glBindBuffer(GL_ARRAY_BUFFER, m_TexturedQuadBatch.gpu_data.vertex_buffer_handle);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_data_size, m_TexturedQuadBatch.render_data.vertex_batch_array);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_TexturedQuadBatch.gpu_data.index_buffer_handle);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, index_data_size, m_TexturedQuadBatch.render_data.index_batch_array);

    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    BeginBatch(m_TexturedQuadBatch);
    m_TextureBatcher.Flush();
}
void Chess_Game::BatchRenderer::BeginBatch(BatchData& batch_to_begin)
{
    batch_to_begin.render_data.vertex_batch_pointer = batch_to_begin.render_data.vertex_batch_array;
    batch_to_begin.render_data.index_batch_pointer = batch_to_begin.render_data.index_batch_array;
}


Chess_Game::BatchRenderer::~BatchRenderer()
{
    //TODO: Change this to be in a batchData destructor

    FreeBatchMemory(m_CircleQuadBatch);
    FreeBatchMemory(m_TexturedQuadBatch);
}
void Chess_Game::BatchRenderer::FreeBatchMemory(BatchData& batch_to_free)
{
    delete[] batch_to_free.render_data.vertex_batch_array;
    delete[] batch_to_free.render_data.index_batch_array;

    glDeleteBuffers(1, &batch_to_free.gpu_data.vertex_buffer_handle);
    glDeleteBuffers(1, &batch_to_free.gpu_data.index_buffer_handle);
    glDeleteVertexArrays(1, &batch_to_free.gpu_data.vertex_attribute_array_handle);

}

void Chess_Game::BatchRenderer::SetupBatch(BatchData& batch_to_setup)
{
    glGenVertexArrays(1, &batch_to_setup.gpu_data.vertex_attribute_array_handle);

    glGenBuffers(1, &batch_to_setup.gpu_data.vertex_buffer_handle);
    glGenBuffers(1, &batch_to_setup.gpu_data.index_buffer_handle);

    glBindVertexArray(batch_to_setup.gpu_data.vertex_attribute_array_handle);

    glBindBuffer(GL_ARRAY_BUFFER, batch_to_setup.gpu_data.vertex_buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, BatchRendererData::kBatchVertexArraySize * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch_to_setup.gpu_data.index_buffer_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, BatchRendererData::kBatchIndexArraySize * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, local_position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, world_position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_sampler_index));
    glEnableVertexAttribArray(4);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    batch_to_setup.render_data.vertex_batch_array = new Vertex[BatchRendererData::kBatchVertexArraySize];
    batch_to_setup.render_data.vertex_batch_pointer = batch_to_setup.render_data.vertex_batch_array;

    batch_to_setup.render_data.index_batch_array = new GLuint[BatchRendererData::kBatchIndexArraySize];
    batch_to_setup.render_data.index_batch_pointer = batch_to_setup.render_data.index_batch_array;
}

