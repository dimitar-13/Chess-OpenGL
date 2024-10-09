#include "D:/c++/OpenGl/Chess-OpenGL/build/CMakeFiles/Chess.dir/Debug/cmake_pch.hxx"
#include "TextUI.h"

void Chess_Game::TextUI::Draw(BatchRenderer& renderer)
{
     size_t scaled_line_spacing = m_TextFont->GetFontLineSpacing();

    glm::vec2 start_pos = this->GetScreenPos();
    glm::vec2 origin = start_pos;

    glm::ivec2 glyph_offset{};

    for (auto character : m_Text)
    {
        if (character == '\n')
        {
            origin.y -= (scaled_line_spacing >> 6) * m_TextScale.y;
            origin.x = start_pos.x;
            continue;

        }

        if (!m_TextFont->IsGlyphPresentInFont(character))
        {
            continue;
        }

        auto& character_metrics = m_TextFont->GetGlyphMetrix(character);

        glm::vec2 glyph_pos = origin;

        glm::vec2 character_drawable_pos;
        glm::vec2 character_drawable_size;
        glm::vec2 max_bbox, min_bbox;


        max_bbox.x = glyph_pos.x + (character_metrics.Bearing.x + character_metrics.Size.x) * m_TextScale.x;
        max_bbox.y = glyph_pos.y + character_metrics.Bearing.y * m_TextScale.y;

        min_bbox.x = glyph_pos.x + character_metrics.Bearing.x * m_TextScale.x;
        min_bbox.y = glyph_pos.y - (character_metrics.Size.y - character_metrics.Bearing.y) * m_TextScale.y;

        character_drawable_size = (max_bbox - min_bbox) / 2.0f;
        character_drawable_pos = min_bbox + character_drawable_size;

        Texture glyph_text = m_TextFont->GetGlyphCoords(character);

       //quad_raw_data[0].position = { min_bbox.x,max_bbox.y };
       //quad_raw_data[0].uv_coords = { character_data.character_uv_coords.start.x, character_data.character_uv_coords.end.y };
       //
       //quad_raw_data[1].position = min_bbox;
       //quad_raw_data[1].uv_coords = { character_data.character_uv_coords.start.x, character_data.character_uv_coords.start.y };
       //
       //quad_raw_data[2].position = { max_bbox.x,min_bbox.y };
       //quad_raw_data[2].uv_coords = { character_data.character_uv_coords.end.x, character_data.character_uv_coords.start.y };
       //
       //quad_raw_data[3].position = max_bbox;
       //quad_raw_data[3].uv_coords = { character_data.character_uv_coords.end.x, character_data.character_uv_coords.end.y };
       //

       renderer.PushDrawable(0,glm::vec3(character_drawable_pos,.0), character_drawable_size,
           glm::vec3(1), glyph_text);

        origin.x += (character_metrics.Advance >> 6) * m_TextScale.x;
    }

}
