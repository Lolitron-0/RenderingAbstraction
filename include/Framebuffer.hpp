#pragma once 
#include "Base.hpp"

namespace Ra
{
    struct FramebufferProperties
    {
        std::uint32_t Width, Height;
        std::uint32_t Samples = 1;
    };

    class Framebuffer
    {
    public:
        virtual ~Framebuffer() = default;

        virtual void StartWriting() = 0;
        virtual void StopWriting() = 0;

        virtual void Resize(std::uint32_t width, std::uint32_t height) = 0;

        virtual RendererId GetColorAttachmentHandle() const = 0;

        virtual const FramebufferProperties& GetProperties() const = 0;

        static Ref<Framebuffer> Create(const FramebufferProperties& props);
    };

}