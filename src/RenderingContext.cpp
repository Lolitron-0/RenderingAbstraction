#include "rapch.h"
#include "RenderingContext.hpp"
#include "OpenGL/OpenGLContext.hpp"
#include "Renderer.hpp"

namespace Ra
{
    Scope<RenderingContext> RenderingContext::Create(std::any windowHandle)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL: return MakeScope<OpenGLContext>(std::any_cast<GLFWwindow*>(windowHandle));
        default:
        {
            return nullptr;
        }
        }
    }
}