#include "rapch.h"
#include "OpenGL/OpenGLVertexArray.hpp"
#include "Buffer.hpp"
#include <glad/glad.h>

namespace Ra
{

    OpenGLVertexArray::OpenGLVertexArray()
    {
        glGenVertexArrays(1, &m_Handle);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_Handle);
    }

    void OpenGLVertexArray::Bind()
    {
        glBindVertexArray(m_Handle);
    }

    void OpenGLVertexArray::Unbind()
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& buffer)
    {
        RA_ASSERT(buffer->GetLayout().GetCount(), "Buffer has no layout!");
        Bind();
        buffer->Bind();

        const auto& layout = buffer->GetLayout();

        for (auto element : layout)
        {
            switch (element.DataType)
            {
            case BufferDataType::Float:
            case BufferDataType::Float2:
            case BufferDataType::Float3:
            case BufferDataType::Float4:
            {
                glEnableVertexAttribArray(m_VertexAttribIndex);
                glVertexAttribPointer(m_VertexAttribIndex,
                    element.GetComponentCount(),
                    GL_FLOAT,
                    GL_FALSE,
                    static_cast<GLsizei>(layout.GetStride()),
                    (const void*)element.Offset);
                m_VertexAttribIndex++;
                break;
            }
            case BufferDataType::Int:
            case BufferDataType::Int2:
            case BufferDataType::Int3:
            case BufferDataType::Int4:
            {
                glEnableVertexAttribArray(m_VertexAttribIndex);
                glVertexAttribIPointer(m_VertexAttribIndex,
                    element.GetComponentCount(),
                    GL_INT,
                    static_cast<GLsizei>(layout.GetStride()),
                    (const void*)element.Offset);
                m_VertexAttribIndex++;
                break;
            }
            case BufferDataType::Mat3:
            case BufferDataType::Mat4:
            {
                std::uint8_t count = element.GetComponentCount();
                for (int i{ 0 }; i < count; i++)
                {
                    glEnableVertexAttribArray(m_VertexAttribIndex);
                    glVertexAttribPointer(m_VertexAttribIndex,
                        count, // component count == number of floats in a row
                        GL_FLOAT,
                        GL_FALSE,
                        static_cast<GLsizei>(layout.GetStride()),
                        (const void*)(element.Offset + sizeof(float) * count * i));
                    m_VertexAttribIndex++;
                }
                break;
            }
            default:
                RA_ASSERT(false, "Unknown data type!");
                break;
            }
        }
        m_VertexBuffers.push_back(buffer);
        buffer->Unbind();
        Unbind();
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& buffer)
    {
        Bind();
        buffer->Bind();

        m_IndexBuffer = buffer;
        Unbind();
    }

    std::vector<Ra::Ref<Ra::VertexBuffer>> OpenGLVertexArray::GetVertexBuffers() const
    {
        return m_VertexBuffers;
    }

    Ra::Ref<Ra::IndexBuffer> OpenGLVertexArray::GetIndexBufer() const
    {
        return m_IndexBuffer;
    }

}