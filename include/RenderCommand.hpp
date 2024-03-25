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
    static void SetViewport(std::uint32_t x, std::uint32_t y,
                            std::uint32_t width, std::uint32_t height);

    /// Sets viewport clear color
    static void SetClearColor(const glm::vec4& color);
    /// Clears viewport
    static void Clear();

    /// Draws given VertexArray using chosen DrawMode. Optionally index count
    /// can be specified so not all vertices will be drawn
    static void DrawIndexed(const Ref<VertexArray>& array,
                            RendererAPI::DrawMode mode,
                            std::size_t indexCount = 0);

	/// Sets depth test funcion
	static void SetDepthFunc(RendererAPI::DepthFunc func);

    /// Enables/Disables writing to depth buffer
    static void SetDepthBufferReadOnly(bool readOnly);

	static auto GetAPI() -> RendererAPI::API;

private:
    static Scope<RendererAPI> s_RendererAPI;
    static RendererAPI::API s_RenderingAPICode;
};
} // namespace Ra
