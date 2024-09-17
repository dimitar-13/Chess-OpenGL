#include "D:/c++/OpenGl/Chess-OpenGL/build/CMakeFiles/Chess.dir/Debug/cmake_pch.hxx"
#include "Framebuffer.h"

Chess_Game::IntFramebuffer::IntFramebuffer(Size2D framebuffer_size)
{
  
    glGenTextures(1, &m_TextureAttachmentHandle);

    glBindTexture(GL_TEXTURE_2D, m_TextureAttachmentHandle);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI,
        framebuffer_size.width, framebuffer_size.height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenRenderbuffers(1, &m_RBODepthStencilAttachmentHandle);

    glBindRenderbuffer(GL_RENDERBUFFER, m_RBODepthStencilAttachmentHandle);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framebuffer_size.width, framebuffer_size.height);

    glGenFramebuffers(1, &m_FramebufferHandle);

    glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferHandle);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureAttachmentHandle, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBODepthStencilAttachmentHandle);

    m_IsFramebufferValid = glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Chess_Game::IntFramebuffer::~IntFramebuffer()
{
    glDeleteFramebuffers(1, &m_FramebufferHandle);
    glDeleteTextures(1, &m_TextureAttachmentHandle);
    glDeleteRenderbuffers(1, &m_RBODepthStencilAttachmentHandle);
}

size_t Chess_Game::IntFramebuffer::GetPixelData(GLint x, GLint y)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferHandle);
    size_t pixel_object_id{};
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &pixel_object_id);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return pixel_object_id;
}

void Chess_Game::IntFramebuffer::ResizeFramebuffer(Size2D new_size)
{
    m_CurrentFramebufferSize = new_size;

    glBindTexture(GL_TEXTURE_2D,m_TextureAttachmentHandle);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI,
        new_size.width, new_size.height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, m_RBODepthStencilAttachmentHandle);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, new_size.width, new_size.height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

}

void Chess_Game::IntFramebuffer::OnEvent(const Event& e)
{
    if (e.GetEventType() == EventType_kWindowResize)
    {
        Size2D new_size = dynamic_cast<const WindowResizeEvent&>(e).GetWindowSize();
        this->ResizeFramebuffer(new_size);
    }
}
