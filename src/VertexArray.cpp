#include "rapch.h"
#include "VertexArray.hpp"
#include "Renderer.hpp"
#include "OpenGL/OpenGLVertexArray.hpp"

namespace Ra
{
    Ref<VertexArray> VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexArray>();
        default: { RA_THROW_ERROR("No rendering API has been set!"); return nullptr; }
        }
    }
}