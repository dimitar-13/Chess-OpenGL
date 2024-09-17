#pragma once
#include "Core/Chess_pch.h"
#include "Core/ApplicationData.h"
#include "Core/EventListener.h"
namespace Chess_Game
{
    class IntFramebuffer : public Listener
    {
    public:
        IntFramebuffer(Size2D framebuffer_size);
        ~IntFramebuffer();
        size_t GetPixelData(GLint x, GLint y);
        bool IsFramebufferValid()const { return m_IsFramebufferValid; }
        void BindFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferHandle); }
        static void BindDefaultFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
    private:
        void ResizeFramebuffer(Size2D new_size);
        void OnEvent(const Event& e) override;
    private:
        GLuint m_FramebufferHandle{};
        GLuint m_TextureAttachmentHandle{};
        GLuint m_RBODepthStencilAttachmentHandle{};
        Size2D m_CurrentFramebufferSize{};
        bool m_IsFramebufferValid = true;
    };
}