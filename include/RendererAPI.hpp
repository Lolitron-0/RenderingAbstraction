#pragma once
#include "Base.hpp"
#include <glm/glm.hpp>

namespace Ra
{
    class VertexArray;

    class RendererAPI
    {
    public:
        enum class API
        {
            None = -1,
            OpenGL,
        };

    public:
        virtual ~RendererAPI() = default;

        virtual void Init() = 0;

        virtual void SetViewport(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height) = 0;

        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed(const Ref<VertexArray>& array, std::uint32_t indexCount) = 0;
        virtual void DrawLines(const Ref<VertexArray>& array, std::uint32_t vertexCount) = 0;

        static Scope<RendererAPI> Create();
    };
}