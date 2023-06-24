#pragma once
#include "Base.hpp"

namespace Ra
{
    /// Data types enum for shader uniforms and buffer elements
    enum class UniformDataType { None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool };

    /// Statically evaluates given data type size
    static size_t UniformDataTypeSize(UniformDataType dataType)
    {
        switch (dataType)
        {
        case UniformDataType::Float: return 4;
        case UniformDataType::Float2: return 4 * 2;
        case UniformDataType::Float3: return 4 * 3;
        case UniformDataType::Float4: return 4 * 4;
        case UniformDataType::Mat3: return 4 * 3 * 3;
        case UniformDataType::Mat4: return 4 * 4 * 4;
        case UniformDataType::Int: return 4;
        case UniformDataType::Int2: return 4 * 2;
        case UniformDataType::Int3: return 4 * 3;
        case UniformDataType::Int4: return 4 * 4;
        case UniformDataType::Bool: return 1;
        default: return 0;
        }
    }

    /**
     * @brief Element of Buffer layout
     * 
     * Size and offset are calculated according to given data type and used to calculate vertex stride
    */
    struct BufferElement
    {
        UniformDataType DataType;
        std::string Name;
        size_t Offset;
        size_t Size;

        BufferElement(const UniformDataType& dataType, const std::string& name)
            : DataType(dataType), Name(name), Offset(0), Size(UniformDataTypeSize(DataType))
        {}
    };

    /**
     * @brief Class describing data arrangement in VertexBuffers
     * 
     * Layout can be defined as follows:
     * @code{.cpp}
     * BufferLayout layout = {
     *      {UniformDataType::Float3, "myPosition"},
     *      {UniformDataType::Float2, "myTexCoord"},
     *      {UniformDataType::Bool,   "myBoolForSomethingImportant"},
     * }
     * @endcode
    */
    class BufferLayout
    {
    public:
        BufferLayout() = default;

        BufferLayout(const std::initializer_list<BufferElement>& list)
            :m_BufferElements(list), m_Stride(0)
        {
            CalculateOffsetAndStride_();
        }

        std::vector<BufferElement>::iterator begin() { return m_BufferElements.begin(); }
        std::vector<BufferElement>::iterator end() { return m_BufferElements.end(); }
        std::vector<BufferElement>::reverse_iterator rbegin() { return m_BufferElements.rbegin(); }
        std::vector<BufferElement>::reverse_iterator rend() { return m_BufferElements.rend(); }

        std::vector<BufferElement>::const_iterator begin() const { return m_BufferElements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return m_BufferElements.cend(); }
        std::vector<BufferElement>::const_reverse_iterator rbegin() const { return m_BufferElements.rbegin(); }
        std::vector<BufferElement>::const_reverse_iterator rend() const { return m_BufferElements.rend(); }

    private:
        void CalculateOffsetAndStride_()
        {
            m_Stride = 0;
            for (auto& element : m_BufferElements)
            {
                element.Offset += m_Stride;
                m_Stride += element.Size;
            }
        }

        size_t m_Stride;
        std::vector<BufferElement> m_BufferElements;
    };

    
    /// Interface for vertex buffer object
    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void SetData(const void* data, size_t size) = 0;

        virtual void SetLayout(const BufferLayout& layout) = 0;
        virtual BufferLayout GetLayout() const = 0;

        /**
         * @brief Factory method for vertex buffer creation
         * @param size size of the buffer
         * @return Ref counted object to abstract vertex buffer interface, nullptr in case of unknown/unset rendering API
         * @note If no data provided dynamic drawn buffer will be created (slightly slower rendering)
        */
        static Ref<VertexBuffer> Create(size_t size);
        /**
         * @brief Factory method for vertex buffer creation
         * @param vertices Pointer to vertex data
         * @param size Size of the buffer
         * @return Ref counted object to abstract vertex buffer interface, nullptr in case of unknown/unset rendering API
        */
        static Ref<VertexBuffer> Create(const float* vertices, size_t size);
    };

    /// Interface for index buffer object
    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        /// Returns the number of indices stored
        virtual std::uint32_t GetCount() const = 0;

        /**
         * @brief Factory method for index buffer creation
         * @param indices Pointer to index array
         * @param count Number of indices
         * @return Ref counted object to abstract index buffer interface, nullptr in case of unknown/unset rendering API
        */
        static Ref<IndexBuffer> Create(const std::uint32_t* indices, std::uint32_t count);
    };
}