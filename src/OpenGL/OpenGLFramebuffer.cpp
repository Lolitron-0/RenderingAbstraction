#include "rapch.h"
#include "OpenGL/OpenGLFramebuffer.hpp"
#include <glad/glad.h>

namespace Ra
{
    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferProperties& props)
        :m_Properties(props)
    {
        Invalidate_();
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        Cleanup_();
    }

    void OpenGLFramebuffer::StartWriting()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);
        glViewport(0, 0, m_Properties.Width, m_Properties.Height);
    }

    void OpenGLFramebuffer::StopWriting()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Resize(std::uint32_t width, std::uint32_t height)
    {
        m_Properties.Width = width;
        m_Properties.Height = height;
        Invalidate_();
    }

    Ra::RendererId OpenGLFramebuffer::GetColorAttachmentHandle() const
    {
        return m_ColorAttachmentHandle;
    }

    const Ra::FramebufferProperties& OpenGLFramebuffer::GetProperties() const
    {
        return m_Properties;
    }

    void OpenGLFramebuffer::Invalidate_()
    {
        if (m_Handle)
            Cleanup_();

        glGenFramebuffers(1, &m_Handle);
        glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachmentHandle);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachmentHandle);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Properties.Width, m_Properties.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachmentHandle, 0);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachmentHandle);
        glBindTexture(GL_TEXTURE_2D, m_DepthAttachmentHandle);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Properties.Width, m_Properties.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Properties.Width, m_Properties.Height);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachmentHandle, 0);

        RA_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Cleanup_()
    {
        glDeleteFramebuffers(1, &m_Handle);
        glDeleteTextures(1, &m_ColorAttachmentHandle);
        glDeleteTextures(1, &m_DepthAttachmentHandle);
    }

}