#include "rapch.h"
#include "RenderCommand.hpp"
#include "RendererAPI.hpp"

namespace Ra
{
    void RenderCommand::Init()
    {
        s_RendererAPI = RendererAPI::Create();
        s_RendererAPI->Init();
    }

    void RenderCommand::SetViewport(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height)
    {
        s_RendererAPI->SetViewport(x, y, width, height);
    }

    void RenderCommand::SetClearColor(const glm::vec4& color)
    {
        s_RendererAPI->SetClearColor(color);
    }

    void RenderCommand::Clear()
    {
        s_RendererAPI->Clear();
    }

    void RenderCommand::DrawIndexed(const Ref<VertexArray>& array, std::uint32_t indexCount)
    {
        s_RendererAPI->DrawIndexed(array, indexCount);
    }

    void RenderCommand::DrawLines(const Ref<VertexArray>& array, std::uint32_t vertexCount)
    {
        s_RendererAPI->DrawLines(array, vertexCount);
    }

    Scope<Ra::RendererAPI> RenderCommand::s_RendererAPI = nullptr;
}