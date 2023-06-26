#pragma once
#include "RendererAPI.hpp"

namespace Ra
{
    class OpenGLRendererAPI : public RendererAPI
    {
    public:
        virtual void Init() override;

        void SetClearColor(const glm::vec4& color) override;
        void Clear() override;

        void SetViewport(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height) override;

        void DrawIndexed(const Ref<VertexArray>& array, std::uint32_t indexCount) override;
        void DrawLines(const Ref<VertexArray>& array, std::uint32_t vertexCount) override;

    };
}