#include "RenderingContext.hpp"
#include "OpenGL/OpenGLContext.hpp"
#include "Renderer3D.hpp"
#include "rapch.h"

namespace Ra
{
Scope<RenderingContext> RenderingContext::Create(std::any windowHandle)
{
    switch (Renderer3D::GetAPI())
    {
    case RendererAPI::API::OpenGL:
        return MakeScope<OpenGLContext>(
            std::any_cast<GLFWwindow*>(windowHandle));
    default:
    {
        return nullptr;
    }
    }
}
} // namespace Ra
