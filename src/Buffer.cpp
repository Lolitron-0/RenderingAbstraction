#include "rapch.h"
#include "Buffer.hpp"
#include "Renderer.hpp"
#include "OpenGL/OpenGLBuffer.hpp"

namespace Ra
{
    Scope<VertexBuffer> VertexBuffer::Create(size_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL: return MakeScope<OpenGLVertexBuffer>(size);
        default: return nullptr;
        }
    }

    Scope<VertexBuffer> VertexBuffer::Create(const float* vertices, size_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL: return MakeScope<OpenGLVertexBuffer>(vertices, size);
        default: return nullptr;
        }
    }

    Scope<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL: return MakeScope<OpenGLIndexBuffer>(indices, size);
        default: return nullptr;
        }
    }
}