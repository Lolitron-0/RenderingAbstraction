#include "rapch.h"
#include "SubMesh.hpp"
#include "Buffer.hpp"
#include "VertexArray.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include <Profiler.hpp>

namespace Ra
{

    SubMesh::SubMesh(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices,const Material& material)
        : m_VertexData{ vertices }, m_IndexData{ indices }, m_Material{ material }
    {
        SetupSubmesh_();
    }

    Ra::Ref<Ra::VertexArray> SubMesh::GetVertexArray() const
    {
        return m_VertexArray;
    }

    const Ra::Material& SubMesh::GetMaterial() const
    {
        return m_Material;
    }

    Ra::Material& SubMesh::GetMaterial()
    {
        return m_Material;
    }

    void SubMesh::SetupSubmesh_()
    {
        PROFILER_SCOPE("SubMesh::SetupSubmesh_()");
        BufferLayout l = {
            {BufferDataType::Float3, "Position"},
            {BufferDataType::Float3, "Normal"},
            {BufferDataType::Float2, "TexCoord"},
        };
        Ref<VertexBuffer> vbo = VertexBuffer::Create(&m_VertexData[0], m_VertexData.size() * sizeof(Vertex));
        vbo->SetLayout(l);
        Ref<IndexBuffer> ebo = IndexBuffer::Create(&m_IndexData[0], m_IndexData.size());
        m_VertexArray = VertexArray::Create();
        m_VertexArray->AddVertexBuffer(vbo);
        m_VertexArray->SetIndexBuffer(ebo);
    }

}