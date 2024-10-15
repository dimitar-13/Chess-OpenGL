#pragma once
#include "TextureEnum.h"
#include <ft2build.h>
#include "Chess_pch.h"
#include FT_FREETYPE_H 

namespace Chess_Game
{
    struct FontCharacterData
    {
        TextureRegion character_uv_coords;
        glm::ivec2 Size;     
        glm::ivec2 Bearing;  
        unsigned int Advance;  
    };
    struct FontLibraryData
    {
        FT_Library library_handle;
        FT_Face font_face;
    };

    class FontLoader
    {
    public:
        FontLoader(const char* path_of_font_to_load);
        ~FontLoader();
        bool IsLoadedSuccessfully()const { return m_IsLoadedSuccessfully; }
        void LoadFontDataInMemory(size_t font_pixel_width,size_t font_pixel_height,
            GLuint& glyph_atlas_text_ref);
        const glm::vec2& GetFontAtlasSize()const { return m_FontAtlasSize; }
        const std::unordered_map<unsigned char, FontCharacterData>& GetCharMap()const { return m_FontCharacterHash; }
        size_t GetFontLineSpacing()const {return m_FontLibData.font_face->size->metrics.height; }
    private:
        glm::vec2 m_FontAtlasSize;
        std::unordered_map<unsigned char, FontCharacterData> m_FontCharacterHash;
        bool m_IsLoadedSuccessfully = false;
        FontLibraryData m_FontLibData;
    };



    class TextFont
    {
    public:
        TextFont();
        ~TextFont();
        Texture GetGlyphCoords(unsigned char glyph);
        FontCharacterData GetGlyphMetrix(unsigned char glyph);
        size_t GetFontLineSpacing()const { return m_FontLineSpacing; }
        bool IsGlyphPresentInFont(unsigned char glyph)const { return m_FontCharacterHash.find(glyph) != m_FontCharacterHash.end(); }
    private:
        //void FreeResource();
    private:
        size_t m_FontLineSpacing = 0;

        std::unordered_map<unsigned char, FontCharacterData> m_FontCharacterHash;
        GLuint m_GlyphAtlas;
    };

}