#pragma once
#include "Buffer.hpp"
#include <cstdint>

namespace Ra
{
    /// Implementation for OpenGL vertex buffer
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(size_t size);
        OpenGLVertexBuffer(const float* vertices, size_t size);

        virtual void Bind() override;
        virtual void Unbind() override;
        virtual void SetData(const void* data, size_t size) override;
        virtual void SetLayout(const BufferLayout& layout) override;
        virtual BufferLayout GetLayout() const override;
    private:
        std::uint32_t m_Handle;
        BufferLayout m_Layout;
    };

    /************************************************************************/
    /* IndexBuffer                                                          */
    /************************************************************************/

    /// Implementation for OpenGL index (element) buffer
    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(const std::uint32_t* indices, std::uint32_t count);

        void Bind() override;
        void Unbind() override;

        std::uint32_t GetCount() const override { return m_Count; }
    private:
        std::uint32_t m_Handle;
        std::uint32_t m_Count;
    };
}