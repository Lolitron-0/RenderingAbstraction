#include "rapch.h"
#include "Framebuffer.hpp"
#include "Renderer.hpp"
#include "OpenGL/OpenGLFramebuffer.hpp"

namespace Ra
{

    Ra::Ref<Ra::Framebuffer> Framebuffer::Create(const FramebufferProperties& props)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL: return std::make_shared<OpenGLFramebuffer>(props);
        default: return nullptr;
        }
    }

}