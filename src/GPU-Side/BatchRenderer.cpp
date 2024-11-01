#include "BatchRenderer.h"
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
    :m_ApplicationAssetLoader(application_asset_loader),
    m_TexturedQuadBatch("D:/c++/OpenGl/Chess-OpenGL/Shaders/TextureShader.glsl"),
    m_TextQuadBatch("D:/c++/OpenGl/Chess-OpenGL/Shaders/FontRenderingShader.glsl"),
    m_CircleQuadBatch("D:/c++/OpenGl/Chess-OpenGL/Shaders/CircleShader.glsl")
{

    m_MousePickingFramebuffer = std::make_shared<IntFramebuffer>(window_size);

    m_MousePickingShader =
        std::make_unique<ShaderClass>("D:/c++/OpenGl/Chess-OpenGL/Shaders/MousePickingShader.glsl");

 }


void Chess_Game::BatchRenderer::PushCircle(const glm::vec3& position, const glm::vec2& scale, const glm::vec3& object_color)
{
    std::array<Vertex, kBaseQuadVertexData.size()>quad_vertex_data_copy{};

    std::copy(std::begin(kBaseQuadVertexData), std::end(kBaseQuadVertexData), std::begin(quad_vertex_data_copy));

    for (auto& vertex : quad_vertex_data_copy)
    {
        vertex.world_position *= glm::vec3(scale.x, scale.y, 1.0f);
        vertex.world_position += position;
        vertex.color = object_color;
    }
    m_CircleQuadBatch.PushToBatch(quad_vertex_data_copy);

}

void Chess_Game::BatchRenderer::PushTexturedQuad(size_t object_id,
    const glm::vec3& position, const glm::vec2& scale, 
    const glm::vec3& object_color, const std::string& texture_name)
{
    std::array<Vertex, kBaseQuadVertexData.size()>quad_vertex_data_copy{};

    const Texture& texture_data = m_ApplicationAssetLoader->GetTextureAsset(texture_name);
    size_t texture_binding_point = m_TextureBatcher.PushTextureForRendering(texture_data.texture_handle);

    std::copy(std::begin(kBaseQuadVertexData), std::end(kBaseQuadVertexData), std::begin(quad_vertex_data_copy));

    for (auto& vertex : quad_vertex_data_copy)
    {
        vertex.object_index = object_id;
        vertex.world_position *= glm::vec3(scale.x, scale.y, 0.0f);
        vertex.world_position += position;
        vertex.color = object_color;
        vertex.texture_sampler_index = static_cast<float>(texture_binding_point);
    }
    if (texture_data.is_texture_atlas)
    {
        quad_vertex_data_copy[0].uv = { texture_data.texture_region.start.x,texture_data.texture_region.end.y };
        quad_vertex_data_copy[1].uv = texture_data.texture_region.start;
        quad_vertex_data_copy[2].uv = { texture_data.texture_region.end.x,texture_data.texture_region.start.y };
        quad_vertex_data_copy[3].uv = texture_data.texture_region.end;
    }
    m_TexturedQuadBatch.PushToBatch(quad_vertex_data_copy);

}

