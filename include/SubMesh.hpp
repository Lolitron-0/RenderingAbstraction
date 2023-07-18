#pragma once
#include <glm/glm.hpp>
#include "Material.hpp"

namespace Ra
{
    class VertexArray;
    class Texture;
    class Shader;

    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoord;
    };

    class SubMesh
    {
    public:
        SubMesh(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices, const Material& material);
        Ref<VertexArray> GetVertexArray() const;

        const Material& GetMaterial() const;

    private:
        void SetupSubmesh_();

        Ref<VertexArray> m_VertexArray;
        std::vector<Vertex> m_VertexData;
        std::vector<std::uint32_t> m_IndexData;
        Material m_Material;
    };

}