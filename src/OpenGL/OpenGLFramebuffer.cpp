#include "rapch.h"
#include "OpenGL/OpenGLFramebuffer.hpp"
#include <glad/glad.h>
#include "../../include/Renderer.hpp"

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
            case Ra::TextureFormat::DepthComponent:
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
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
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
                if (format == GL_DEPTH_COMPONENT)
                    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_FLOAT, nullptr);
                else
                    glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
                float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
                glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
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
                m_ColorAttachmentSpecifications.push_back(spec);
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
        glClearColor(1, 0, 1, 1);
        glClear((m_ColorAttachments.size() ? GL_COLOR_BUFFER_BIT : 0) | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLFramebuffer::StopWriting()
    {
        if (m_Properties.Samples > 1) Resolve_();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Resize(std::uint32_t width, std::uint32_t height)
    {
        m_Properties.Width = width;
        m_Properties.Height = height;
        Invalidate_();
    }

    RendererId OpenGLFramebuffer::GetColorAttachmentHandle(std::size_t index) const
    {
        return m_ColorAttachments[index];
    }

    RendererId OpenGLFramebuffer::GetDrawTextureHandle(std::size_t index /*= 0*/) const
    {
        return m_Properties.Samples > 1 ? m_ResolvedAttachments[index] : m_ColorAttachments[index];
    }

    const Ra::FramebufferProperties& OpenGLFramebuffer::GetProperties() const
    {
        return m_Properties;
    }

    int OpenGLFramebuffer::ReadPixel(std::uint32_t attachmentIndex, int x, int y, bool leaveBound)
    {
        int pixelData;
        RA_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Attachment index out of bounds!");
        if (m_Properties.Samples > 1)
        {
            Resolve_();
            glBindFramebuffer(GL_FRAMEBUFFER, m_ResolveFramebufferHandle);
        }
        else
            glBindFramebuffer(GL_FRAMEBUFFER, m_MainFramebufferHandle);
        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
        return pixelData;
    }

    RendererId OpenGLFramebuffer::GetDepthTextureHandle()
    {
        return m_DepthAttachment;
    }

    void OpenGLFramebuffer::BindDepthTexture()
    {
        RA_ASSERT(m_Properties.Samples == 1, "Depth texture binding for multisampled buffers not supported!");
        glActiveTexture(GL_TEXTURE0 + Renderer::GetLastTextureUnit() + 1);
        glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
        Renderer::SetLastTextureUnit(Renderer::GetLastTextureUnit() + 1);
    }

    void OpenGLFramebuffer::BindColorAttachment(std::size_t index /*= 0*/)
    {
        glActiveTexture(GL_TEXTURE0 + Renderer::GetLastTextureUnit() + 1);
        Utils::BindTexture(GetColorAttachmentHandle(index), m_Properties.Samples > 1);
        Renderer::SetLastTextureUnit(Renderer::GetLastTextureUnit() + 1);
    }

    void OpenGLFramebuffer::BindDrawTexture(std::size_t index /*= 0*/)
    {
        glActiveTexture(GL_TEXTURE0 + Renderer::GetLastTextureUnit() + 1);
        Utils::BindTexture(GetDrawTextureHandle(index), m_Properties.Samples > 1);
        Renderer::SetLastTextureUnit(Renderer::GetLastTextureUnit() + 1);
    }

    void OpenGLFramebuffer::Invalidate_()
    {
        if (m_MainFramebufferHandle)
            Cleanup_();

        glGenFramebuffers(1, &m_MainFramebufferHandle);
        glBindFramebuffer(GL_FRAMEBUFFER, m_MainFramebufferHandle);
        m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());

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
            m_ColorAttachments[i] = textureId;
        }

        Utils::CreateTexture(&m_DepthAttachment, m_Properties.Samples > 1);
        switch (m_DepthAttachmentSpecification.Format)
        {
        case TextureFormat::Depth24Stencil8:
            Utils::AttachDepthTexture(m_DepthAttachment, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Properties.Width, m_Properties.Height, m_Properties.Samples);
            break;
        case TextureFormat::DepthComponent:
            Utils::AttachDepthTexture(m_DepthAttachment, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT, m_Properties.Width, m_Properties.Height, m_Properties.Samples);
            break;
        }

        auto a = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        RA_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

        // Resolve framebuffer
        if (m_Properties.Samples > 1)
        {
            glGenFramebuffers(1, &m_ResolveFramebufferHandle);
            glBindFramebuffer(GL_FRAMEBUFFER, m_ResolveFramebufferHandle);
            m_ResolvedAttachments.resize(m_ColorAttachmentSpecifications.size());

            for (std::size_t i = 0; i < m_ColorAttachmentSpecifications.size(); i++)
            {
                RendererId textureId;
                Utils::CreateTexture(&textureId, false);
                switch (m_ColorAttachmentSpecifications[i].Format)
                {
                case TextureFormat::RGBA8:
                    Utils::AttachColorTexture(textureId, GL_RGBA8, GL_RGBA, m_Properties.Width, m_Properties.Height, 1, i);
                    break;
                case TextureFormat::R32:
                    Utils::AttachColorTexture(textureId, GL_R32I, GL_RED_INTEGER, m_Properties.Width, m_Properties.Height, 1, i);
                    break;
                default:
                    RA_ASSERT(false, "Wrong TextureFormat");
                    break;
                }
                m_ResolvedAttachments[i] = textureId;
            }
            RA_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Resolve framebuffer is incomplete!");
        }

        // Specify draw buffers
        glBindFramebuffer(GL_FRAMEBUFFER, m_MainFramebufferHandle);
        if (m_ColorAttachments.size() > 1)
        {
            RA_ASSERT(m_ColorAttachments.size() <= 4, "Framebuffers support up to 4 color attachments!");
            GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 , GL_COLOR_ATTACHMENT2 ,GL_COLOR_ATTACHMENT3 };
            glDrawBuffers((GLsizei)m_ColorAttachments.size(), buffers);
        }
        else if (m_ColorAttachments.size() == 0)
            glDrawBuffer(GL_NONE);

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
            glDeleteTextures((GLsizei)m_ResolvedAttachments.size(), m_ResolvedAttachments.data());
        }
    }

    void OpenGLFramebuffer::Resolve_()
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MainFramebufferHandle);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_ResolveFramebufferHandle);
        glBlitFramebuffer(
            0, 0, m_Properties.Width, m_Properties.Height,
            0, 0, m_Properties.Width, m_Properties.Height,
            GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, m_MainFramebufferHandle);
    }

}