void Chess_Game::BatchRenderer::PushText(const std::string& text_to_draw,
    const glm::vec3& position, const glm::vec2& scale,
    const glm::vec3& text_color,std::shared_ptr<TextFont> font_to_use)
{
    size_t texture_binding_point = m_TextureBatcher.PushTextureForRendering(font_to_use->GetGlyphCoords('a').texture_handle);

  
    size_t scaled_line_spacing = font_to_use->GetFontLineSpacing();

    glm::vec2 start_pos = position;
    glm::vec2 origin = start_pos;

    glm::ivec2 glyph_offset{};

    for (size_t i = 0; i < text_to_draw.size(); i++)
    {
        if (text_to_draw[i] == '\n')
        {
            origin.y -= (scaled_line_spacing >> 6) * scale.y;
            origin.x = start_pos.x;
            continue;

        }

        if (!font_to_use->IsGlyphPresentInFont(text_to_draw[i]))
        {
            continue;
        }

        auto& character_metrics = font_to_use->GetGlyphMetrix(text_to_draw[i]);

        glm::vec2 glyph_pos = origin;

        glm::vec2 character_drawable_pos;
        glm::vec2 character_drawable_size;
        glm::vec2 max_bbox, min_bbox;


        max_bbox.x = glyph_pos.x + (character_metrics.Bearing.x + character_metrics.Size.x) * scale.x;
        max_bbox.y = glyph_pos.y + character_metrics.Bearing.y * scale.y;

        min_bbox.x = glyph_pos.x + character_metrics.Bearing.x * scale.x;
        min_bbox.y = glyph_pos.y - (character_metrics.Size.y - character_metrics.Bearing.y) * scale.y;

        character_drawable_size = (max_bbox - min_bbox) / 2.0f;
        character_drawable_pos = min_bbox + character_drawable_size;

        Texture glyph_text = font_to_use->GetGlyphCoords(text_to_draw[i]);

        std::array<Vertex, kBaseQuadVertexData.size()>quad_vertex_data_copy{};
        std::copy(std::begin(kBaseQuadVertexData), std::end(kBaseQuadVertexData), std::begin(quad_vertex_data_copy));

        for (auto& vertex : quad_vertex_data_copy)
        {
            //vertex.object_index = 0;
            vertex.world_position *= glm::vec3(character_drawable_size.x, character_drawable_size.y, 0.0f);
            vertex.world_position += glm::vec3(character_drawable_pos, position.z);
            vertex.color = text_color;
            vertex.texture_sampler_index = static_cast<float>(texture_binding_point);
        }
        quad_vertex_data_copy[0].uv = { glyph_text.texture_region.start.x,glyph_text.texture_region.end.y };
        quad_vertex_data_copy[1].uv = glyph_text.texture_region.start;
        quad_vertex_data_copy[2].uv = { glyph_text.texture_region.end.x,glyph_text.texture_region.start.y };
        quad_vertex_data_copy[3].uv = glyph_text.texture_region.end;

        m_TextQuadBatch.PushToBatch(quad_vertex_data_copy);

        origin.x += (character_metrics.Advance >> 6) * scale.x;
    }
}

void Chess_Game::BatchRenderer::DrawCircleBatch(const glm::mat4& projection)
{
   const char* projection_uniform_name = "orthographicProjection";
  
   m_CircleQuadBatch.GetPipelineShader().UseProgram();
   m_CircleQuadBatch.GetPipelineShader().SetUniform4x4Matrix(projection_uniform_name, projection);

   m_CircleQuadBatch.Draw();

   m_CircleQuadBatch.FlushBatch();

}

void Chess_Game::BatchRenderer::DrawTextBatch(const glm::mat4& projection)
{
    const char* sampler_array_uniform_name = "u_Textures";
    const char* projection_uniform_name = "orthographicProjection";

    m_TextureBatcher.BindTextures();

    m_TextQuadBatch.GetPipelineShader().UseProgram();
    m_TextQuadBatch.GetPipelineShader().SetUniform4x4Matrix(projection_uniform_name, projection);
    m_TextQuadBatch.GetPipelineShader().SetSampler2DArray(sampler_array_uniform_name,
        m_TextureBatcher.GetBoundTexturesSlots().data(), m_TextureBatcher.GetBoundTexturesCount());

    m_TextQuadBatch.Draw();

    m_TextQuadBatch.FlushBatch();
}


void Chess_Game::BatchRenderer::DrawTextureQuadBatch(const glm::mat4& projection,bool output_drawable_id)
{
    const char* sampler_array_uniform_name = "u_Textures";
    const char* projection_uniform_name = "orthographicProjection";

    m_TextureBatcher.BindTextures();

    if (output_drawable_id)
    {
        m_MousePickingFramebuffer->BindFramebuffer();

        m_MousePickingShader->UseProgram();
        m_MousePickingShader->SetUniform4x4Matrix(projection_uniform_name, projection);
        m_MousePickingShader->SetSampler2DArray(sampler_array_uniform_name,
            m_TextureBatcher.GetBoundTexturesSlots().data(), m_TextureBatcher.GetBoundTexturesCount());

        m_TexturedQuadBatch.Draw();
    }
    IntFramebuffer::BindDefaultFramebuffer();


    m_TexturedQuadBatch.GetPipelineShader().UseProgram();
    m_TexturedQuadBatch.GetPipelineShader().SetUniform4x4Matrix(projection_uniform_name, projection);
    m_TexturedQuadBatch.GetPipelineShader().SetSampler2DArray(sampler_array_uniform_name,
        m_TextureBatcher.GetBoundTexturesSlots().data(),m_TextureBatcher.GetBoundTexturesCount());

    m_TexturedQuadBatch.Draw();

    m_TexturedQuadBatch.FlushBatch();

}


