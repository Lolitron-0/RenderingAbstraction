#include "rapch.h"
#include "Renderer.hpp"
#include "RenderCommand.hpp"

namespace Ra
{

    RendererAPI::API Renderer::s_RendererAPI = RendererAPI::API::None;

    Scope<Ra::Renderer::SceneData> Renderer::s_SceneData = std::make_unique<Renderer::SceneData>();

    void Renderer::Init()
    {
        if (s_RendererAPI != RendererAPI::API::None)
            RenderCommand::Init();
    }

    void Renderer::Shutdown()
    {
    }

    void Renderer::BeginScene(const Camera& camera)
    {
        s_SceneData->ViewProjectionMatrix = camera.GetViewProjection();
    }

    void Renderer::EndScene()
    {

    }

    void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader)
    {
        shader->Bind();
        shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);

        RenderCommand::DrawIndexed(vertexArray);
    }

}