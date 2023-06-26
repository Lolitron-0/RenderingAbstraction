#pragma once
#include "VertexArray.hpp"

namespace Ra
{
    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray();

        void Bind() override;
        void Unbind() override;

        void AddVertexBuffer(const Ref<VertexBuffer>& buffer) override;
        void SetIndexBuffer(const Ref<IndexBuffer>& buffer) override;

    private:
        std::uint32_t m_Handle;
        std::vector<Ref<VertexBuffer>> m_VertexBuffers;
        Ref<IndexBuffer> m_IndexBuffer;
        std::uint32_t m_VertexAttribIndex{ 0 };

    };
}