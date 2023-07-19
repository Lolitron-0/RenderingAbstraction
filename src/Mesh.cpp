#include "rapch.h"
#include "Mesh.hpp"
#include "Material.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Profiler.hpp>

namespace Ra
{
    
    Mesh::Mesh(const std::string& path)
        :m_Path{ path }
    {
        LoadMesh(m_Path.string());
    }

    void Mesh::LoadMesh(const std::string& path)
    {
        PROFILER_SCOPE("Mesh::LoadMesh()");
        m_Path = path;
        Assimp::Importer importer;
        const aiScene* scene;
        {
            PROFILER_SCOPE("Mesh: read file");
            scene = importer.ReadFile(m_Path.string(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices |
                aiProcess_OptimizeMeshes | aiProcess_ImproveCacheLocality | aiProcess_GenUVCoords | aiProcess_CalcTangentSpace);
        }
        RA_ASSERT(scene && !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && scene->mRootNode, "Scene not loaded!" + importer.GetErrorString());
        ProcessNode_(scene->mRootNode, scene);
        RA_LOG("Loaded " + path + " mesh!");
    }

    const std::vector<SubMesh>& Mesh::GetSubMeshes() const
    {
        return m_SubMeshes;
    }

    std::string Mesh::GetPath() const
    {
        return m_Path.string();
    }

    void Mesh::ProcessNode_(aiNode* node, const aiScene* scene)
    {
        for (std::uint32_t i{ 0 }; i < node->mNumMeshes; i++)
        {
            aiMesh* subMesh = scene->mMeshes[node->mMeshes[i]];
            m_SubMeshes.push_back(ProcessSubMesh_(subMesh, scene));
        }
        for (std::uint32_t i{ 0 }; i < node->mNumChildren; i++)
            ProcessNode_(node->mChildren[i], scene);
    }

    SubMesh Mesh::ProcessSubMesh_(aiMesh* subMesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<std::uint32_t> indices;
        Material material;
        for (std::uint32_t i{ 0 }; i < subMesh->mNumVertices; i++)
        {
            Vertex v;
            v.Position =    { subMesh->mVertices[i].x,subMesh->mVertices[i].y, subMesh->mVertices[i].z };
            if (subMesh->mTextureCoords[0])
            v.TexCoord =    { subMesh->mTextureCoords[0][i].x, subMesh->mTextureCoords[0][i].y };
            if (subMesh->HasNormals())
                v.Normal =      { subMesh->mNormals[i].x, subMesh->mNormals[i].y, subMesh->mNormals[i].z };
            vertices.push_back(v);
        }
        for (std::uint32_t i{ 0 }; i < subMesh->mNumFaces; i++)
        {
            aiFace face = subMesh->mFaces[i];
            for (std::uint32_t j{ 0 }; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        if (subMesh->mMaterialIndex >= 0)
        {
            aiMaterial* materialSpec = scene->mMaterials[subMesh->mMaterialIndex];
            std::vector<Ref<Texture>> diffuseMaps = LoadMaterialTextures_(materialSpec, TextureType::Diffuse);
            if (!diffuseMaps.size())
                diffuseMaps.push_back(Texture::NullTexture);
            material.Textures.insert(material.Textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            std::vector<Ref<Texture>> specularMaps = LoadMaterialTextures_(materialSpec, TextureType::Specular);
            if (!specularMaps.size())
                specularMaps.push_back(Texture::NullTexture);
            material.Textures.insert(material.Textures.end(), specularMaps.begin(), specularMaps.end());

            std::vector<Ref<Texture>> normalMaps = LoadMaterialTextures_(materialSpec, TextureType::Normal);
            if (!normalMaps.size())
                normalMaps.push_back(Texture::NullTexture);
            material.Textures.insert(material.Textures.end(), normalMaps.begin(), normalMaps.end());

            aiColor3D baseColor = {material.BaseColor.r, material.BaseColor.g, material.BaseColor.b};
            materialSpec->Get(AI_MATKEY_COLOR_DIFFUSE, baseColor);
            material.BaseColor = {baseColor.r,baseColor.g, baseColor.b};
            float opacity = material.Opacity;
            materialSpec->Get(AI_MATKEY_OPACITY, opacity);
            material.Opacity = opacity;
            float shininess = material.Shininess;
            materialSpec->Get(AI_MATKEY_SHININESS, shininess);
            material.Shininess = shininess;
        }

        return SubMesh{ vertices, indices, material };
    }

    std::vector<Ra::Ref<Ra::Texture>> Mesh::LoadMaterialTextures_(aiMaterial* material, TextureType type)
    {
        aiTextureType aiType{ aiTextureType_NONE };
        switch (type)
        {
        case Ra::TextureType::Diffuse:
            aiType = aiTextureType_DIFFUSE;
            break;
        case Ra::TextureType::Specular:
            aiType = aiTextureType_SPECULAR;
            break;
        case Ra::TextureType::Normal:
            aiType = aiTextureType_NORMALS;
            break;
        }

        std::vector<Ref<Texture>> textures;
        for (std::uint32_t i{ 0 }; i < material->GetTextureCount(aiType); i++)
        {
            aiString pathBuf;
            material->GetTexture(aiType, i, &pathBuf);
            Ref<Texture> texture = Texture::Create(m_Path.parent_path().string() + "/" + std::string(pathBuf.C_Str()), TextureFormat::Color, type);
            textures.push_back(texture);
        }

        return textures;
    }

}