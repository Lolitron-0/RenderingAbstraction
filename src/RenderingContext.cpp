#include "RenderingContext.hpp"
#include "OpenGL/OpenGLContext.hpp"
#include "RenderCommand.hpp"
#include "rapch.h"

namespace Ra
{
std::function<void(void)> RenderingContext::s_ContextLoadCallback = []() {};

Scope<RenderingContext> RenderingContext::Create(std::any windowHandle)
{
    switch (RenderCommand::GetAPI())
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
void RenderingContext::SetOnContextLoadCallback(
    std::function<void(void)> callback)
{
    s_ContextLoadCallback = std::move(callback);
}
} // namespace Ra
