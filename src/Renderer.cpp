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

    Ra::Renderer3DData Renderer::Storage;

    void Renderer::Init()
    {
        Texture::NullTexture = Texture::Create();
        std::uint8_t nullTexData[] = { 255,255,255,0 };
        Texture::NullTexture->Load(nullTexData, 1, 1, 4);


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
        
        Storage.CubeVertexBuffer = VertexBuffer::Create(cubeData, sizeof(cubeData));
        Storage.CubeVertexBuffer->SetLayout({
            {BufferDataType::Float3, "position"},
            {BufferDataType::Float3, "normals"},
            {BufferDataType::Float2, "texCoords"},
            {BufferDataType::Float3, "color"},
            });

        std::uint32_t* cubeIndices = new std::uint32_t[36];
        for (int i = 0; i < 36; i++) cubeIndices[i] = i;
        Storage.CubeIndexBuffer = IndexBuffer::Create(cubeIndices, 36);
        delete[] cubeIndices;
       
        Storage.CubeVertexArray = VertexArray::Create();
        Storage.CubeVertexArray->AddVertexBuffer(Storage.CubeVertexBuffer);
        Storage.CubeVertexArray->SetIndexBuffer(Storage.CubeIndexBuffer);

        Storage.PhongShader = Shader::Create(GetShadersDir()+"phong.vert", GetShadersDir()+"phong.frag");
        Storage.PhongShader->Bind();
        Storage.PhongShader->SetInt("u_Material.DiffuseMap", 0);
        Storage.PhongShader->SetInt("u_Material.SpecularMap", 1);
    }

    void Renderer::Shutdown()
    {
    }

    void Renderer::BeginScene(const Camera& camera)
    {
        s_SceneData->ViewProjectionMatrix = camera.GetViewProjection();
    }

    void Renderer::BeginScene(const glm::mat4& viewProjection, const glm::vec3& cameraPosition)
    {
        s_SceneData->ViewProjectionMatrix = viewProjection;
        s_SceneData->CameraPosition = cameraPosition;
        s_SceneData->SubmittedLights = 0;
    }

    void Renderer::EndScene()
    {

    }

    void Renderer::Submit(const Ref<VertexArray>& vertexArray, const glm::mat4& transform, const Material& material, RendererAPI::DrawMode mode)
    {
        Storage.PhongShader->Bind();
        Storage.PhongShader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        Storage.PhongShader->SetMat4("u_Model", transform);
        Storage.PhongShader->SetMat3("u_NormalModel", glm::mat3(glm::transpose(glm::inverse(transform))));
        material.DiffuseMap->Bind(0);
        material.SpecularMap->Bind(1); 
        Storage.PhongShader->SetVec3("u_Material.BaseColor", material.BaseColor);
        Storage.PhongShader->SetFloat("u_Material.Shininess", material.Shininess);
        Storage.PhongShader->SetFloat("u_Material.Transparency", material.Transparency);
        RA_ASSERT(s_SceneData->SubmittedLights <= 128, "Too many point lights!");
        Storage.PhongShader->SetInt("u_PointLightsCount", s_SceneData->SubmittedLights);
        Storage.PhongShader->SetVec3("u_CameraPosition", s_SceneData->CameraPosition);

        RenderCommand::DrawIndexed(vertexArray, mode);
    }

    void Renderer::DrawCube(const glm::mat4& transform, const Material& material, RendererAPI::DrawMode mode)
    {
        Submit(Storage.CubeVertexArray, transform, material, mode);
    }

    void Renderer::SubmitPointLight(const PointLight& light, const glm::vec3& position)
    {
        std::string uniformLightToken = "u_PointLights[" + std::to_string(s_SceneData->SubmittedLights++) + "].";
        Storage.PhongShader->Bind();
        Storage.PhongShader->SetVec3(uniformLightToken + "Position", position);
        Storage.PhongShader->SetVec3(uniformLightToken + "Color", light.Color);
        Storage.PhongShader->SetFloat(uniformLightToken + "Intensity", light.Intensity);
    }

}