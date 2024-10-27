#pragma once
#include "Core/Chess_pch.h"
#include "Core/ApplicationData.h"
#include "Core/EventListener.h"
namespace Chess_Game
{
    /**
     * @brief Class for creating and managing a integer framebuffer.
     *
     * The class implements the 'Listener' to handle window re-sizing.
     * The class provides function to sample a specific pixel from the buffer 'GetPixelData()'.
     * This class is used for mouse picking. 
     */
    class IntFramebuffer : public Listener
    {
    public:
        /**
         * @brief Constructor creates a valid framebuffer along side the needed attachments.
         * 
         * The needed attachments are:
         *  - Integer texture for mouse picking.
         *  - Depth and stencil using the render buffer object.
         * 
         * @param framebuffer_size Starting size of the framebuffer.
         */
        IntFramebuffer(Size2D framebuffer_size);
        /**
         * @brief Destroys the framebuffer,Integer texture and stencil and depth GPU resource.
         */
        ~IntFramebuffer();
        /**
         * @brief Retrieves the index value on the corresponding pixel. 
         * 
         * The function receives a 2D coordinate and retrieves whatever index value is in the index framebuffer texture.
         * The coordinate must be bottom left screen coordinate meaning the location (0,0) is located at the bottom left of the screen.
         * 
         * @param x The x coordinate of the pixel.
         * @param y The y coordinate of the pixel.
         * @returns Returns the index value corresponding to the pixel at the provided location.
         */
        size_t GetPixelData(GLint x, GLint y);
        /**
         * @brief Returns true if the created framebuffer is valid.
         * @returns Returns true if the created framebuffer is valid.
         */
        bool IsFramebufferValid()const { return m_IsFramebufferValid; }
        /**
         * @brief Binds the framebuffer to be the current one beeing used.
         */
        void BindFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferHandle); }
        /**
         * @brief Binds the default framebuffer, provided by OpenGL, to be the one used. 
         */
        static void BindDefaultFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
    private:
        /**
         * @brief Resizes the framebuffer texture and render buffer object.
         * @param new_size New framebuffer size.
         */
        void ResizeFramebuffer(Size2D new_size);
        /**
         * @brief Virtual function override from the 'Listener' class for handling events.
         */
        void OnEvent(const Event& e) override;
    private:
        GLuint m_FramebufferHandle{};               ///< Handle of the GPU framebuffer resource.
        GLuint m_TextureAttachmentHandle{};         ///< Handle of the GPU framebuffer intiger texture resource.
        GLuint m_RBODepthStencilAttachmentHandle{}; ///< Handle of the GPU render buffer object(RBO) resource for depth and stencil. 
        Size2D m_CurrentFramebufferSize{};          ///< Size of the framebuffer.
        bool m_IsFramebufferValid = true;           ///< True if the framebuffer is valid.
    };
}