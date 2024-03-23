#include "rapch.h"
#include "RendererAPI.hpp"
#include "Renderer3D.hpp"
#include "OpenGL/OpenGLRendererAPI.hpp"

namespace Ra
{
    Scope<RendererAPI> RendererAPI::Create()
    {
        switch (Renderer3D::GetAPI())
        {
        case RendererAPI::API::OpenGL: return MakeScope<OpenGLRendererAPI>();
        default: { RA_ASSERT(false, "No rendering API has been set!"); return nullptr; }
        }
    }

    RendererAPI* RendererAPI::createInstance()
    {
        auto inst = RendererAPI::Create();
        return inst.release();
    }

}
