#include "Renderer2D.hpp"
#include "Buffer.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"

namespace Ra
{

struct QuadVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoord;
    float TexIndex;
};

struct Renderer2DStorage
{
    static constexpr uint32_t MaxQuads{ 20'000 };
    static constexpr uint32_t MaxVertices{ MaxQuads * 4 };
    static constexpr uint32_t MaxIndices{ MaxQuads * 6 };

    Ref<VertexArray> QuadVertexArray;
    Ref<VertexBuffer> QuadVertexBuffer;
    Ref<Shader> QuadShader;
    QuadVertex* QuadVertexBufferBase{ nullptr };
};

} // namespace Ra
