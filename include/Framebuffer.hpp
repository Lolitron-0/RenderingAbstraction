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
            :Format(format)
        {}

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
         * @warning This method does exactly what it's name says, so in case of multisampled framebuffer,
         * multisampled texture will be returned. To receive just the 2d texture to draw use GetDrawTextureHandle();
        */
        virtual RendererId GetColorAttachmentHandle(std::size_t index = 0) const = 0;
        /**
         * @brief Returns a resolved 2d texture in case of multisampled framebuffers and color attachment with given index for others
        */
        virtual RendererId GetDrawTextureHandle(std::size_t index = 0) const = 0;

        virtual const FramebufferProperties& GetProperties() const = 0;

        static Scope<Framebuffer> Create(const FramebufferProperties& props);
    };

}