#pragma once
#include "TextureEnum.h"
#include <ft2build.h>
#include "Chess_pch.h"
#include FT_FREETYPE_H 

namespace Chess_Game
{

    /**
     * Drawing a string using font files
     *
     * If you search on the internet for this there might be a lot of examples on this.
     * However I will attempt to explain it quick and the approach in the project to do this.
     * @note The actual drawing logic is in the 'TextUI' class.
     *
     * When drawing a word much like on paper you have lines that you use as guidance on where to draw.
     * This is the same for font files.Every text you draw starts from origin point(2D or 3D position 
     * showing the start of the drawing).
     * 
     * Every character has the following data(the characters have more data but this is the one used here):
     *  Size - Axis-Aligned Bounding box(AABB) that can contain the character.
     *  Bearing - Offset from the origin point(where we start the draw the text).
     *  Advance - After we draw the character how much to advance on the X dir.
     * 
     * When we draw text we just draw different aspect ratio squares like this:
     * 
     *           ___   ______   ____      
     *          |   | |      | |    |  _______      
     *          |   | |      | |    | |       |
     * origin ->|___| |______| |____| |_______|
     * 
     * Say the text 'Text'.
     * We start from a position called Origin(doesn't matter where that position is).
     * The job now is to calculate the 4 points defining the AABB for the first letter 'T'.
     * @note Depending on how your draw your squares this can be different what I mean by this is 
     * you can draw:
     *  - From middle out meaning the square pos is the middle point and the size is half of 
     *    the actual size.
     *  - From one of the corners meaning the pos is located at one of the corners usually is the bottom left
     *    and the size is the actual size of the square.    
     * To see the logic of the drawing see the 'BatchRenderer.PushText()' function for calculating the 4 points.
     * 
     * 
     * Say we found the square points and now we move to next character.
     * Every character has a 'Advance' that dictates how much should we move the origin NOTE advance
     * is from the right side of the previous character:
     *   ____             ____   
     *  |    |->Advance<-|    |
     *  |    |           |    |
     *  |____|           |____|
     * 
     * So after drawing a character you must shift the origin by width of character + the advance.
     * 
     * For more detailed explanation see 'BatchRenderer.PushText()'.
     */

    /**
     * @brief Struct containing data for an individual font character.
     *
     * This struct holds the necessary information to render a single font character.
     * It includes the texture coordinates for the character in the font atlas,
     * as well as its size, bearing, and advance values for proper positioning.
     */
    struct FontCharacterData
    {
        TextureRegion character_uv_coords; ///< Uv coordinates of the glyph in the texture atlas.
        glm::ivec2 Size;                   ///< The width and height of the Axis-Aligned Bounding box contain the character.
        glm::ivec2 Bearing;                ///< The offset of the character from the origin point.
        unsigned int Advance;              ///< This value represents how far to move the cursor horizontally to the next glyph after rendering this character.It is typically in 1 / 64th of a pixel.
    };

