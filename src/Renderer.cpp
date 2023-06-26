#include "rapch.h"
#include "Renderer.hpp"
#include "RenderCommand.hpp"

namespace Ra
{

    RendererAPI::API Renderer::s_RendererAPI = RendererAPI::API::None; 

    void Renderer::Init()
    {
        if (s_RendererAPI != RendererAPI::API::None)
            RenderCommand::Init();
    }

}