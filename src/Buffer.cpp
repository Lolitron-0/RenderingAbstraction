#include "rapch.h"
#include "Buffer.hpp"
#include "Renderer.hpp"
#include "OpenGL/OpenGLBuffer.hpp"

namespace Ra
{
    Ref<VertexBuffer> VertexBuffer::Create(size_t size)
    {
        switch (Renderer::GetAPI())
        {
        case Renderer::API::OpenGL: return std::make_shared<OpenGLVertexBuffer>(size);
        default: return nullptr;
        }
    }

    Ref<VertexBuffer> VertexBuffer::Create(const float* vertices, size_t size)
    {
        switch (Renderer::GetAPI())
        {
        case Renderer::API::OpenGL: return std::make_shared<OpenGLVertexBuffer>(vertices, size);
        default: return nullptr;
        }
    }

    Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case Renderer::API::OpenGL: return std::make_shared<OpenGLIndexBuffer>(indices, size);
        default: return nullptr;
        }
    }
}