#pragma once
#include "Texture.hpp"
#include <glm/glm.hpp>

namespace Ra
{
    class Shader;

    class Material
    {
    public:
        void LoadTo(Ref<Shader>& shader) const;

        float Shininess{ 32.f };
        float Opacity{ 1.f };
        float Reflection{ 0.f };
        float Refraction{ 0.f };
        float RefractiveIndex{ 1.52f };
        bool CastShadows{ true };

        glm::vec3 BaseColor{ .36f };
        std::string Name{ "Default material" };
        std::vector<Ref<Texture>> Textures{};
    };

}