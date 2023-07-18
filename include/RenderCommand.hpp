#pragma once
#include "Base.hpp"
#include "RendererAPI.hpp"
#include <glm/glm.hpp>

namespace Ra
{
    class VertexArray;

    /// Wrapper class - provides static interface to RendererAPI commands
    class RenderCommand
    {
    public:
        static void Init();

        /// Sets the rendering viewport
        static void SetViewport(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height);

        /// Sets viewport clear color
        static void SetClearColor(const glm::vec4& color);
        /// Clears viewport
        static void Clear();

        /// Draws given VertexArray using chosen DrawMode. Optionally index count can be specified so not all vertices will be drawn
        static void DrawIndexed(const Ref<VertexArray>& array, RendererAPI::DrawMode mode, std::size_t indexCount = 0);

    private:
        static Scope<RendererAPI> s_RendererAPI;
    };
}