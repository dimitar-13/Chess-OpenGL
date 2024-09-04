#pragma once
#include "ApplicationData.h"
#include <glm/gtc/matrix_transform.hpp>
namespace Chess_Game
{
    struct OrthographicMatrixBorder
    {
        float right{};
        float top{};
    };

    class ApplicationProjection
    {
    public:
        void UpdateMatrix(Size2D window_width) { CalculateOrthographicMatrix(window_width); }             
        const glm::mat4& GetMatrix()const { return m_OrthographicMatrix; }
        glm::mat4 GetInverseProjection()const { return glm::inverse(m_OrthographicMatrix); }
        OrthographicMatrixBorder GetProjectionSize()const { return m_MatrixBorders; }
    private:
        void CalculateOrthographicMatrix(Size2D window_width);
    private:
        glm::mat4 m_OrthographicMatrix{};
        OrthographicMatrixBorder m_MatrixBorders{};
    };


}