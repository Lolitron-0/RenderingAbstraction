#pragma once
#include "Base.hpp"

namespace Ra
{
    class VertexBuffer;
    class IndexBuffer;

    /// Abstract interface of a vertex array structure
    class VertexArray
    {
    public:
        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        /**
         * @brief Adds a buffer to other VertexBuffers assigned to this VertexArray
         * @param buffer Buffer to add (layout should be provided)
        */
        virtual void AddVertexBuffer(const Ref<VertexBuffer>& buffer) = 0;

        /**
         * @brief Assigns an IndexBuffer to this VertexBuffer
         * @param buffer Buffer to assign
        */
        virtual void SetIndexBuffer(const Ref<IndexBuffer>& buffer) = 0;

        /**
         * @brief Factory method for vertex array creation
         * @return Ref counted object to abstract vertex array interface, nullptr in case of unknown/unset rendering API
        */
        static Ref<VertexArray> Create();
    };
}