#include "Chess_pch.h"
#include "OrthographicApplicationMatrix.h"

Chess_Game::OrthoViewportHandler::OrthoViewportHandler(const Viewport& viewport_dimensions):
    m_ViewportDimension(viewport_dimensions)
{
    UpdateViewport(viewport_dimensions);
}
void Chess_Game::OrthoViewportHandler::UpdateViewport(const Viewport& viewport_dimensions)
{
    m_ViewportDimension = viewport_dimensions;
    size_t width  = viewport_dimensions.width;
    size_t height = viewport_dimensions.height;

    glViewport(m_ViewportDimension.x, m_ViewportDimension.y, m_ViewportDimension.width, m_ViewportDimension.height);\

    CalculateViewportMatrix();

    CalculateOrthographicMatrix(Size2D{ width, height });
}

glm::vec2 Chess_Game::OrthoViewportHandler::FromScreenToOrthographicCoordinates(glm::vec2 screen_position)const
{
    glm::vec4 to_covert = glm::vec4(screen_position.x, screen_position.y,1.0f,1.0f);
    screen_position = glm::inverse(m_OrthographicMatrix) * glm::inverse(m_ViewportTransform) * to_covert;
    return screen_position;
}

void Chess_Game::OrthoViewportHandler::CalculateOrthographicMatrix(Size2D window_width)
{
    /**
     * Dynamic orthographic aspect ratio adjustment.
     *
     * Here I want to explain how this works and give some ideas on how to think about all of this.
     *
     * In general, when you have projection -> NDC -> Viewport, you are just transforming values from one box
     * to another (at least in 2D that is). What I mean is that orthographic projection is a box with
     * borders Ortho(x, y), NDC is a box with borders NDC(-1, 1), and Viewport is a box with borders
     * Viewport(width, height).
     *
     * Now, if you try to use a size-independent projection, meaning your orthographic projection doesn't depend
     * on your window size, eventually, you will encounter a problem where things tend to stretch.
     *
     * The reason why things stretch is really simple. If you have a rectangular box and you try to fit it into
     * a square-like box, things will warp/stretch. The same can be thought of for the projection -> NDC -> Viewport.
     * In this project, the orthographic box has an aspect of 1x1. When you do a viewport transform to a non-square
     * window with an aspect different from 1x1, stretching will occur.
     *
     * To combat this, it would make sense to change the orthographic matrix (or to be precise, scale it) to
     * match the aspect of the window.
     *
     * Now whenever people mention aspect ratio, it's sometimes vague, at least for me. I want to give you an intuitive idea
     * on how to think about the aspect ratio in a more understandable way. A ratio or division in math is a
     * relationship between the denominator and the numerator. It is a question, if you will, that asks:
     * x/y, how many 'y' can we fit into 'x'?
     * But there is another way to think about it. Let's say we have a screen with width: 1600 and height: 800.
     * The aspect ratio here will be 2/1 or 2:1. What this really says is that the width is 2 times the height,
     * or we can fit the height 2 times into the width. An aspect ratio is really just screen proportions.
     *
     * That being said, how would the below calculations work? Well, what we want to avoid with this stretching is really
     * to adjust the projection to match the aspect ratio of the screen. We don't care about screen sizes, but the
     * overall shape. On a mathematical level, the aspect ratio gives us another piece of information, which is:
     * What is bigger, the width or the height (aspect > 1.0 and aspect < 1.0)? This is important because it reveals the rectangular
     * shape. All that's left is to change our orthographic projection to have the same ratio.
     * But how?
     * If we go back to our 2:1 example with width: 1600 and height: 800 and say we have an orthographic matrix
     * that is 0width: 800, 0height: 800. In this case, we would want to change the width to be twice the height.
     * Because this is a box, it's really easy to do this by just multiplying the width by the screen aspect.
     * For the height, it will be the inverse or dividing it by the aspect.
     */

    float screen_aspect_ratio = static_cast<float>(window_width.width) / static_cast<float>(window_width.height);
    constexpr float kBaseMatrixBorder = 800.f;
    float vertical_border = kBaseMatrixBorder;
    float horizontal_border = kBaseMatrixBorder;

    if (screen_aspect_ratio > 1.0f)
    {
        //Width is more   
        horizontal_border = kBaseMatrixBorder * screen_aspect_ratio;
    }
    else if (screen_aspect_ratio < 1.0f)
    {
        //Height is more
        vertical_border = kBaseMatrixBorder / screen_aspect_ratio;
    }

    m_OrthographicMatrix = glm::ortho(
        -horizontal_border/2.0f,
        horizontal_border / 2.0f,
        -vertical_border / 2.0f,
        vertical_border/2.0f,
        -1.0f,
        1.0f
    );

    m_MatrixBorders.right = horizontal_border / 2.0f;
    m_MatrixBorders.left = -m_MatrixBorders.right;
    m_MatrixBorders.top = vertical_border / 2.0f;
    m_MatrixBorders.bottom = -m_MatrixBorders.top;
}

void Chess_Game::OrthoViewportHandler::CalculateViewportMatrix()
{
    constexpr float maxZ = 1.0f;
    constexpr float minZ = 0.0f;
    Size2D window_size = 
        Size2D{static_cast<size_t>(m_ViewportDimension.width), static_cast<size_t>(m_ViewportDimension.height)};

    m_ViewportTransform = glm::mat4(1);

    m_ViewportTransform[0][0] = static_cast<float>(window_size.width) / 2.0f;
    m_ViewportTransform[1][1] = static_cast<float>(window_size.height) / 2.0f;

    m_ViewportTransform[3][0] = m_ViewportTransform[0][0];
    m_ViewportTransform[3][1] = m_ViewportTransform[1][1];

    m_ViewportTransform[2][2] = (maxZ - minZ) / 2.0f;
    m_ViewportTransform[3][2] = (maxZ + minZ) / 2.0f;

}