    /**
     * @brief Struct for working with 'FreeType' lib.
     */
    struct FontLibraryData
    {
        FT_Library library_handle; ///< 'FreeType' library handle.
        FT_Face font_face;         ///< The face of the loaded font.
    };
    /**
     * @brief Class for loading font resources.
     * 
     * The class uses 'FreeType' library to read a true-type font file and create a texture atlas of it.
     * The class follows the RAII principle by initializing the 'FreeType' library and the required 
     * resources to load a font and on destruction it destroys them.
     * 
     * The class provides functionality for extracting the loaded font data with:
     * - 'GetFontAtlasSize()' gets the size of the created font atlas.
     * - 'GetCharMap()' gets the character font data hash.
     * - 'GetFontLineSpacing()' returns the font line spacing.
     * 
     * The class also provides way to check the validity of the data using the 'IsLoadedSuccessfully()'
     * function.
     */
    class FontLoader
    {
    public:
        /**
         * @brief Constructor initializing the font lib and attempting to load the specified font. 
         * 
         * The constructor initializes the 'TrueType' lib and attemps to load the specified font.
         * If both of the 'TrueType' lib and the specified font were loaded successfully it 
         * sets the 'm_IsLoadedSuccessfully' flag to true. You can use the 'IsLoadedSuccessfully()'
         * to retrieve the flag.
         * 
         * @param path_of_font_to_load Path of the font to load.
         */
        FontLoader(const char* path_of_font_to_load);
        /**
         * @brief Destructor frees the resources used by the class to load a font.
         * 
         * The function frees both the resources used by 'FreeType' lib and the loaded font.
         */
        ~FontLoader();
        /**
         * @brief Returns true if the font was loaded.
         * 
         * Use this function to validate the data of the font.
         * 
         * @returns Returns true if the font was loaded.
         */
        bool IsLoadedSuccessfully()const { return m_IsLoadedSuccessfully; }
        /**
         * @brief Function loads the font information and creates a GPU texture 2D resource(font atlas).
         * 
         * The function sets the font pixel size using the provided font_pixel_width and font_pixel_height.
         * After setting the glyph pixel size then function extract all of the font character data 
         * and creates a 2D texture atlas. The function also create a character hash that holds the 
         * character UV location in the font atlas. To get the information use the
         * 'Get<information>()' functions.
         *
         * @param font_pixel_width The pixel width to load the font if set to 0 is auto adjusted.
         * @param font_pixel_height The pixel height to load the font
         * @param glyph_atlas_text_ref Ref to retrieve the created font texture atlas.
         */
        void LoadFontDataInMemory(size_t font_pixel_width,size_t font_pixel_height,
            GLuint& glyph_atlas_text_ref);
        /**
         * @brief Gets the size of the created font atlas.
         *
         * @returns Returns the size of the created font atlas.
         */
        const glm::vec2& GetFontAtlasSize()const { return m_FontAtlasSize; }
        /**
         * @brief Gets the loaded font char charactered data hash.
         *
         * @returns Returns the font char charactered data hash.
         */
        const std::unordered_map<unsigned char, FontCharacterData>& GetCharMap()const { return m_FontCharacterHash; }
        /**
         * @brief Gets the loaded font line spacing.
         *
         * @returns Returns the loaded font line spacing.
         */
        size_t GetFontLineSpacing()const {return m_FontLibData.font_face->size->metrics.height; }
    private:
        glm::vec2 m_FontAtlasSize; ///< Loaded font texture atlas size.
        std::unordered_map<unsigned char, FontCharacterData> m_FontCharacterHash; ///< Loaded font char character data hash.
        bool m_IsLoadedSuccessfully = false;    ///< Flag indicating if the loading of the font was successful.
        FontLibraryData m_FontLibData;          ///< 'TrueType' lib font data.
    };


    /**
     * @brief  he class loads a font from a provided font file and provides a way to use the font pixel data
     * 
     *
     * The class used the 'FontLoader' class to load the font information. 
     * The font pixel data is loaded in a texture atlas.
     * The class keeps a hash map of every character with the their information.
     * A character consists of:
     *   TextureRegion - The texture atlas region containing the character.
     *   Size - Axis-Aligned Bounding box(AABB) that can contain the character     
     *   Bearing - 2D Offset from the base line.  
     *   Advance - X offset from the Size.x.
     * 
     * The class also manages the created font atlas texture resource from the 'FontLoader'
     * and destroys it at destruction.
     *                                                               ^
     * To see how to use this font class effectively see top of file |
     * 
     */
    class TextFont
    {
    public:
        /**
         * @brief Creates a font file using the 'FontLoader'.
         * 
         * Constructor loads a font file using the 'FontLoader' helper class.
         * After the load it extract the relevant font information like:
         *  - Font vertical line spacing.
         *  - Font character hash.
         *  - Font atlas texture index.
         * 
         * @param font_path_to_load The path of the font.
         */
        TextFont(const char* font_path_to_load);
        /**
         * @brief Frees the font atlas GPU resource. 
         */
        ~TextFont();
        Texture GetGlyphCoords(unsigned char glyph);
        /**
         * @brief Gets the glyph 'FontCharacterData' by a specified character.
         * 
         * @returns Returns 'FontCharacterData' corresponding to the character in the hash.
         */
        FontCharacterData GetGlyphMetrix(unsigned char glyph);
        /**
         * @brief Gets the font vertical spacing.
         *
         * @returns Returns the vertical font spacing.
         */
        size_t GetFontLineSpacing()const { return m_FontLineSpacing; }
        /**
         * @brief Returns true if the glyph is in the font.
         * @returns Returns true if the glyph is in the font.
         */
        bool IsGlyphPresentInFont(unsigned char glyph)const { return m_FontCharacterHash.find(glyph) != m_FontCharacterHash.end(); }
    private:
        size_t m_FontLineSpacing = 0; ///< The font line spacing.
        std::unordered_map<unsigned char, FontCharacterData> m_FontCharacterHash; ///< Character font data hash.
        GLuint m_GlyphAtlas; ///< The texture atlas GPU handle.
    };

}