#include "Renderer2D.hpp"
#include "Buffer.hpp"
#include "Profiler.hpp"
#include "RenderCommand.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"

namespace Ra
{

Renderer2D::SceneData Renderer2D::s_SceneData{};
RendererStats Renderer2D::s_Stats{};
Renderer2DStorage Renderer2D::s_Storage{};

void Renderer2D::Init()
{
    PROFILER_SCOPE("Renderer2D::Init");

    RenderCommand::Init();

    s_Storage.QuadVertexArray = VertexArray::Create();

    s_Storage.QuadVertexBuffer = VertexBuffer::Create(
        Renderer2DStorage::MaxVertices * sizeof(QuadVertex));
    s_Storage.QuadVertexBuffer->SetLayout(
        { { BufferDataType::Float3, "a_Position" },
          { BufferDataType::Float4, "a_Color" },
          { BufferDataType::Float2, "a_TexCoord" },
          { BufferDataType::Float, "a_TexIndex" } });
    s_Storage.QuadVertexArray->AddVertexBuffer(s_Storage.QuadVertexBuffer);

    gsl::owner<uint32_t*> quadIndices{
        new uint32_t[Renderer2DStorage::MaxIndices]
    };
    uint32_t offset{};
    for (int32_t i{ 0 }; i < Renderer2DStorage::MaxIndices; i += 6)
    {
        quadIndices[i + 0] = offset + 0;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 2;

        quadIndices[i + 3] = offset + 2;
        quadIndices[i + 4] = offset + 3;
        quadIndices[i + 5] = offset + 0;

        offset += 4;
    }
    Ref<IndexBuffer> quadIndexBuffer{ IndexBuffer::Create(
        quadIndices, Renderer2DStorage::MaxIndices) };

    s_Storage.QuadVertexArray->SetIndexBuffer(quadIndexBuffer);

    delete[] quadIndices;

    uint32_t whiteTextureData{ 0xff'ff'ff'ff };
    s_Storage.WhiteTexure =
        Texture::Create(reinterpret_cast<uint8_t*>(&whiteTextureData), 1, 1, 4);

    s_Storage.QuadShader = Shader::Create(GetShadersDir() + "quad.vert",
                                          GetShadersDir() + "quad.frag");
}

void Renderer2D::BeginScene(const glm::mat4& viewProjMatrix)
{
    s_Stats.OnSceneBegin();
	s_SceneData.ViewProjection = viewProjMatrix;
    StartBatch();
}
void Renderer2D::EndScene()
{
    Flush();
	s_Stats.OnSceneEnd();
}

void Renderer2D::StartBatch()
{
    s_Storage.QuadIndexCount = 0;
    s_Storage.QuadVertexBufferPtr = s_Storage.QuadVertexBufferBase;
}

void Renderer2D::Flush()
{
    PROFILER_SCOPE("Renderer2D::Flush")

    if (s_Storage.QuadIndexCount)
    {
        auto batchSize{ static_cast<size_t>(
            reinterpret_cast<uint8_t*>(s_Storage.QuadVertexBufferPtr) -
            reinterpret_cast<uint8_t*>(s_Storage.QuadVertexBufferBase)) };
        s_Storage.QuadVertexBuffer->SetData(s_Storage.QuadVertexBufferBase,
                                            batchSize);

        s_Storage.QuadVertexArray->Bind();
        s_Storage.QuadShader->Bind();
		s_Storage.QuadShader->SetMat4("u_ViewProjection", s_SceneData.ViewProjection);
        s_Storage.WhiteTexure->Bind();
        RenderCommand::DrawIndexed(s_Storage.QuadVertexArray,
                                   RendererAPI::DrawMode::Triangles,
                                   s_Storage.QuadIndexCount);
        s_Stats.DrawCalls++;
        s_Stats.Indices += s_Storage.QuadIndexCount;
    }
}

void Renderer2D::NextBatch()
{
    Flush();
    StartBatch();
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size,
                          const glm::vec4& color)
{
    auto transform{ glm::translate(glm::mat4{ 1.0F },
                                   glm::vec3{ position.x, position.y, 1.0F }) *
                    glm::scale(glm::mat4{ 1.0F },
                               glm::vec3{ size.x, size.y, 1.0F }) };
    DrawQuad(transform, color);
}

void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
{
    if (s_Storage.QuadIndexCount >= Renderer2DStorage::MaxIndices)
    {
        NextBatch();
    }

    for (int32_t i{ 0 }; i < Renderer2DStorage::QuadVertexCount; i++)
    {
        s_Storage.QuadVertexBufferPtr->Position =
            transform * Renderer2DStorage::QuadVertexPositions.at(i);
        s_Storage.QuadVertexBufferPtr->Color = color;
        s_Storage.QuadVertexBufferPtr->TexCoord =
            Renderer2DStorage::QuadTexCoords.at(i);
        s_Storage.QuadVertexBufferPtr->TexIndex = 0.F;
        s_Storage.QuadVertexBufferPtr++; // todo: iterators and vectors
    }

    s_Storage.QuadIndexCount += 6;
}

auto Renderer2D::GetStats() -> RendererStats
{
    return s_Stats;
}

} // namespace Ra
