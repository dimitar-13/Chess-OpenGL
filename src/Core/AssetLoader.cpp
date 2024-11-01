#include "AssetLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Logging/Logger.h"
#include <nlohmann/json.hpp>
#include <filesystem>
std::string Chess_Game::AssetLoader::s_WhiteTextureName = "WhiteTexture";

Chess_Game::AssetLoader::AssetLoader()
{
    namespace fs = std::filesystem;

    fs::path currentPath = fs::current_path() / ".." / "resources/";
    std::string relative_resources_path = fs::canonical(currentPath).string();

    const TexturePathNamePair texture_paths [] =
    {
        {"",s_WhiteTextureName.c_str(),false},
        {relative_resources_path + "/chess/chess_boards/ches_game_borwnish_chess_board.png","chess_game_board.jpg",false},
        {relative_resources_path + "/UI/tinyrpg/Application UI/chess_game_button_background.png","chess_game_button_background.png",false},
        {relative_resources_path + "/UI/chess_game_logo.png","chess_game_logo.png",false},
        {relative_resources_path + "/UI/tinyrpg/Application UI/chess_game_panel_background.png", "chess_game_panel_background.png",false},
        {relative_resources_path + "/UI/tinyrpg/Application UI/chess_game_pause_button.png","chess_game_pause_button.png",false },
        {relative_resources_path + "/UI/tinyrpg/Application UI/chess_game_pause_button.png","chess_game_pause_button.png",false },
        {relative_resources_path + "/UI/tinyrpg/Application UI/chess_game_close_button.png","chess_game_close_button.png",false },

        {"D:/c++/OpenGl/Chess-OpenGL/resources/chess/ChessPieces/chess_regular_set/chess_regular_set_texture_atlas.png",
        "chess_regular_set_texture_atlas",true}
        //{"D:/c++/OpenGl/Chess-OpenGL/resources/chess/ChessPieces/chess_brownish_set/chess_brownish_set_texture_atlas.png",
        //"chess_brownish_set_texture_atlas",true}
        //        
        //{"D:/c++/OpenGl/Chess-OpenGL/resources/chess/ChessPieces/chess_set/chess_set.png",
        //"chess_set",true}

    };
    size_t loaded_asset_count = sizeof(texture_paths)/ sizeof(texture_paths[0]);

    m_AssetTextureArray.resize(loaded_asset_count);
    glGenTextures(loaded_asset_count, &m_AssetTextureArray[0]);

    stbi_set_flip_vertically_on_load(true);

    for (size_t i = 0; i < loaded_asset_count; i++)
    {
        GLuint current_texture_handle;
        TextureReadData texture_data;

        if (texture_paths[i].texture_file_name == s_WhiteTextureName)
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

        current_texture_handle = m_AssetTextureArray[i];

        glBindTexture(GL_TEXTURE_2D, current_texture_handle);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
       
    
        glTexImage2D(GL_TEXTURE_2D, 0, texture_data.texture_format,
            texture_data.texture_size.width, texture_data.texture_size.height,
            0, texture_data.texture_format, GL_UNSIGNED_BYTE, texture_data.texture_source.data());


        if (texture_paths[i].is_texture_atlas)
        {
            UnpackTextureAtlas(current_texture_handle,
                glm::vec2{ texture_data.texture_size.width,texture_data.texture_size.height},
                texture_paths[i].texture_path);
            continue;
        }
        Texture texture;
        texture.texture_handle = current_texture_handle;
        texture.is_texture_atlas = false;

        m_AssetTextureHash.emplace(texture_paths[i].texture_file_name, texture);

    }
}

Chess_Game::AssetLoader::~AssetLoader()
{
    glDeleteTextures(m_AssetTextureArray.size(), m_AssetTextureArray.data());
}

const Chess_Game::Texture& Chess_Game::AssetLoader::GetTextureAsset(const std::string& name_of_texture)
{
    if (m_AssetTextureHash.find(name_of_texture) == m_AssetTextureHash.end())
    {
        CHESS_LOG_WARN("Invalid enum or texture failed to load.");
        return m_AssetTextureHash.at(s_WhiteTextureName);
    }

    return m_AssetTextureHash.at(name_of_texture);
}

Chess_Game::TextureReadData Chess_Game::AssetLoader::ReadTexture(const std::string& texture_path)
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

    stbi_uc* texture_data_ptr = stbi_load(texture_path.data(), &x, &y, &component, 0);
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

void Chess_Game::AssetLoader::UnpackTextureAtlas(GLuint texture_index, const glm::vec2& texture_size,
    std::string atlas_file_path)
{
    using json = nlohmann::json;
    size_t dot_pos = atlas_file_path.rfind('.');
    if (dot_pos != std::string::npos) {
        atlas_file_path.replace(dot_pos, atlas_file_path.size() - dot_pos, ".json");
    }

    std::ifstream f(atlas_file_path);
    if (!f.is_open())
    {
        CHESS_LOG_ERROR("Failed to open the json data file.");
        return;
    }

    json data, slices;
    f >> data;
    f.close();

    slices = data["frames"];
    for (const auto& sliced_texture_data : slices)
    {
        json frame = sliced_texture_data.at("frame");
        glm::vec2 sprite_width = glm::vec2{ frame["w"], frame["h"] };
        Texture sliced_texture;

        sliced_texture.texture_region.start = { frame["x"], frame["y"]};
        sliced_texture.texture_region.end   = sliced_texture.texture_region.start + sprite_width;

        sliced_texture.texture_region.start /= texture_size;
        sliced_texture.texture_region.end /= texture_size;

        sliced_texture.texture_handle = texture_index;
        sliced_texture.is_texture_atlas = true;

        m_AssetTextureHash.emplace(sliced_texture_data["filename"], sliced_texture);
    }

}
