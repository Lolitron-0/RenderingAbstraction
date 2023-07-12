#pragma once
#include "Texture.hpp"
#include <glm/glm.hpp>

namespace Ra
{
    class Material
    {
    public:
        Ref<Texture> DiffuseMap;
        Ref<Texture> SpecularMap;
        float Shininess;
        float Transparency{ 1.f };

        glm::vec3 BaseColor{ .36f };
        std::string Name{ "Default material" };
    };

}