#pragma once
#include "Pch/Chess_pch.h"
#include "GPU-Side/TextureBatcher.h"
#include "GPU-Side/Framebuffer.h"
#include "Core/Drawable.h"
#include "Core/AssetLoader.h"
#include "Core/TextFont.h"
#include "BatchPipeline.h"
namespace Chess_Game
{
    /**
     * @brief Class for batch rendering.
     *
     * The class work with the 'BatchPipeline' to render different batches.
     * You can pick from the following batches to render:
     *  - Circle batch for circle rendering.
     *  - Textured quad batch for rendering textured quads.
     *  - Text batch for text rendering.
     * 
     * The class also supports mouse picking for the textured class by setting a flag to enable it in the following function
     * 'DrawTextureQuadBatch()'.
     * 
     */
    class BatchRenderer
    {
    public:
        /**
         * @brief Constructor creates all of the different batches and mouse picking shader along with the integer framebuffer.
         *
         * @param window_size The window size used for creating the integer framebuffer.
         * @param application_asset_loader Ref to the application asset loader for getting the texture indices.
         */
        BatchRenderer(Size2D window_size, std::shared_ptr<AssetLoader> application_asset_loader);
        /**
         * @brief Pushes circle to the batch. 
         * Constructs a circle data from the provided arguments and push it to the circle batch.
         * @param position Circle orthographic matrix position.
         * @param scale Size of the circle.
         * @param object_color Circle color.
         */
        void PushCircle(const glm::vec3& position, const glm::vec2& scale, const glm::vec3& object_color);
        /**
         * @brief Pushes textured quad to the quad batch data.
         * Constructs a textured quad data from the provided arguments and push it to the quad batch data.
         * @param object_id Drawable ID.
         * @param position Quad orthographic matrix position.
         * @param scale Size of the quad.
         * @param object_color Quad color.
         * @param texture_name Texture to use default is 'TextureName_kWhiteTexture'.
         */
        void PushTexturedQuad(size_t object_id, const glm::vec3& position, const glm::vec2& scale,
            const glm::vec3& object_color, const std::string& texture_name =AssetLoader::GetWhiteTextureAssetName());
        /**
         * @brief Pushes the string to the text batch data.
         *
         * From a provided string the function calculates all of the squares for each character. The way they are calculated 
         * is by creating a bounding box and finding the center this is how we get the position and then half of the size of the bounding
         * box is the scale of the square.
         * 
         * @note The function only recognize the '/n' and makes a new like but no other special symbols.
         * 
         * @param text_to_draw The text string to be drawn.
         * @param position The text start position.
         * @param scale The scale of the overall text.
         * @param text_color The text color.
         * @param font_to_use Font data to use.
         */
        void PushText(const std::string& text_to_draw,
            const glm::vec3& position, const glm::vec2& scale, const glm::vec3& text_color, std::shared_ptr<TextFont> font_to_use);
        /**
         * @brief Draws the circle batch.
         * The function sets the needed shader uniforms and then issues a draw call to the circle batch and then flushes the batch.
         * @param projection Orthographic projection needed by the batch shader.
         */
        void DrawCircleBatch(const glm::mat4& projection);
        /**
         * @brief Draws the text batch.
         * The function sets the needed shader uniforms and then issues a draw call to the text batch and then flushes the batch.
         * @param projection Orthographic projection needed by the batch shader.
         */
        void DrawTextBatch(const glm::mat4& projection);
        /**
         * @brief Draws the textured quad batch.
         * 
         * This batch is used both by the UI and the 2D scene drawables. The function expects a projection to that is needed
         * by the shader uniform and provide a flag for outputting the drawable ID using the Integer framebuffer.
         * 
         * @param projection Orthographic projection needed by the batch shader.
         * @param output_drawable_id If set to true it outputs the drawn drawables ID to the 'IntFramebuffer' that can be retrived 
         *                           using the 'GetIDFramebuffer()'.
         */
        void DrawTextureQuadBatch(const glm::mat4& projection,bool output_drawable_id = false);
        /**
         * @brief Gets the integer framebuffer that outputs the drawable ID.
         * @returns Returns the integer framebuffer that outputs the drawable ID.
         */
        std::shared_ptr<IntFramebuffer> GetIDFramebuffer() { return m_MousePickingFramebuffer; }
        /**
         * @brief Flushes the texture batch. 
         */
        void FlushTextureBatch() { m_TextureBatcher.Flush(); }
    private:
        std::shared_ptr<AssetLoader> m_ApplicationAssetLoader{};    ///< Ref to the asset loader.
        std::unique_ptr<BatchPipeline<200>> m_TexturedQuadBatch;                    ///< Texture quad batch.
        std::unique_ptr<BatchPipeline<100>> m_CircleQuadBatch;                      ///< Circle quad batch.
        std::unique_ptr<BatchPipeline<500>> m_TextQuadBatch;                        ///< Text quad batch.
        TextureBatcher m_TextureBatcher{};                          ///< Texture batcher class.
        std::unique_ptr<ShaderClass> m_MousePickingShader{};        ///< Mouse picking shader.
        std::shared_ptr<IntFramebuffer> m_MousePickingFramebuffer;  ///< Integer framebuffer for outputting drawable ID.
    };
}