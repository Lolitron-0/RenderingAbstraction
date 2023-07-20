#pragma once
#include "Texture.hpp"
#include <glm/glm.hpp>

namespace Ra
{
    class Shader;

    class Material
    {
    public:
        void LoadTo(const Ref<Shader>& shader) const;

        float Shininess{ 32.f };
        float Opacity{ 1.f };
        bool SkipLight{ false };

        glm::vec3 BaseColor{ .36f };
        std::string Name{ "Default material" };
        std::vector<Ref<Texture>> Textures{};
    };

}