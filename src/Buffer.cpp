#include "rapch.h"
#include "Buffer.hpp"
#include "Renderer.hpp"
#include "OpenGL/OpenGLBuffer.hpp"

namespace Ra
{
    Scope<VertexBuffer> VertexBuffer::Create(std::size_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL: return MakeScope<OpenGLVertexBuffer>(size);
        default: return nullptr;
        }
    }

    Scope<VertexBuffer> VertexBuffer::Create(const void* data, std::size_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL: return MakeScope<OpenGLVertexBuffer>(data, size);
        default: return nullptr;
        }
    }

    Scope<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, std::size_t count)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL: return MakeScope<OpenGLIndexBuffer>(indices, count);
        default: return nullptr;
        }
    }
}