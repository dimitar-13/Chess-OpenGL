#pragma once
#include <glm/glm.hpp>
namespace Chess_Game
{
    class Drawable
    {
    public:
        Drawable(glm::vec2 start_position, glm::vec3 draw_color) :
            m_position(start_position), m_color(draw_color)
        {}

        glm::vec2 GetPosition()const { return m_position; }
        glm::vec2 SetPosition()const { return m_position; }
        const glm::vec3& GetColor()const { return m_color; }

    private:
        glm::vec2 m_position{};
        glm::vec3 m_color{};
    };
}