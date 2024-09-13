#pragma once
#include "Core/Chess_pch.h"
#include "TextureEnum.h"
#include "Logging/Logger.h"
namespace Chess_Game
{
    struct DrawableData
    {
        glm::vec3 position{};
        glm::vec3 color{};
        glm::vec2 scale{};
        TextureName_ texture_name = TextureName_kBoard;
        DrawableData() = default;
        DrawableData(TextureName_ draw_tex_name,
            glm::vec3 draw_pos = glm::vec3(0), glm::vec3 draw_color = glm::vec3(1),
            glm::vec2 draw_scale = glm::vec2(30.0f)) :
            position(draw_pos), color(draw_color), scale(draw_scale), texture_name(draw_tex_name)
        {}
    };

    class Drawable
    {
    public:
        Drawable(const DrawableData& drawable_data) :m_DrawableData(drawable_data)
        {}
        glm::vec3 GetPosition()const { return m_DrawableData.position; }
        const glm::vec3& GetColor()const { return m_DrawableData.color; }
        glm::vec2 GetScale()const { return m_DrawableData.scale; }
        void SetPosition(const glm::vec3& new_position) { m_DrawableData.position = new_position; }
        void SetScale(const glm::vec2& new_scale) { m_DrawableData.scale = new_scale; }
        void SetColor(const glm::vec3& color) { m_DrawableData.color = color; }
        TextureName_ GetDrawableTextureName()const { return m_DrawableData.texture_name; }
        void SetDrawableTextureName(TextureName_ new_texture_name) { m_DrawableData.texture_name = new_texture_name; }
    private:
        DrawableData m_DrawableData;
    };
}