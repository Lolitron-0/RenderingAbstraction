#pragma once
#include "Base.hpp"
#include "RendererAPI.hpp"
#include <glm/glm.hpp>
#include <gsl/gsl>

namespace Ra
{

class VertexBuffer;
class Shader;
class Texture;

struct QuadVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoord;
    float TexIndex;
};

struct Renderer2DStorage
{
    Renderer2DStorage()
        : QuadVertexBufferBase(new QuadVertex[MaxVertices])
    {
    }

    ~Renderer2DStorage()
    {
        delete[] QuadVertexBufferBase;
    }

    Renderer2DStorage(const Renderer2DStorage&) = default;
    Renderer2DStorage(Renderer2DStorage&&) = delete;
    auto operator=(const Renderer2DStorage&) -> Renderer2DStorage& = default;
    auto operator=(Renderer2DStorage&&) -> Renderer2DStorage& = delete;

    static constexpr uint32_t MaxQuads{ 20'000 };
    static constexpr uint32_t MaxVertices{ MaxQuads * 4 };
    static constexpr uint32_t MaxIndices{ MaxQuads * 6 };
    static constexpr int32_t QuadVertexCount{ 4 };
    static constexpr std::array<glm::vec4, 4> QuadVertexPositions{
        { { -0.5F, -0.5F, 0.0F, 1.0F },
          { 0.5F, -0.5F, 0.0F, 1.0F },
          { 0.5F, 0.5F, 0.0F, 1.0F },
          { -0.5F, 0.5F, 0.0F, 1.0F } }
    };
    static constexpr std::array<glm::vec2, 4> QuadTexCoords{
        { { 0.0F, 0.0F }, { 1.0F, 0.0F }, { 1.0F, 1.0F }, { 0.0F, 1.0F } }
    };

    Ref<VertexArray> QuadVertexArray;
    Ref<VertexBuffer> QuadVertexBuffer;
    Ref<Shader> QuadShader;

    gsl::owner<QuadVertex*> QuadVertexBufferBase{ nullptr };
    QuadVertex* QuadVertexBufferPtr{ nullptr };
    uint32_t QuadIndexCount{ 0 };

    Ref<Texture> WhiteTexure;
};

class Renderer2D
{
public:
    static void Init();
    static void Shutdown();

    // static void ResizeViewport(const glm::vec2& size);
    // static auto GetViewportSize() -> glm::vec2;
    // static auto GetResultTextureHandle() -> RendererId;

    static void BeginScene();

    /// Marks scene as finished
    static void EndScene();

    static void Flush();
    static void NextBatch();
    static void StartBatch();

    static void DrawQuad(const glm::mat4& transform, Ref<Texture> texture);
    static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);

private:
    static Renderer2DStorage s_Storage;
};
} // namespace Ra
