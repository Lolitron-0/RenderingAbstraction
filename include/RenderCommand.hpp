#pragma once
#include "Base.hpp"
#include <glm/glm.hpp>

namespace Ra
{
    class RendererAPI;
    class VertexArray;

    /// Wrapper class - provides static interface to RendererAPI commands
    class RenderCommand
    {
    public:
        static void Init();

        static void SetViewport(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height);

        static void SetClearColor(const glm::vec4& color);
        static void Clear();

        static void DrawIndexed(const Ref<VertexArray>& array, std::uint32_t indexCount = 0);
        static void DrawLines(const Ref<VertexArray>& array, std::uint32_t vertexCount);

    private:
        static Scope<RendererAPI> s_RendererAPI;
    };
}