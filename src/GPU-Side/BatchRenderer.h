#pragma once
#include "Core/Chess_pch.h"
#include "GPU-Side/TextureBatcher.h"
#include "GPU-Side/Framebuffer.h"
#include "Core/Drawable.h"
#include "Core/AssetLoader.h"
#include "Core/TextFont.h"
#include "BatchPipeline.h"
namespace Chess_Game
{
    class Application;
    class BatchRenderer
    {
    public:
        BatchRenderer(Size2D window_size, std::shared_ptr<AssetLoader> aplication_asset_loader);
        void PushCircle(const glm::vec3& position, const glm::vec2& scale, const glm::vec3& object_color);
        void PushTexturedQuad(size_t object_id, const glm::vec3& position, const glm::vec2& scale,
            const glm::vec3& object_color, TextureName_ texture_name = TextureName_kWhiteTexture);
        void PushText(const std::string& text_to_draw,
            const glm::vec3& position, const glm::vec2& scale, const glm::vec3& text_color, std::shared_ptr<TextFont> font_to_use);
        void DrawCircleBatch(const glm::mat4& projection);
        void DrawTextBatch(const glm::mat4& projection);
        void DrawTextureQuadBatch(const glm::mat4& projection,bool output_drawable_id = false);
        std::shared_ptr<IntFramebuffer> GetIDFramebuffer() { return m_MousePickingFramebuffer; }
        void FlushTextureBatch() { m_TextureBatcher.Flush(); }
    private:
        std::shared_ptr<AssetLoader> m_ApplicationAssetLoader{};
        BatchPipeline<1000> m_TexturedQuadBatch;
        BatchPipeline<1000> m_CircleQuadBatch;
        BatchPipeline<1000> m_TextQuadBatch;
        TextureBatcher m_TextureBatcher{};
        std::unique_ptr<ShaderClass> m_MousePickingShader{};
        std::shared_ptr<IntFramebuffer> m_MousePickingFramebuffer;
    };
}