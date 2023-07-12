#include "rapch.h"
#include "OpenGL/OpenGLFramebuffer.hpp"
#include <glad/glad.h>

namespace Ra
{

    namespace Utils
    {
        GLenum TextureTarget(bool multisampled) { return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D; }

        void BindTexture(RendererId texture, bool multisampled)
        {
            glBindTexture(TextureTarget(multisampled), texture);
        }

        bool IsDepthFormat(TextureFormat format)
        {
            switch (format)
            {
            case Ra::TextureFormat::Depth24Stencil8:
                return true;
            }
            return false;
        }

        void CreateTexture(RendererId* outId, bool multisampled)
        {
            glCreateTextures(TextureTarget(multisampled), 1, outId);
        }

        void AttachColorTexture(RendererId texture, GLenum internalFormat, GLenum format, std::uint32_t width, std::uint32_t height, int samples, std::size_t index)
        {
            bool multisampled = samples > 1;
            BindTexture(texture, multisampled);

            if (multisampled)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, (GLint)index, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (GLint)index, TextureTarget(multisampled), texture, 0);
        }

        void AttachDepthTexture(RendererId texture, GLenum format, GLenum attachmentType, std::uint32_t width, std::uint32_t height, int samples)
        {
            bool multisampled = samples > 1;
            BindTexture(texture, multisampled);

            if (multisampled)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
            }
            else
            {
                glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), texture, 0);
        }
    }

    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferProperties& props)
        :m_Properties(props)
    {
        for (const auto& spec : props.Attachments)
        {
            if (Utils::IsDepthFormat(spec.Format))
                m_DepthAttachmentSpecification = spec;
            else
                m_ColorAttachmentSpecifications.emplace_back(spec);
        }
        Invalidate_();
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        Cleanup_();
    }

    void OpenGLFramebuffer::StartWriting()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_MainFramebufferHandle);
        glViewport(0, 0, m_Properties.Width, m_Properties.Height);
    }

    void OpenGLFramebuffer::StopWriting()
    {
        if (m_Properties.Samples > 1)
        {
            glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MainFramebufferHandle);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_ResolveFramebufferHandle);
            glBlitFramebuffer(
                0, 0, m_Properties.Width, m_Properties.Height,
                0, 0, m_Properties.Width, m_Properties.Height,
                GL_COLOR_BUFFER_BIT, GL_NEAREST);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Resize(std::uint32_t width, std::uint32_t height)
    {
        m_Properties.Width = width;
        m_Properties.Height = height;
        Invalidate_();
    }

    Ra::RendererId OpenGLFramebuffer::GetColorAttachmentHandle(std::size_t index) const
    {
        return m_ColorAttachments[index];
    }

    Ra::RendererId OpenGLFramebuffer::GetDrawTextureHandle(std::size_t index /*= 0*/) const
    {
        return m_Properties.Samples > 1 ? m_ResolvedAttachment : m_ColorAttachments[index];
    }

    const Ra::FramebufferProperties& OpenGLFramebuffer::GetProperties() const
    {
        return m_Properties;
    }

    void OpenGLFramebuffer::Invalidate_()
    {
        if (m_MainFramebufferHandle)
            Cleanup_();

        glGenFramebuffers(1, &m_MainFramebufferHandle);
        glBindFramebuffer(GL_FRAMEBUFFER, m_MainFramebufferHandle);

        bool multisampled = m_Properties.Samples > 1;
        for (std::size_t i = 0; i < m_ColorAttachmentSpecifications.size(); i++)
        {
            RendererId textureId;
            Utils::CreateTexture(&textureId, multisampled);
            switch (m_ColorAttachmentSpecifications[i].Format)
            {
            case TextureFormat::RGBA8:
                Utils::AttachColorTexture(textureId, GL_RGBA8, GL_RGBA, m_Properties.Width, m_Properties.Height, m_Properties.Samples, i);
                break;
            case TextureFormat::R32:
                Utils::AttachColorTexture(textureId, GL_R32I, GL_RED_INTEGER, m_Properties.Width, m_Properties.Height, m_Properties.Samples, i);
                break;
            default:
                RA_ASSERT(false, "Wrong TextureFormat");
                break;
            }
            m_ColorAttachments.emplace_back(textureId);
        }

        Utils::CreateTexture(&m_DepthAttachment, m_Properties.Samples > 1);
        switch (m_DepthAttachmentSpecification.Format)
        {
        case TextureFormat::Depth24Stencil8:
            Utils::AttachDepthTexture(m_DepthAttachment, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Properties.Width, m_Properties.Height, m_Properties.Samples);
            break;
        }

        auto a = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        RA_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

        // Resolve framebuffer
        if (m_Properties.Samples > 1)
        {
            glGenFramebuffers(1, &m_ResolveFramebufferHandle);
            glBindFramebuffer(GL_FRAMEBUFFER, m_ResolveFramebufferHandle);

            Utils::CreateTexture(&m_ResolvedAttachment, false);
            Utils::AttachColorTexture(m_ResolvedAttachment, GL_RGBA8, GL_RGBA, m_Properties.Width, m_Properties.Height, 1, 0);

            RA_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Resolve framebuffer is incomplete!");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Cleanup_()
    {
        glDeleteFramebuffers(1, &m_MainFramebufferHandle);
        glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
        glDeleteTextures(1, &m_DepthAttachment);

        if (m_ResolveFramebufferHandle)
        {
            glDeleteFramebuffers(1, &m_ResolveFramebufferHandle);
            glDeleteTextures(1, &m_ResolvedAttachment);
        }
    }

}