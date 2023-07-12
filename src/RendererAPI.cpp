#include "rapch.h"
#include "RendererAPI.hpp"
#include "Renderer.hpp"
#include "OpenGL/OpenGLRendererAPI.hpp"

namespace Ra
{
    Scope<RendererAPI> RendererAPI::Create()
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL: return MakeScope<OpenGLRendererAPI>();
        default: { RA_ASSERT(false, "No rendering API has been set!"); return nullptr; }
        }
    }
}
