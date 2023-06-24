#pragma once
#include "Renderer.hpp"

namespace Ra
{
    class OpenGLRenderer : public Renderer
    {
    public:

        void SetClearColor(const glm::vec4& color) override;
        void Clear() override;

        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

    };
}