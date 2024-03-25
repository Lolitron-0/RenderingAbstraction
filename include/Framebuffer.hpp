#pragma once
#include "Base.hpp"
#include "Texture.hpp"

namespace Ra
{
/// Data needed for attachment initialization
struct AttachmentSpecification
{
public:
    AttachmentSpecification() = default;
    AttachmentSpecification(TextureFormat format)
        : Format(format)
    {
    }

    TextureFormat Format;
};

/// Framebuffer settings
struct FramebufferProperties
{
    std::uint32_t Width, Height;
    std::vector<AttachmentSpecification> Attachments;
    std::uint32_t Samples = 1;
};

/**
 * @brief Framebuffer object abstraction
 *
 * Attachments can be set up in FramebufferProperties object
 * @code{.cpp}
 * props.Attachments = {TextureFormat::RGBA8, TextureFormat::Depth};
 * @endcode
 */
class Framebuffer
{
public:
    virtual ~Framebuffer() = default;

    /// Binds this framebuffer
    virtual void StartWriting() = 0;
    /// Performs multi sampling resolving if needed
    virtual void StopWriting() = 0;

    virtual void Resize(std::uint32_t width, std::uint32_t height) = 0;

    /**
     * @brief Returns color attachment with given index
     * @warning This method does exactly what it's name says, so in case of
     * multisampled framebuffer, multisampled texture will be returned. To
     * receive just the 2d texture to draw use GetDrawTextureHandle();
     */
    virtual auto GetColorAttachmentHandle(std::size_t index = 0) const
        -> RendererId = 0;
    /// Binds color attachment with given index
    virtual void BindColorAttachment(std::size_t index = 0) = 0;
    /**
     * @brief Returns a resolved 2d texture with given index in case of
     * multisampled framebuffers and color attachment for other
     */
    virtual auto GetDrawTextureHandle(std::size_t index = 0) const
        -> RendererId = 0;

    /// Binds resolved 2d texture with given index in case of multisampled
    /// framebuffer and color attachment for others
    virtual void BindDrawTexture(std::size_t index = 0) = 0;
    virtual auto GetDepthTextureHandle() -> RendererId = 0;
    virtual void BindDepthTexture() = 0;

    virtual const FramebufferProperties& GetProperties() const = 0;

    /**
     * @brief Gets pixel value from attachment with given index
     * @param leaveBound if true main buffer remains bound after call
     * @return Pixel value
     * @note Buffer binding is internal so binding manually is not necessary
     */
    virtual auto ReadPixel(std::uint32_t attachmentIndex, int x, int y,
                           bool leaveBound = true) -> int = 0;

    static auto Create(const FramebufferProperties& props)
        -> Scope<Framebuffer>;
};

} // namespace Ra
