#include "BatchRenderer.h"
#include "Core/Chess_pch.h"
#include "Logging/Logger.h"
#include "Core/Application.h"
static const std::array<Chess_Game::Vertex,4> kBaseQuadVertexData = {
    //First triangle
    Chess_Game::Vertex{0,{-1.0f,1.0f,0.0f} ,{ -1.0f,1.0f,0.0f } , {0.0f,1.0f}, {1.0f,1.0f,1.0f},0.0f},
    Chess_Game::Vertex{0,{-1.0f,-1.0f,0.0f},{ -1.0f,-1.0f,0.0f} , {0.0f,0.0f}, {1.0f,1.0f,1.0f},0.0f},
    //Second triangle  
    Chess_Game::Vertex{0,{1.0f,-1.0f,0.0f} ,{ 1.0f,-1.0f,0.0f} , {1.0f,0.0f} , {1.0f,1.0f,1.0f},0.0f},
    Chess_Game::Vertex{0,{1.0f,1.0f,0.0f}  ,{1.0f,1.0f,0.0f}   , {1.0f,1.0f} , {1.0f,1.0f,1.0f},0.0f},
};


Chess_Game::BatchRenderer::BatchRenderer(Size2D window_size, std::shared_ptr<AssetLoader> application_asset_loader)
    :m_ApplicationAssetLoader(application_asset_loader)
{
    SetupBatch(m_TexturedQuadBatch);
    SetupBatch(m_CircleQuadBatch);

    m_MousePickingFramebuffer = std::make_shared<IntFramebuffer>(window_size);

    m_MousePickingShader =
        std::make_unique<ShaderClass>("D:/c++/OpenGl/Chess-OpenGL/Shaders/MousePickingShader.glsl");

    m_TexturedQuadBatch.batch_shader =
        std::make_unique<ShaderClass>("D:/c++/OpenGl/Chess-OpenGL/Shaders/TextureShader.glsl");

    m_CircleQuadBatch.batch_shader =
        std::make_unique<ShaderClass>("D:/c++/OpenGl/Chess-OpenGL/Shaders/CircleShader.glsl");
 }

void Chess_Game::BatchRenderer::PushDrawable(size_t object_id,const glm::vec3& position,
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
        vertex.object_index = object_id;
        vertex.world_position *= glm::vec3(scale.x, scale.y,0.0f);
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

    m_CircleQuadBatch.batch_shader->UseProgram();
    m_CircleQuadBatch.batch_shader->SetUniform4x4Matrix(projection_uniform_name, projection);

    Draw(*m_CircleQuadBatch.batch_shader, m_CircleQuadBatch);

    BeginBatch(m_CircleQuadBatch);
}

void Chess_Game::BatchRenderer::SortBatch(BatchData& batch_to_sort)
{
    std::sort(batch_to_sort.batch_drawables.begin(), batch_to_sort.batch_drawables.end(),
        [](std::weak_ptr<Drawable>drawable_1, std::weak_ptr<Drawable>drawable_2)
        {
            if(drawable_1.expired()) return false;
            if (drawable_2.expired()) return true;

            std::shared_ptr<Drawable> drawable_1_shared = drawable_1.lock();
            std::shared_ptr<Drawable> drawable_2_shared = drawable_2.lock();

            return drawable_1_shared->GetPosition().z < drawable_2_shared->GetPosition().z;
        });
    for (const auto& drawable_weak : batch_to_sort.batch_drawables)
    {
        if (auto drawable_shared = drawable_weak.lock())
        {
            PushDrawable(drawable_shared->GetDrawableID(), drawable_shared->GetPosition(),
                drawable_shared->GetScale(), drawable_shared->GetColor(),
                m_ApplicationAssetLoader->GetTextureAsset(drawable_shared->GetDrawableTextureName()));
        }
    }
}

