#include "rapch.h"
#include "RenderCommand.hpp"
#include "RendererAPI.hpp"
#include "VertexArray.hpp"
#include "Buffer.hpp"

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

    void RenderCommand::DrawIndexed(const Ref<VertexArray>& array, RendererAPI::DrawMode mode, std::size_t indexCount /*= 0*/)
    {
        s_RendererAPI->DrawIndexed(array, mode, indexCount ? indexCount : array->GetIndexBufer()->GetCount());
    }

    void RenderCommand::SetDepthBufferReadOnly(bool readOnly)
    {
        s_RendererAPI->SetDepthBufferReadOnly(readOnly);
    }

    Scope<Ra::RendererAPI> RenderCommand::s_RendererAPI = nullptr;
}