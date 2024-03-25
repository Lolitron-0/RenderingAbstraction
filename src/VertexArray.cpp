#include "VertexArray.hpp"
#include "OpenGL/OpenGLVertexArray.hpp"
#include "RenderCommand.hpp"
#include "rapch.h"

namespace Ra
{
Scope<VertexArray> VertexArray::Create()
{
    switch (RenderCommand::GetAPI())
    {
    case RendererAPI::API::OpenGL:
        return MakeScope<OpenGLVertexArray>();
    default:
    {
        RA_THROW_ERROR("No rendering API has been set!");
        return nullptr;
    }
    }
}
} // namespace Ra
