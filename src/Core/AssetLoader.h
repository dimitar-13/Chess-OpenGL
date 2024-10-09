#pragma once
#include "ApplicationData.h"
#include "Chess_pch.h"
#include "TextureEnum.h"
namespace Chess_Game
{
    struct TextureReadData
    {
        std::vector<unsigned char> texture_source{};
        bool is_texture_valid{};
        Size2D texture_size{};
        GLenum texture_format{};
    };
    struct TexturePathNamePair
    {
        const char* texture_path{};
        TextureName_ texture_name{};
    };


    class AssetLoader
    {
    public:
        AssetLoader();
        ~AssetLoader();
        GLuint GetTextureAsset(TextureName_ name_of_texture);
    private:
        TextureReadData ReadTexture(const char* texture_path);
    private:
        std::unordered_map<TextureName_, GLuint> m_AssetTextureHash{};
        std::vector<GLuint> m_AssetTextureArray{};
    };
}