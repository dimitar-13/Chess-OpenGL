#pragma once
#include "ApplicationData.h"
#include <glm/gtc/matrix_transform.hpp>
namespace Chess_Game
{
    struct OrthographicMatrixBorder
    {
        float top{};
        float bottom{};
        float right{};
        float left{};
    };
    struct Viewport
    {
        float x{};
        float y{};
        float width{};
        float height{};
    };

    class OrthoViewportHandler
    {
    public:
        OrthoViewportHandler(const Viewport viewport_dimensions);

        void UpdateViewport(const Viewport& viewport_dimensions);
        const glm::mat4& GetMatrix()const { return m_OrthographicMatrix; }
        glm::mat4 GetInverseProjection()const { return glm::inverse(m_OrthographicMatrix); }
        OrthographicMatrixBorder GetProjectionSize()const { return m_MatrixBorders; }
        glm::vec2 FromScreenToOrthographicCoordinates(glm::vec2 screen_position)const;
    private:
        void CalculateOrthographicMatrix(Size2D window_width);
        void CalculateViewportMatrix();
    private:
        glm::mat4 m_OrthographicMatrix{};
        glm::mat4 m_ViewportTransform{};
        Viewport m_ViewportDimension{};
        OrthographicMatrixBorder m_MatrixBorders{};
    };


}