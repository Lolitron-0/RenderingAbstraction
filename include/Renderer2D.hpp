#pragma once
#include "Base.hpp"
#include "RendererAPI.hpp"
#include <glm/glm.hpp>

namespace Ra
{

class Renderer2D
{
    void Init();
    void Shutdown();

    static void ResizeViewport(const glm::vec2& size);
    static auto GetViewportSize() -> glm::vec2;
    static auto GetResultTextureHandle() -> RendererId;

    /// Initializes scene with given view-projection matrix
    static void BeginScene();
    /// Marks scene as finished
    static void EndScene();

    static void DrawQuad(const glm::vec2& position, const glm::vec2& size,
                         const glm::vec4& color);

    static RendererAPI::API GetAPI()
    {
        return s_RendererAPI;
    }

    /// Sets current rendering API. Can only be set once
    static void SetAPI(RendererAPI::API api)
    {
        RA_ASSERT(s_RendererAPI == RendererAPI::API::None,
                  "Rendering API can only be set once!");
        s_RendererAPI = api;
    }

private:
    static RendererAPI::API s_RendererAPI;
};
} // namespace Ra
