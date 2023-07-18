#pragma once
#include "Texture.hpp"
#include <glm/glm.hpp>

namespace Ra
{
    struct PointLight
    {
        glm::vec3 Color{ 1.f };
        float Intensity{ 1.f }; // @todo split
    };

    class Shader;

    class Material
    {
    public:
        void LoadTo(const Ref<Shader>& shader) const;

        float Shininess{ 32.f };
        float Opacity{ 1.f };

        glm::vec3 BaseColor{ .36f };
        std::string Name{ "Default material" };
        std::vector<Ref<Texture>> Textures{};
    };

}