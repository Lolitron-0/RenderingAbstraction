#include "Framebuffer.hpp"
#include "OpenGL/OpenGLFramebuffer.hpp"
#include "RenderCommand.hpp"
#include "rapch.h"

namespace Ra
{

auto Framebuffer::Create(const FramebufferProperties& props) -> Scope<Framebuffer>
{
    switch (RenderCommand::GetAPI())
    {
    case RendererAPI::API::OpenGL:
        return MakeScope<OpenGLFramebuffer>(props);
    default:
        return nullptr;
    }
}

} // namespace Ra
