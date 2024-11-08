#pragma once
#include "Pch/Chess_pch.h"
#include "ApplicationData.h"
#include "TextureEnum.h"
namespace Chess_Game
{
    /**
     * @brief Plain old data(Pod) struct representing texture read information.
     *
     * Struct representing texture read information. The struct holds:
     * - Raw pixel data.
     * - Flag to indicate if the data is valid.
     * - The size of the texture
     * - The pixel data format RGB,RGBA etc.
     *
     */
    struct TextureReadData
    {
        std::vector<unsigned char> texture_source{};///< The texture pixel data.
        bool is_texture_valid{};                    ///< True if the textrue data is valid.
        Size2D texture_size{};                      ///< The size of the texture.
        GLenum texture_format{};                    ///< The pixel data format.
    };
    struct TextureResourceData
    {
        std::string texture_path{};     ///< Path to the texture resource starting from the resource folder.
        const char* texture_file_name;  ///< File name or name of the resource.
        bool is_texture_atlas = false;  ///< Is the resource to be understood as texture atlas. 
    };


    /**
     * @brief Handles the loading and management of textures for the project.
     *
     * The 'AssetLoader' class is responsible for loading textures from predefined asset paths, creating
     * corresponding GPU texture resources, and providing access to these resources during the application's
     * runtime. It provides a way to retrieve each GPU texture resource via the `TextureName_` enum
     * and ensures that all textures are properly cleaned up and destroyed when they are no longer needed.
     */
    class AssetLoader
    {
    public:
        /**
         * @brief Constructor loads all of the project assets(textures).
         * 
         * The constructor attemps to load all of the project assets.
         * It extract the information using the 'GetTextureAsset()' and if the result is valid
         * it allocates a GPU texture resource and sets it using the 'GetTextureAsset()' result.
         * 
         */
        AssetLoader();
        /**
         * @brief Destoies all of the allocated GPU resouces.
         */
        ~AssetLoader();
        /**
         * @brief Returns the texture handle corresponding to the provided 'TextureName_' enum.
         * 
         * The function returns the texture index from the 'm_AssetTextureHash' 
         * corresponding to the provided 'TextureName_' enum.
         * 
         * @note If texture not presented or enum is invalid then the function returns 0.

         * @param name_of_texture Enum of the texture get search.
         * 
         * @returns Returns non zero(valid) texture handle based on the provided 'TextureName_' enum.
         */
        const Texture& GetTextureAsset(const std::string& name_of_texture);
        /**
         * @brief Retrieves the name of the white texture asset.
         *
         * @returns Returns the name of the white texture asset.
         */
        static const std::string& GetWhiteTextureAssetName() { return s_WhiteTextureName; }
    private:
        /**
         * @brief Reads texture from path and extract the texture information.
         *
         * The function uses the 'stbi' library to read a texture from provided path and retrieve
         * a 'TextureReadData' struct containing all of the required information.
         * If read fails the functions logs an error.
         * 
         * @param texture_path Texture to read.
         * 
         * @returns Returns 'TextureReadData' info struct of the texture from the specified path.
         */
        TextureReadData ReadTexture(const std::string& texture_path);
        void UnpackTextureAtlas(GLuint texture_index, const glm::vec2& texture_size,
            std::string atlas_file_path);
    private:
        std::unordered_map<std::string, Texture> m_AssetTextureHash{}; ///< Map of texture enum and GPU texture handle pairs.
        std::vector<GLuint> m_AssetTextureArray{};                     ///< GPU texture handles array.
        static std::string s_WhiteTextureName;                         ///< White texture asset name.
    };
}