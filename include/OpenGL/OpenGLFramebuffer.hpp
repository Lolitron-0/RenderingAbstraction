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

        RendererId GetColorAttachmentHandle(std::size_t index) const override;
        RendererId GetDrawTextureHandle(std::size_t index = 0) const override;

        const FramebufferProperties& GetProperties() const override;

    private:
        void Invalidate_();
        void Cleanup_();

        RendererId m_MainFramebufferHandle{ 0 };
        std::vector<RendererId> m_ColorAttachments;
        RendererId m_DepthAttachment{ 0 };

        std::vector<TextureSpecification> m_ColorAttachmentSpecifications;
        TextureSpecification m_DepthAttachmentSpecification;

        RendererId m_ResolveFramebufferHandle{ 0 };
        RendererId m_ResolvedAttachment{ 0 };

        FramebufferProperties m_Properties;
    };
}