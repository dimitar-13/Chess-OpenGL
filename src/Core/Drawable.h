#pragma once
#include <glm/glm.hpp>
namespace Chess_Game
{
    class Drawable
    {
    public:
        Drawable(const glm::vec3& start_position,const glm::vec3& draw_color, glm::vec2 scale = glm::vec2(10.0f)) :
            m_position(start_position), m_color(draw_color), m_scale(scale)
        {}

        glm::vec3 GetPosition()const { return m_position; }
        const glm::vec3& GetColor()const { return m_color; }
        glm::vec2 GetScale()const { return m_scale; }
        void SetPosition(const glm::vec3& new_position) { m_position = new_position; }
        void SetScale(const glm::vec2& new_scale) { m_scale = new_scale; }
        void SetColor(const glm::vec3& color) { m_color = color; }
    private:
        glm::vec3 m_position{};
        glm::vec2 m_scale;
        glm::vec3 m_color{};
    };
}