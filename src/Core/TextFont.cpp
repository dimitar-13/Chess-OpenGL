#include "D:/c++/OpenGl/Chess-OpenGL/build/CMakeFiles/Chess.dir/Debug/cmake_pch.hxx"
#include "TextFont.h"
#include "Logging/Logger.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

static const char* kApplicationFontPath = "D:/c++/OpenGl/Chess-OpenGL/resources/fonts/arial.ttf";

Chess_Game::TextFont::TextFont()
{
    FontLoader font_loader(kApplicationFontPath);

    font_loader.LoadFontDataInMemory(0,48, m_GlyphAtlas);
    if (!font_loader.IsLoadedSuccessfully())
    {
        CHESS_LOG_ERROR("Failed to load defualt text font.");
        return;
    }

    glm::vec2 font_atlas_size = font_loader.GetFontAtlasSize();
    m_FontCharacterHash = font_loader.GetCharMap();
    m_FontLineSpacing = font_loader.GetFontLineSpacing();

}

Chess_Game::TextFont::~TextFont()
{
    glDeleteTextures(1, &m_GlyphAtlas);
}

Chess_Game::Texture Chess_Game::TextFont::GetGlyphCoords(unsigned char glyph)
{
    if (m_FontCharacterHash.find(glyph) == m_FontCharacterHash.end())
    {
        return Texture();
    }

    Texture result;

    result.texture_handle = m_GlyphAtlas;
    result.texture_region = m_FontCharacterHash.at(glyph).character_uv_coords;

    return result;
}

Chess_Game::FontCharacterData Chess_Game::TextFont::GetGlyphMetrix(unsigned char glyph)
{
    if (m_FontCharacterHash.find(glyph) == m_FontCharacterHash.end())
    {
        return FontCharacterData();
    }

    return m_FontCharacterHash.at(glyph);
}


Chess_Game::FontLoader::FontLoader(const char* path_of_font_to_load)
{
    if (FT_Init_FreeType(&m_FontLibData.library_handle))
    {
        CHESS_LOG_ERROR("Could not init free type lib.");
        return;
    }

    if (FT_New_Face(m_FontLibData.library_handle, path_of_font_to_load, 0, &m_FontLibData.font_face))
    {
        CHESS_LOG_ERROR("Could not load the selected application font free type lib.");
        return;
    }

    m_IsLoadedSuccessfully = true;

}

Chess_Game::FontLoader::~FontLoader()
{
    FT_Done_Face(m_FontLibData.font_face);
    FT_Done_FreeType(m_FontLibData.library_handle);
}

void Chess_Game::FontLoader::LoadFontDataInMemory(size_t font_pixel_width, size_t font_pixel_height,
   GLuint& glyph_atlas_text_ref )
{
    constexpr unsigned char kFinalCharToLoad = 128;

    FT_Set_Pixel_Sizes(m_FontLibData.font_face, font_pixel_width, font_pixel_height);

    //Sort by size/area


    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    size_t texture_width = 0;
    size_t texture_height = 0;

    for (unsigned char character = 0; character < kFinalCharToLoad; character++)
    {
        if (FT_Load_Char(m_FontLibData.font_face, character, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        texture_width += m_FontLibData.font_face->glyph->bitmap.width;
        texture_height = std::max<size_t>(texture_height, m_FontLibData.font_face->glyph->bitmap.rows);
    }

    glGenTextures(1, &glyph_atlas_text_ref);

    glBindTexture(GL_TEXTURE_2D, glyph_atlas_text_ref);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, texture_width, texture_height, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    size_t texture_atlas_x_offset = 0;
    size_t texture_atlas_y_offset = 0;

    for (unsigned char character = 0; character < kFinalCharToLoad; character++)
    {
        if (FT_Load_Char(m_FontLibData.font_face, character, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        glTexSubImage2D(GL_TEXTURE_2D,
            0,
            texture_atlas_x_offset, 0,
            m_FontLibData.font_face->glyph->bitmap.width,
            m_FontLibData.font_face->glyph->bitmap.rows,
            GL_RED, GL_UNSIGNED_BYTE,
            m_FontLibData.font_face->glyph->bitmap.buffer);


        TextureRegion char_react;
        char_react.start.x = static_cast<float>(texture_atlas_x_offset) / static_cast<float>(texture_width);
        char_react.start.y = static_cast<float>(m_FontLibData.font_face->glyph->bitmap.rows) /
            static_cast<float>(texture_height);

        char_react.end.x =
            static_cast<float>((texture_atlas_x_offset + m_FontLibData.font_face->glyph->bitmap.width)) /
            static_cast<float>(texture_width);
        char_react.end.y = static_cast<float>(0) / static_cast<float>(texture_height);


        FontCharacterData character_data =
        {
            char_react,
            glm::ivec2(m_FontLibData.font_face->glyph->bitmap.width,
            m_FontLibData.font_face->glyph->bitmap.rows),
            glm::ivec2(m_FontLibData.font_face->glyph->bitmap_left,
            m_FontLibData.font_face->glyph->bitmap_top),
            m_FontLibData.font_face->glyph->advance.x
        };
        m_FontCharacterHash.emplace(character, character_data);

        texture_atlas_x_offset += m_FontLibData.font_face->glyph->bitmap.width;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

}
