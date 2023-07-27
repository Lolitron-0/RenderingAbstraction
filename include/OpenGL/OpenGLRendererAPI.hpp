#pragma once
#include "RendererAPI.hpp"

namespace Ra
{
    /// Implementation for OpenGL rendering API commands
    class OpenGLRendererAPI : public RendererAPI
    {
    public:
        OpenGLRendererAPI();

        void Init() override;

        void SetClearColor(const glm::vec4& color) override;
        void Clear() override;

        void SetViewport(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height) override;

        void DrawIndexed(const Ref<VertexArray>& array, DrawMode mode, std::size_t indexCount = 0) override;

        void SetDepthBufferReadOnly(bool readOnly) override;

        void SetDepthFunc(DepthFunc func) override;
        void SetCullFace(CullFace face) override;

    protected:
        int ToAPIDrawMode_(RendererAPI::DrawMode mode) override;
        int ToAPIDepthFunc_(RendererAPI::DepthFunc func) override;

    private:
        const static std::unordered_map<RendererAPI::DrawMode, int> s_ToAPIDrawModeMap;

    };
}