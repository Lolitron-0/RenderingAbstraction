#include "Material.hpp"
#include "Shader.hpp"
#include "rapch.h"
#include <Profiler.hpp>

namespace Ra
{

void Material::LoadTo(Ref<Shader>& shader) const
{
    PROFILER_SCOPE("Material::LoadTo()");
    shader->Bind();
    std::uint32_t diffuseNum = 1;
    std::uint32_t specularNum = 1;
    std::uint32_t normalNum = 1;
    for (std::uint32_t i{ 0 }; i < Textures.size(); i++)
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
    shader->SetFloat("u_Material.Reflection", Reflection);
    shader->SetFloat("u_Material.Refraction", Refraction);
    shader->SetFloat("u_Material.RefractiveIndex", RefractiveIndex);
    shader->SetFloat("u_Material.Opacity", Opacity);
    shader->SetFloat("u_Material.CastShadows", CastShadows);
}

} // namespace Ra
