#pragma once
#include "SubMesh.hpp"
#include "Texture.hpp"

    struct aiNode;
    struct aiScene;
    struct aiMesh;
    struct aiMaterial;

namespace Ra
{
    class Mesh
    {
    public:
        Mesh(const std::string& path);
        void LoadMesh(const std::string& path);

        const std::vector<SubMesh>& GetSubMeshes() const;
    private:
        void ProcessNode_(aiNode* node, const aiScene* scene);
        SubMesh ProcessSubMesh_(aiMesh* subMesh, const aiScene* scene);
        std::vector<Ref<Texture>> LoadMaterialTextures_(aiMaterial* material, TextureType type);

        std::vector<SubMesh> m_SubMeshes;
        std::filesystem::path m_Path;
    };
}