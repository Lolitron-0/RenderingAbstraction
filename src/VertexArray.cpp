#include "rapch.h"
#include "VertexArray.hpp"
#include "Renderer.hpp"
#include "OpenGL/OpenGLVertexArray.hpp"

namespace Ra
{
    Scope<VertexArray> VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL: return MakeScope<OpenGLVertexArray>();
        default: { RA_THROW_ERROR("No rendering API has been set!"); return nullptr; }
        }
    }
}