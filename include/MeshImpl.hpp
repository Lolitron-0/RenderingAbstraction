#pragma once
#include "SubMesh.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;

namespace Ra
{
    class MeshImpl : public Mesh
    {
    public:
        MeshImpl() = default;

        const std::vector<SubMesh>& GetSubMeshes() const override;
        std::string GetPath() const override;
        void ForEashSubmesh(const std::function<void(SubMesh&) >& func) override;

        void LoadMesh(const std::string& path);
    private:

        void ProcessNode_(aiNode* node, const aiScene* scene);
        SubMesh ProcessSubMesh_(aiMesh* subMesh, const aiScene* scene);
        std::vector<Ref<Texture>> LoadMaterialTextures_(aiMaterial* material, TextureType type);

        std::vector<SubMesh> m_SubMeshes;
        std::filesystem::path m_Path;
    };

}