void Chess_Game::BatchRenderer::DrawTextureQuadBatch(const glm::mat4& projection,bool output_drawable_id)
{
    const char* sampler_array_uniform_name = "u_Textures";
    const char* projection_uniform_name = "orthographicProjection";

    SortBatch(m_TexturedQuadBatch);

    m_TextureBatcher.BindTextures();

    if (output_drawable_id)
    {
        m_MousePickingShader->UseProgram();
        m_MousePickingShader->SetUniform4x4Matrix(projection_uniform_name, projection);
        m_MousePickingShader->SetSampler2DArray(sampler_array_uniform_name, m_TextureBatcher.GetBoundTexturesSlots().data(),
            m_TextureBatcher.GetBoundTexturesCount());

        m_MousePickingFramebuffer->BindFramebuffer();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDisable(GL_BLEND);
        glDisable(GL_DITHER);

        Draw(*m_MousePickingShader, m_TexturedQuadBatch);

        IntFramebuffer::BindDefaultFramebuffer();

        glEnable(GL_BLEND);
        glEnable(GL_DITHER);
    }


    m_TexturedQuadBatch.batch_shader->UseProgram();
    m_TexturedQuadBatch.batch_shader->SetUniform4x4Matrix(projection_uniform_name, projection);
    m_TexturedQuadBatch.batch_shader->SetSampler2DArray(sampler_array_uniform_name, m_TextureBatcher.GetBoundTexturesSlots().data(),
        m_TextureBatcher.GetBoundTexturesCount());

    Draw(*m_TexturedQuadBatch.batch_shader, m_TexturedQuadBatch);

    BeginBatch(m_TexturedQuadBatch);
    m_TextureBatcher.Flush();
}

Chess_Game::BatchDrawData Chess_Game::BatchRenderer::CalculateBatchDrawData(BatchData& batch_to_cacl)
{
    BatchDrawData result{};
    result.vertex_count=
        (batch_to_cacl.render_data.vertex_batch_pointer - batch_to_cacl.render_data.vertex_batch_array);
    result.index_count =
        (batch_to_cacl.render_data.index_batch_pointer - batch_to_cacl.render_data.index_batch_array);
    return result;

}
void Chess_Game::BatchRenderer::Draw(ShaderClass& shader_to_use, BatchData& batch_data_to_use)
{
    BatchDrawData draw_data = CalculateBatchDrawData(batch_data_to_use);

    if (draw_data.index_count == 0) return;

    glBindVertexArray(batch_data_to_use.gpu_data.vertex_attribute_array_handle);

    glBindBuffer(GL_ARRAY_BUFFER, batch_data_to_use.gpu_data.vertex_buffer_handle);
    glBufferSubData(GL_ARRAY_BUFFER, 0, draw_data.vertex_count * sizeof(Vertex), batch_data_to_use.render_data.vertex_batch_array);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch_data_to_use.gpu_data.index_buffer_handle);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, draw_data.index_count * sizeof(GLuint), batch_data_to_use.render_data.index_batch_array);

    shader_to_use.UseProgram();
    glDrawElements(GL_TRIANGLES, draw_data.index_count, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    IntFramebuffer::BindDefaultFramebuffer();

}


void Chess_Game::BatchRenderer::BeginBatch(BatchData& batch_to_begin)
{
    batch_to_begin.render_data.vertex_batch_pointer = batch_to_begin.render_data.vertex_batch_array;
    batch_to_begin.render_data.index_batch_pointer = batch_to_begin.render_data.index_batch_array;

    batch_to_begin.batch_drawables.clear();
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

    batch_to_setup.render_data.vertex_batch_array = new Vertex[BatchRendererData::kBatchVertexArraySize];
    batch_to_setup.render_data.vertex_batch_pointer = batch_to_setup.render_data.vertex_batch_array;

    batch_to_setup.render_data.index_batch_array = new GLuint[BatchRendererData::kBatchIndexArraySize];
    batch_to_setup.render_data.index_batch_pointer = batch_to_setup.render_data.index_batch_array;
}

