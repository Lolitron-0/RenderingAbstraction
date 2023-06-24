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
        case Renderer::API::OpenGL: return std::make_unique<OpenGLContext>(std::any_cast<GLFWwindow*>(windowHandle));
        default:
        {
            return nullptr;
        }
        }
    }
}