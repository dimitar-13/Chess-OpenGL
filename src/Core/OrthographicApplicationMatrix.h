#pragma once
#include "ApplicationData.h"
#include "Core/Chess_pch.h"

namespace Chess_Game
{
    /**
     * @brief Defines the borders of an orthographic projection matrix.
     *
     * The `OrthographicMatrixBorder` struct represents the boundaries of an orthographic projection
     * matrix used in rendering. It defines the clipping planes for the top, bottom, right, and left
     * edges of the projection.
     */
    struct OrthographicMatrixBorder
    {
        float top{};      ///< Top boundary of the orthographic matrix.
        float bottom{};   ///< Bottom boundary of the orthographic matrix.
        float right{};    ///< Right boundary of the orthographic matrix.
        float left{};     ///< Left boundary of the orthographic matrix.
    };

    /**
     * @brief Represents a rectangular viewport in screen space.
     *
     * The `Viewport` struct defines the position and size of a rectangular region used for rendering
     * on the screen. It specifies where the rendering should occur in normalized screen coordinates.
     *
     * @note This is using OpenGL convention meaning the origin O{0,0} is located at the bottom 
     * left of the screen.
     */
    struct Viewport
    {
        float x{};      ///< x-coordinate of the viewport's origin.
        float y{};      ///< y-coordinate of the viewport's origin.
        float width{};  ///< Width of the viewport.
        float height{}; ///< Height of the viewport.
    };


    /**
     * @brief Helper class for handling projections.
     *
     * The class creates both orthographic and viewport transform matrices based on provided 
     * viewport dimensions.
     * 
     * The class also provides ways of extracting information about the matrices like the borders of the
     * orthographic matrix and the matrices themself both (orthographic and viewport transform).
     * 
     */
    class OrthoViewportHandler
    {
    public:
        /**
         * @brief Constructs an `OrthoViewportHandler` with the given viewport dimensions.
         *
         * This constructor initializes the `OrthoViewportHandler` with the provided `viewport_dimensions`
         * and immediately calls the `UpdateViewport()` function to set up the viewport and recalculate
         * the necessary matrices (both orthographic and viewport transform matrices).
         *
         * @param viewport_dimensions The initial dimensions of the viewport, specified as a `Viewport` struct.
         *
         * @note The constructor ensures that the viewport is fully initialized and that the associated OpenGL
         *       `glViewport` is configured with the provided dimensions.
         */
        OrthoViewportHandler(const Viewport& viewport_dimensions);
        /**
         * @brief Updates the viewport and recalculates the matrices.
         *
         * Updates the current viewport with the provided 'viewport_dimensions' and 
         * recalculates both the orthographic and viewport transform matrices.
         * 
         * @note The function also updates the glViewport.
         * 
         * @param viewport_dimensions Thew new viewport.
         */
        void UpdateViewport(const Viewport& viewport_dimensions);
        /**
         * @brief Gets the orthographic matrix.
         * 
         * @returns Returns the orthographic matrix.
         */
        const glm::mat4& GetOrthographicMatrix()const { return m_OrthographicMatrix; }
        /**
         * @brief Gets an inverse orthographic matrix projection.
         * @returns Returns an inverse orthographic matrix projection.
         */
        glm::mat4 GetInverseOrthographicProjection()const { return glm::inverse(m_OrthographicMatrix); }
        /**
         * @brief Gets the current orthographic matrix borders.
         * @returns Returns the current orthographic matrix borders left,right,bottom,top.
         */
        OrthographicMatrixBorder GetOrthographicProjectionSize()const { return m_MatrixBorders; }
        /**
         * @brief Converts screen position to orthographic position.
         *
         * The function retrieves a screen position and attemps to convert it to orthographic 
         * position.
         * 
         * @param screen_position A window position you want to convert.
         * @returns Returns the converted 'screen_position' to orthographic position.
         */
        glm::vec2 FromScreenToOrthographicCoordinates(glm::vec2 screen_position)const;
    private:
        /**
         * @brief Calculates orthographic matrix on provided window_size.
         * 
         * The calculation of the matrix is not your usual window size base one.
         * The application uses a square window with aspect 1x1.
         * To avoid stretching when calculating the new matrix if the new window have different aspect
         * then 1x1 it adjust the base matrix values to match the aspect ration.
         * At the end it gets half of the aspect adjusted width and height so that the world center is
         * at middle.
         * 
         * @param window_size The current window size.
         */
        void CalculateOrthographicMatrix(Size2D window_size);
        /**
         * @brief Calculates the viewport transform matrix based on the 'm_ViewportDimension'.
         * 
         * The functions calculates the viewport transform matrix based on the current said 
         * 'm_ViewportDimension'.
         */
        void CalculateViewportMatrix();
    private:
        glm::mat4 m_OrthographicMatrix{};           ///< The current orthographic matrix.
        glm::mat4 m_ViewportTransform{};            ///< The current viewport transform matrix.
        Viewport m_ViewportDimension{};             ///< The current viewport borders x,y,width,height.
        OrthographicMatrixBorder m_MatrixBorders{}; ///< The orthographic matrix borders left,right,bottom,top.
    };


}