#include "rapch.h"
#include "Renderer.hpp"
#include "RenderCommand.hpp"
#include "Buffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include <Profiler.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Ra
{
    Ra::RendererStats Renderer::s_Stats{};

    RendererAPI::API Renderer::s_RendererAPI = RendererAPI::API::None;

    Scope<Ra::Renderer::SceneData> Renderer::s_SceneData = std::make_unique<Renderer::SceneData>();

    Ra::Renderer3DData Renderer::Storage{};

    void Renderer::Init()
    {
        PROFILER_SCOPE("Renderer::Init()");
        std::uint8_t nullTexData[] = { 255,255,255,0 };
        Texture::NullTexture = Texture::Create(nullTexData, 1, 1, 4);


        if (s_RendererAPI != RendererAPI::API::None)
            RenderCommand::Init();

        Storage.PhongShader = Shader::Create(GetShadersDir()+"phong.vert", GetShadersDir()+"phong.frag");
        Storage.PhongShader->Bind();
        Storage.PhongShader->SetInt("u_Material.DiffuseMap", 0);
        Storage.PhongShader->SetInt("u_Material.SpecularMap", 1);

        Storage.VectorMesh = Mesh::Create(GetMeshesDir() + "Vector.fbx");
        Storage.VectorMesh->ForEashSubmesh([](auto& subMesh)
            {
                subMesh.GetMaterial().SkipLight = true;
            });
    }

    void Renderer::Shutdown()
    {
    }

    void Renderer::BeginScene(const Camera& camera)
    {
        s_SceneData->ViewProjectionMatrix = camera.GetViewProjection();
        s_Stats = {};
    }

    void Renderer::BeginScene(const glm::mat4& viewProjection, const glm::vec3& cameraPosition)
    {
        s_SceneData->ViewProjectionMatrix = viewProjection;
        s_SceneData->CameraPosition = cameraPosition;
        s_SceneData->SubmittedPointLights = 0;
        s_SceneData->SubmittedDirLights = 0;
        s_Stats = {};
    }

    void Renderer::EndScene()
    {
        s_Stats.ScenesPerSecond = 1000000.0f / s_Stats.ScenesPerSecondSW.Elapsed();
    }

    void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Transform& transform, RendererAPI::DrawMode mode /*= RendererAPI::DrawMode::Triangles*/)
    {
        PROFILER_SCOPE("Renderer::Submit( VertexArray )");
        Storage.PhongShader->Bind();
        Storage.PhongShader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        Storage.PhongShader->SetMat4("u_Model", transform.Model);
        Storage.PhongShader->SetMat3("u_NormalModel", transform.Normal);
        RA_ASSERT(s_SceneData->SubmittedPointLights <= 128, "Too many point lights!");
        Storage.PhongShader->SetInt("u_PointLightsCount", s_SceneData->SubmittedPointLights);
        RA_ASSERT(s_SceneData->SubmittedDirLights <= 1, "Too many dir lights!");
        Storage.PhongShader->SetInt("u_DirLightsCount", s_SceneData->SubmittedDirLights);
        Storage.PhongShader->SetVec3("u_CameraPosition", s_SceneData->CameraPosition);

        RenderCommand::DrawIndexed(vertexArray, mode);
        s_Stats.DrawCalls += 1;
        s_Stats.Indices += vertexArray->GetIndexBufer()->GetCount();
    }

    void Renderer::Submit(const Ref<Mesh>& mesh, const Transform& transform, RendererAPI::DrawMode mode /*= RendererAPI::DrawMode::Triangles*/)
    {
        PROFILER_SCOPE("Renderer::Submit( Mesh )  -  " + mesh->GetPath());
        auto& subMeshes = mesh->GetSubMeshes();
        for (auto& it : subMeshes)
        {
            PROFILER_SCOPE("Renderer: submesh iteration")
            it.GetMaterial().LoadTo(Storage.PhongShader);
            Submit(it.GetVertexArray(), transform, mode);
        }
    }

    void Renderer::DrawVector(const glm::vec3& position, const glm::vec3& direction)
    {
        glm::mat4 trans;
        glm::mat4 translation{glm::translate(glm::mat4{1.f}, position)};
        auto q = glm::quatLookAt(glm::normalize(direction), { 0,1,0 });
        trans = translation * glm::toMat4(q);

        Transform transform{trans};
        Submit(Storage.VectorMesh, transform);
    }

    //void Renderer::DrawCube(const Transform& transform, const Material& material, RendererAPI::DrawMode mode)
    //{
    //    Submit(Storage.CubeVertexArray, transform, material, mode);
    //}

    void Renderer::SubmitLight(const PointLight& light, const glm::vec3& position)
    {
        std::string uniformLightToken = "u_PointLights[" + std::to_string(s_SceneData->SubmittedPointLights++) + "].";
        Storage.PhongShader->Bind();
        Storage.PhongShader->SetVec3(uniformLightToken + "Position", position);
        Storage.PhongShader->SetVec3(uniformLightToken + "Color", light.Color);
        Storage.PhongShader->SetFloat(uniformLightToken + "Intensity", light.Intensity);
    }

    void Renderer::SubmitLight(const DirLight& light)
    {
        std::string uniformLightToken = "u_DirLight.";
        Storage.PhongShader->Bind();
        Storage.PhongShader->SetVec3(uniformLightToken + "Direction", light.Direction);
        Storage.PhongShader->SetVec3(uniformLightToken + "Color", light.Color);
        Storage.PhongShader->SetFloat(uniformLightToken + "Intensity", light.Intensity);
        s_SceneData->SubmittedDirLights++;
    }

    Ra::RendererStats Renderer::GetStats()
    {
        return s_Stats;
    }

}