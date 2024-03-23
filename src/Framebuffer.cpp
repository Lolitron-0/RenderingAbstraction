#include "rapch.h"
#include "Framebuffer.hpp"
#include "Renderer3D.hpp"
#include "OpenGL/OpenGLFramebuffer.hpp"

namespace Ra
{

    Scope<Framebuffer> Framebuffer::Create(const FramebufferProperties& props)
    {
        switch (Renderer3D::GetAPI())
        {
        case RendererAPI::API::OpenGL: return MakeScope<OpenGLFramebuffer>(props);
        default: return nullptr;
        }
    }

}
