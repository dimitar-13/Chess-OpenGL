#include "AssetLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Logging/Logger.h"

Chess_Game::AssetLoader::AssetLoader()
{
    const TexturePathNamePair texture_paths [] =
    {
        {" ",TextureName_kWhiteTexture},
        {"D:/c++/OpenGl/Chess-OpenGL/resources/Chess_board2.jpg",TextureName_kBoard},
        {"D:/c++/OpenGl/Chess-OpenGL/resources/VendorChessPieces/OpenGameArt/King.png",   TextureName_kKing},
        {"D:/c++/OpenGl/Chess-OpenGL/resources/VendorChessPieces/OpenGameArt/Knight.png", TextureName_kKnight},
        {"D:/c++/OpenGl/Chess-OpenGL/resources/VendorChessPieces/OpenGameArt/Pawn.png",   TextureName_kPawn},
        {"D:/c++/OpenGl/Chess-OpenGL/resources/VendorChessPieces/OpenGameArt/Queen.png",  TextureName_kQueen},
        {"D:/c++/OpenGl/Chess-OpenGL/resources/VendorChessPieces/OpenGameArt/Rook.png",   TextureName_kRook},
        {"D:/c++/OpenGl/Chess-OpenGL/resources/VendorChessPieces/OpenGameArt/Bishop.png", TextureName_kBishop },
        {"D:/c++/OpenGl/Chess-OpenGL/resources/Test_Button.png",                          TextureName_kButton },
        {"D:/c++/OpenGl/Chess-OpenGL/resources/Chess_logo.png",                           TextureName_kGameLogo},
        {"D:/c++/OpenGl/Chess-OpenGL/resources/RestButtonTest.png",                       TextureName_kResetButton},
        {"D:/c++/OpenGl/Chess-OpenGL/resources/HomeButtonTest.png",                       TextureName_kHomeButton }
    };
    size_t loaded_asset_count = sizeof(texture_paths)/ sizeof(texture_paths[0]);

    m_AssetTextureArray.resize(loaded_asset_count);
    glGenTextures(loaded_asset_count, &m_AssetTextureArray[0]);

    stbi_set_flip_vertically_on_load(true);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (size_t i = 0; i < loaded_asset_count; i++)
    {

        TextureReadData texture_data;

        if (i == 0)
        {
            texture_data.is_texture_valid = true;
            texture_data.texture_size = Size2D{1,1};
            texture_data.texture_format = GL_RGBA;
            texture_data.texture_source =
            {
                255,255,255,255
            };
        }
        else
        {
            texture_data = ReadTexture(texture_paths[i].texture_path);
        }
        if (!texture_data.is_texture_valid)
            continue;

        glBindTexture(GL_TEXTURE_2D, m_AssetTextureArray[i]);


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);


        glTexImage2D(GL_TEXTURE_2D, 0, texture_data.texture_format,
            texture_data.texture_size.width, texture_data.texture_size.height,
            0, texture_data.texture_format, GL_UNSIGNED_BYTE, texture_data.texture_source.data());

        m_AssetTextureHash.emplace(texture_paths[i].texture_name,m_AssetTextureArray[i]);

    }
}

Chess_Game::AssetLoader::~AssetLoader()
{
    glDeleteTextures(m_AssetTextureArray.size(), m_AssetTextureArray.data());
}

Chess_Game::Texture Chess_Game::AssetLoader::GetTextureAsset(TextureName_ name_of_texture)
{
    if (m_AssetTextureHash.find(name_of_texture) == m_AssetTextureHash.end())
    {
        CHESS_LOG_WARN("Invalid enum or texture failed to load.");
        return Texture();
    }

    return m_AssetTextureHash.at(name_of_texture);
}

Chess_Game::TextureReadData Chess_Game::AssetLoader::ReadTexture(const char* texture_path)
{
    int x{}, y{};
    int component{};
    TextureReadData result{};
    auto ExtractTextureFormat = [](int component_count) {
        switch (component_count)
        {
        case 1:
            return GL_RED;
            break;
        case 2:
            return GL_RG;
            break;
        case 3:
            return GL_RGB;
            break;
        case 4:
            return GL_RGBA;
            break;
        default:
            return GL_RGBA;
            break;
        }
        };
    stbi_uc* texture_data_ptr = stbi_load(texture_path, &x, &y, &component, 0);
    result.is_texture_valid = texture_data_ptr != nullptr;

    if (!result.is_texture_valid)
    {
        CHESS_LOG_ERROR("[STBI_ERROR] Was not able to load texture at path:'{0}'.\n Reason:'{1}'", texture_path, stbi_failure_reason());
        return result;
    }

    result.texture_size = Size2D{ static_cast<size_t>(x),static_cast<size_t>(y) };
    result.texture_source.resize(result.texture_size.width * result.texture_size.height * static_cast<size_t>(component));

    result.texture_source.assign(texture_data_ptr, texture_data_ptr + result.texture_source.size());

    result.texture_format = ExtractTextureFormat(component);
    stbi_image_free(texture_data_ptr);
    return result;
}
