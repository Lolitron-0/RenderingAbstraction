#pragma once
#include "Framebuffer.hpp"

namespace Ra
{
    class OpenGLFramebuffer : public Framebuffer
    {
    public:
        OpenGLFramebuffer(const FramebufferProperties& props);
        ~OpenGLFramebuffer();

        void StartWriting() override;
        void StopWriting() override;

        void Resize(std::uint32_t width, std::uint32_t height) override;

        RendererId GetColorAttachmentHandle() const override;

        const FramebufferProperties& GetProperties() const override;

    private:
        void Invalidate_();
        void Cleanup_();

        RendererId m_Handle{ 0 };
        RendererId m_ColorAttachmentHandle{ 0 }, m_DepthAttachmentHandle{ 0 };

        FramebufferProperties m_Properties;
    };
}