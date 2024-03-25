#include "RendererAPI.hpp"
#include "OpenGL/OpenGLRendererAPI.hpp"
#include "Renderer3D.hpp"
#include "rapch.h"

namespace Ra
{

RendererAPI::API RendererAPI::s_Api{ RendererAPI::API::None };

Scope<RendererAPI> RendererAPI::Create()
{
    switch (s_Api)
    {
    case RendererAPI::API::OpenGL:
        return MakeScope<OpenGLRendererAPI>();
    default:
    {
        RA_ASSERT(false, "No rendering API has been set!");
        return nullptr;
    }
    }
}

auto RendererAPI::GetAPI() -> RendererAPI::API
{
    return s_Api;
}

void RendererAPI::SetAPI(RendererAPI::API api)
{
    s_Api = api;
}

} // namespace Ra
