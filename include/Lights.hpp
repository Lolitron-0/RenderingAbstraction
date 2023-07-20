#pragma once
#include <glm/glm.hpp>

namespace Ra
{
    struct PointLight
    {
        glm::vec3 Color{ 1.f };
        float Intensity{ 1.f }; // @todo split
    };

    struct DirLight
    {
        glm::vec3 Color{ 1.f };
        glm::vec3 Direction{ 0, -1, 0 };
        float Intensity{ 1.f };
    };

}