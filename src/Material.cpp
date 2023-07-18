#include "rapch.h"
#include "Material.hpp"
#include "Shader.hpp"

namespace Ra
{

    void Material::LoadTo(const Ref<Shader>& shader) const
    {
        shader->Bind();
        std::uint32_t diffuseNum = 1;
        std::uint32_t specularNum = 1;
        std::uint32_t normalNum = 1;
        for (std::uint32_t i{0}; i < Textures.size(); i++)
        {
            Textures[i]->Bind(i);
            std::string name;
            switch (Textures[i]->GetType())
            {
            case TextureType::Diffuse:
                name = "DiffuseMap" + std::to_string(diffuseNum++);
                break;
            case TextureType::Specular:
                name = "SpecularMap" + std::to_string(specularNum++);
                break;
            case TextureType::Normal:
                name = "NormalMap" + std::to_string(normalNum++);
                break;
            }
            shader->SetInt("u_Material." + name, i);
        }
        shader->SetVec3("u_Material.BaseColor", BaseColor);
        shader->SetFloat("u_Material.Shininess", Shininess);
        shader->SetFloat("u_Material.Opacity", Opacity);
    }

}