#include "rapch.h"
#include "Renderer.hpp"
#include "RenderCommand.hpp"
#include "Buffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"

namespace Ra
{

    RendererAPI::API Renderer::s_RendererAPI = RendererAPI::API::None;

    Scope<Ra::Renderer::SceneData> Renderer::s_SceneData = std::make_unique<Renderer::SceneData>();


    struct Renderer3DData
    {
        Ref<VertexBuffer>   CubeVertexBuffer;
        Ref<IndexBuffer>    CubeIndexBuffer;
        Ref<VertexArray>    CubeVertexArray;

        Ref<Shader> PhongShader;
    };

    static Renderer3DData s_Data;

    void Renderer::Init()
    {
        if (s_RendererAPI != RendererAPI::API::None)
            RenderCommand::Init();

        float cubeData[] =
        {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,    1.0f, 1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,    1.0f, 1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,    1.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,    1.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,    0.0f, 0.0f, 1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,    1.0f, 1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,    1.0f, 1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,    1.0f, 0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,    1.0f, 0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,    1.0f, 1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,    1.0f, 1.0f, 1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,    1.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,    1.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,    0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,    0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,    1.0f, 1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,    1.0f, 1.0f, 1.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,    1.0f, 0.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,    1.0f, 1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,    1.0f, 1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,    1.0f, 1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,    1.0f, 1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,    1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,    0.0f, 0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,    1.0f, 1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,    1.0f, 1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,    1.0f, 1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,    1.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,    0.0f, 0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,    1.0f, 1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,    1.0f, 1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,    1.0f, 0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,    1.0f, 0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,    1.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,    1.0f, 1.0f, 1.0f
        };
        
        s_Data.CubeVertexBuffer = VertexBuffer::Create(cubeData, sizeof(cubeData));
        s_Data.CubeVertexBuffer->SetLayout({
            {BufferDataType::Float3, "position"},
            {BufferDataType::Float3, "normals"},
            {BufferDataType::Float2, "texCoords"},
            {BufferDataType::Float3, "color"},
            });

        std::uint32_t* cubeIndices = new std::uint32_t[36];
        for (int i = 0; i < 36; i++) cubeIndices[i] = i;
        s_Data.CubeIndexBuffer = IndexBuffer::Create(cubeIndices, 36);
        delete[] cubeIndices;
       
        s_Data.CubeVertexArray = VertexArray::Create();
        s_Data.CubeVertexArray->AddVertexBuffer(s_Data.CubeVertexBuffer);
        s_Data.CubeVertexArray->SetIndexBuffer(s_Data.CubeIndexBuffer);

        s_Data.PhongShader = Shader::Create("assets/shaders/phong.vert", "assets/shaders/phong.frag");
        s_Data.PhongShader->Bind();
        s_Data.PhongShader->SetInt("u_Material.DiffuseMap", 0);
        s_Data.PhongShader->SetInt("u_Material.SpecularMap", 1);
    }

    void Renderer::Shutdown()
    {
    }

    void Renderer::BeginScene(const Camera& camera)
    {
        s_SceneData->ViewProjectionMatrix = camera.GetViewProjection();
    }

    void Renderer::BeginScene(const glm::mat4& viewProjection)
    {
        s_SceneData->ViewProjectionMatrix = viewProjection;
    }

    void Renderer::EndScene()
    {

    }

    void Renderer::Submit(const Ref<VertexArray>& vertexArray, const glm::mat4& transform, const Material& material, RendererAPI::DrawMode mode)
    {
        bool textured = material.DiffuseMap.get();
        s_Data.PhongShader->Bind();
        s_Data.PhongShader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        s_Data.PhongShader->SetMat4("u_Model", transform);
        if (textured)
            material.DiffuseMap->Bind(0);
        s_Data.PhongShader->SetBool("u_Material.Textured", textured);
        //material.SpecularMap->Bind(1);  // @todo
        s_Data.PhongShader->SetVec3("u_Material.BaseColor", material.BaseColor);
        s_Data.PhongShader->SetFloat("u_Material.Shininess", material.Shininess);
        s_Data.PhongShader->SetFloat("u_Material.Transparency", material.Transparency);

        RenderCommand::DrawIndexed(vertexArray, mode);
    }

    void Renderer::DrawCube(const glm::mat4& transform, const Material& material, RendererAPI::DrawMode mode)
    {
        Submit(s_Data.CubeVertexArray, transform, material, mode);
    }

}