#include "rapch.h"
#include "OpenGL/OpenGLBuffer.hpp"
#include <glad/glad.h>

namespace Ra
{

    OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size)
    {
        glGenBuffers(1, &m_Handle);
        Bind();
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(const float* vertices, size_t size)
    {
        glGenBuffers(1, &m_Handle);
        Bind();
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    void OpenGLVertexBuffer::Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
    }

    void OpenGLVertexBuffer::Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLVertexBuffer::SetData(const void* data, size_t size)
    {
        Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout)
    {
        m_Layout = layout;
    }

    BufferLayout OpenGLVertexBuffer::GetLayout() const
    {
        return m_Layout;
    }

    /************************************************************************/
    /* IndexBuffer                                                          */
    /************************************************************************/

    OpenGLIndexBuffer::OpenGLIndexBuffer(const std::uint32_t* indices, std::uint32_t count)
        :m_Count(count)
    {
        glGenBuffers(1, &m_Handle);
        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    void OpenGLIndexBuffer::Bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle);
    }

    void OpenGLIndexBuffer::Unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}