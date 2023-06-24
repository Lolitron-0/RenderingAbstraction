#pragma once
#include <glm/glm.hpp>

namespace Ra
{
    class Renderer
    {
    public:
        enum class API
        {
            None = -1,
            OpenGL,
        };

    public:

        static API GetAPI() { return s_RendererAPI; }

        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;

    private:
        static API s_RendererAPI;
    };
}