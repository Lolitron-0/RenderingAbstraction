#pragma once
#include "Base.hpp"
#include <glm/glm.hpp>

namespace Ra
{
    class VertexArray;

    /// Non-static interface for API-level rendering commands
    class RendererAPI : public Singleton<RendererAPI>
    {
    public:
        enum class API : std::uint8_t
        {
            None,
            OpenGL,
        };

        enum class DrawMode : std::uint8_t
        {
            Points = 0,
            Triangles,
            TriangleFan,
            TriangleStrip,
            Lines,
            LineLoop,
            LineStrip,

            Last = LineStrip,
            Count
        };

        enum class DepthFunc : std::uint8_t
        {
            Always,
            Never,
            Less,
            Equal,
            Lequal,
            Greater,
            Gequal,
            NotEqual,
        };

        enum class CullFace : std::uint8_t
        {
            Both,
            Back,
            Front,
        };

    public:
        virtual ~RendererAPI() = default;

        virtual void Init() = 0;

        virtual void SetViewport(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height) = 0;

        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed(const Ref<VertexArray>& array, DrawMode mode, std::size_t indexCount = 0) = 0;

        virtual void SetDepthBufferReadOnly(bool readOnly) = 0;

        virtual void SetDepthFunc(DepthFunc func) = 0;

        virtual void SetCullFace(CullFace face) = 0;

        static Scope<RendererAPI> Create();

    protected:
        virtual int ToAPIDrawMode_(RendererAPI::DrawMode mode) = 0;
        virtual int ToAPIDepthFunc_(RendererAPI::DepthFunc func) = 0;

        static RendererAPI* createInstance();

        friend class Singleton<RendererAPI>;

    };
}