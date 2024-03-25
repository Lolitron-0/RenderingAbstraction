#include "RenderCommand.hpp"
#include "Buffer.hpp"
#include "RendererAPI.hpp"
#include "VertexArray.hpp"
#include "rapch.h"

namespace Ra
{
void RenderCommand::Init()
{
    s_RendererAPI = RendererAPI::Create();
}

void RenderCommand::SetViewport(std::uint32_t x, std::uint32_t y,
                                std::uint32_t width, std::uint32_t height)
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

void RenderCommand::DrawIndexed(const Ref<VertexArray>& array,
                                RendererAPI::DrawMode mode,
                                std::size_t indexCount /*= 0*/)
{
    s_RendererAPI->DrawIndexed(array, mode,
                               indexCount ? indexCount
                                          : array->GetIndexBufer()->GetCount());
}

	void RenderCommand::SetDepthFunc(RendererAPI::DepthFunc func){
		s_RendererAPI->SetDepthFunc(func);
	}

void RenderCommand::SetDepthBufferReadOnly(bool readOnly)
{
    s_RendererAPI->SetDepthBufferReadOnly(readOnly);
}

auto RenderCommand::GetAPI() -> RendererAPI::API
{
    return s_RendererAPI->GetAPI();
}

Scope<Ra::RendererAPI> RenderCommand::s_RendererAPI = nullptr;
} // namespace Ra
