#pragma once
#include "Core/Chess_pch.h"
#include "Core/ApplicationData.h"
#include "Core/TextureEnum.h"
#include "gpu-side/ShaderClass.h"

namespace Chess_Game
{
    /**
     * @brief Class for batching textures in batch rendering setting.
     *
     * The class is responsible for keeping track of the currently bound textures and provides way of flushing the data.
     * Using 'PushTextureForRendering(index_of_the_texture)' you submit the texture to be bound to the batch array.
     * After all of the necessary textures have been pushed you can call 'BindTextures()' to bind them to their corresponding 
     * binding slots.
     * The class provides ways to 'Flush()' the batch and retrieve the bound texture index array 'GetBoundTexturesSlots()'.
     * 
     * 
     */
    class TextureBatcher
    {
    private:
        constexpr static size_t kShaderSamplerArrayBaseSize = 10;
    public:
        /**
         * @brief Constructor initializes the 'm_BindingPointsArray' with the corresponding binding points.
         * 
         * By corresponding binding points it means every value is equal to its index. This is only done so that we don't
         * construct an array every time.
         */
        TextureBatcher();
        /**
         * @brief Binds all of the textures pushed to the batcher to individual texture binding points.
         */
        void BindTextures()const;
        /**
         * @brief Gets the array of currently active binding points.
         * @returns Returns the array of currently active binding points.
         */
        const std::array<int,kShaderSamplerArrayBaseSize>& GetBoundTexturesSlots()const { return m_BindingPointsArray; }
        /**
         * @brief Gets the bound textures count.
         * @returns Returns the count of the bound textures.
         */
        size_t GetBoundTexturesCount() { return m_TextureToBindingArrayIndexHash.size();}
        /**
         * @brief Flushes the bound textures array. 
         * Resets the texture batch array and sets the 'm_CurrentFreeTextureSlotIndex' to 0.
         */
        void Flush();
        /**
         * @brief Pushes a valid 2D texture to be bound and returns the binding point. 
         * 
         * The function checks if the specified texture index is already pushed in the batch. If it is it returns the 
         * binding point. If its not then it appends it to the 'm_BindingPointsArray' and returns the available binding point after incrementing it.
         * 
         * If no more space available the function returns 0 and asserts.
         * 
         * @param texture_to_push Index of the GPU texture resource see 'AssetLoader' on how to acquire that index.
         * @returns Returns the binding point of the pushed texture.
         */
        size_t PushTextureForRendering(GLuint texture_to_push);
    private:
        std::unordered_map<GLuint, size_t> m_TextureToBindingArrayIndexHash{}; ///< Texture resource index and binding point hash.
        std::array<int, kShaderSamplerArrayBaseSize> m_BindingPointsArray{};  ///< Array of currently active binding points.
        size_t m_CurrentFreeTextureSlotIndex = 0; ///< Currently available binding point.
    };
}