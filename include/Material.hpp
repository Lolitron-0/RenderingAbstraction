#pragma once
#include "Texture.hpp"
#include <glm/glm.hpp>

namespace Ra
{
    struct PointLight
    {
        glm::vec3 Color{ 1.f };
        float Intensity{ 1.f };
    };

    class Material
    {
    public:
        Ref<Texture> DiffuseMap{ Texture::NullTexture };
        Ref<Texture> SpecularMap{ Texture::NullTexture };
        float Shininess{ 32.f };
        float Transparency{ 1.f };

        glm::vec3 BaseColor{ .36f };
        std::string Name{ "Default material" };
    };

}