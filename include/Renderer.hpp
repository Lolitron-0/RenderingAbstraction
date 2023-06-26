#pragma once
#include "RendererAPI.hpp"
#include <glm/glm.hpp>

namespace Ra
{
    class Renderer
    {
    public:

        static void Init();

        static RendererAPI::API GetAPI() { return s_RendererAPI; }
        static void SetAPI(RendererAPI::API api) 
        {
            RA_ASSERT(s_RendererAPI == RendererAPI::API::None, "Rendering API can only be set once!");
            s_RendererAPI = api;
        }

        static void BeginScene();
        static void EndScene();

    private:
        static RendererAPI::API s_RendererAPI;
    };
}