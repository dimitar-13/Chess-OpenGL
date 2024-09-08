#pragma once
#include <glm/glm.hpp>
#include "TextureEnum.h"
#include "Logging/Logger.h"
namespace Chess_Game
{
    class Drawable
    {
    public:
        Drawable(const glm::vec3& start_position,const glm::vec3& draw_color, glm::vec2 scale = glm::vec2(10.0f),
            TextureName_ texture_name = TextureName_kBoard) :
            m_position(start_position), m_color(draw_color), m_scale(scale), m_textureName(texture_name)
        {}
        glm::vec3 GetPosition()const { return m_position; }
        const glm::vec3& GetColor()const { return m_color; }
        glm::vec2 GetScale()const { return m_scale; }
        void SetPosition(const glm::vec3& new_position) { m_position = new_position; }
        void SetScale(const glm::vec2& new_scale) { m_scale = new_scale; }
        void SetColor(const glm::vec3& color) { m_color = color; }
        TextureName_ GetDrawableTextureName()const { return m_textureName; }
    private:
        glm::vec3 m_position{};
        glm::vec2 m_scale;
        glm::vec3 m_color{};
        TextureName_ m_textureName{};
    };
}