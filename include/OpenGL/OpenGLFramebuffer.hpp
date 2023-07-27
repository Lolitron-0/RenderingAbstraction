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
        void BindColorAttachment(std::size_t index = 0) override;

        RendererId GetDrawTextureHandle(std::size_t index = 0) const override;
        void BindDrawTexture(std::size_t index = 0) override;

        RendererId GetDepthTextureHandle() override;
        void BindDepthTexture() override;

        const FramebufferProperties& GetProperties() const override;

        int ReadPixel(std::uint32_t attachmentIndex, int x, int y, bool leaveBound = true) override;


    private:
        void Invalidate_();
        void Cleanup_();
        void Resolve_();

        RendererId m_MainFramebufferHandle{ 0 };
        std::vector<RendererId> m_ColorAttachments;
        RendererId m_DepthAttachment{ 0 };

        std::vector<AttachmentSpecification> m_ColorAttachmentSpecifications;
        AttachmentSpecification m_DepthAttachmentSpecification;

        RendererId m_ResolveFramebufferHandle{ 0 };
        std::vector<RendererId> m_ResolvedAttachments;

        FramebufferProperties m_Properties;
    };
}