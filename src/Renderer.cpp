#include "rapch.h"
#include "Renderer.hpp"
//#include "RenderCommand.hpp"
#include "RendererApi.hpp"
#include "Buffer.hpp"
#include "VertexArray.hpp"
#include "Framebuffer.hpp"
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

    Ra::Renderer::Renderer3DData Renderer::s_RendererData{};

    Ra::Renderer3DStorage Renderer::Storage{};

    void Renderer::Init()
    {
        PROFILER_SCOPE("Renderer::Init()");
        std::uint8_t nullTexData[] = { 255,255,255,0 };
        Texture::NullTexture = Texture::Create(nullTexData, 1, 1, 4);


        //if (s_RendererAPI != RendererAPI::API::None)
        //    RenderCommand::Init();

        Storage.BlinnPhongShader = Shader::Create(GetShadersDir() + "blinn-phong.vert", GetShadersDir() + "blinn-phong.frag");
        Storage.SkyboxShader = Shader::Create(GetShadersDir() + "skybox.vert", GetShadersDir() + "skybox.frag");
        Storage.DepthShader = Shader::Create(GetShadersDir() + "depth.vert", GetShadersDir() + "depth.frag");

        Storage.VectorMesh = Mesh::Create(GetMeshesDir() + "Vector.fbx");

        FramebufferProperties shadowMapProps;
        shadowMapProps.Width = 8192;
        shadowMapProps.Height = 8192;
        shadowMapProps.Samples = 1;
        shadowMapProps.Attachments = { TextureFormat::DepthComponent };
        s_RendererData.ShadowMap = Framebuffer::Create(std::move(shadowMapProps));

        FramebufferProperties postprocessingBufferProps;
        postprocessingBufferProps.Width = 1920; //default
        postprocessingBufferProps.Height = 1080;
        postprocessingBufferProps.Samples = 1;
        postprocessingBufferProps.Attachments = { TextureFormat::ColorLinear, Ra::TextureFormat::DepthStencil };
        s_RendererData.PostprocessingBuffer = Framebuffer::Create(std::move(postprocessingBufferProps));
    }

    void Renderer::Shutdown()
    {
    }

    void Renderer::DrawSkybox(const Skybox& skybox)
    {
        RendererAPI::GetInstance().SetDepthFunc(RendererAPI::DepthFunc::Lequal);

        Storage.SkyboxShader->Bind();
        Storage.SkyboxShader->SetMat4("u_ViewProjection", s_SceneData->ProjMatrix * glm::mat4(glm::mat3(s_SceneData->ViewMatrix)));
        skybox.Bind();

        skybox.DrawBoundsCube();

        RendererAPI::GetInstance().SetDepthFunc(RendererAPI::DepthFunc::Less);
    }

    void Renderer::ResizeViewport(const glm::vec2& size)
    {
        s_RendererData.PostprocessingBuffer->Resize((std::uint32_t)size.x, (std::uint32_t)size.y);
    }

    glm::vec2 Renderer::GetViewportSize()
    {
        return { s_RendererData.PostprocessingBuffer->GetProperties().Width, s_RendererData.PostprocessingBuffer->GetProperties().Height };
    }

    Ra::RendererId Renderer::GetResultTextureHandle()
    {
        return s_RendererData.PostprocessingBuffer->GetDrawTextureHandle();
        //return s_RendererData.ShadowMap->GetDepthTextureHandle();
    }

    void Renderer::BeginScene(const glm::mat4& viewMatrix, const glm::mat4& projMatrix, const glm::vec3& cameraPosition, const Skybox& skybox)
    {
        s_SceneData->ViewMatrix = viewMatrix;
        s_SceneData->ProjMatrix = projMatrix;
        s_SceneData->CameraPosition = cameraPosition;
        s_SceneData->SubmittedPointLights = 0;
        s_SceneData->SubmittedDirLights = 0;
        s_SceneData->SkyboxObject = skybox;
        s_SceneData->RenderQueue.clear(); // no shrinking
        s_Stats = {};
    }

    void Renderer::EndScene()
    {

        // Shadow pass
        //RendererAPI::GetInstance().SetCullFace(RendererAPI::CullFace::Front);
        s_RendererData.ShadowMap->StartWriting();
        for (const auto& func : s_SceneData->RenderQueue)
            func(Storage.DepthShader);
        s_RendererData.ShadowMap->StopWriting();
        //RendererAPI::GetInstance().SetCullFace(RendererAPI::CullFace::Back);

        s_SceneData->ViewProjectionMatrix = s_SceneData->ProjMatrix * s_SceneData->ViewMatrix;

        // Rendering pass 
        s_RendererData.PostprocessingBuffer->StartWriting();

        DrawSkybox(s_SceneData->SkyboxObject);
        for (const auto& func : s_SceneData->RenderQueue)
            func(Storage.BlinnPhongShader);

        s_RendererData.PostprocessingBuffer->StopWriting();

        s_Stats.ScenesPerSecond = 1000000.0f / s_Stats.ScenesPerSecondSW.Elapsed();
    }

    void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Transform& transform, Ref<Shader>& shader, RendererAPI::DrawMode mode /*= RendererAPI::DrawMode::Triangles*/)
    {

        PROFILER_SCOPE("Renderer::Submit( VertexArray )");
        shader->Bind();
        shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        shader->SetMat4("u_LightSpaceMatrix", s_SceneData->LightSpaceMatrix);
        shader->SetMat4("u_Model", transform.Model);
        shader->SetMat3("u_NormalModel", transform.Normal);
        s_RendererData.ShadowMap->BindDepthTexture();
        shader->SetInt("u_ShadowMap", s_SceneData->LastUsedTextureUnit);
        RA_ASSERT(s_SceneData->SubmittedPointLights <= 128, "Too many point lights!");
        shader->SetInt("u_PointLightsCount", s_SceneData->SubmittedPointLights);
        shader->SetInt("u_DirLightsCount", s_SceneData->SubmittedDirLights);
        shader->SetVec3("u_CameraPosition", s_SceneData->CameraPosition);
        s_SceneData->SkyboxObject.Bind(s_SceneData->LastUsedTextureUnit + 1);
        shader->SetInt("u_Environment", s_SceneData->LastUsedTextureUnit);

        RendererAPI::GetInstance().DrawIndexed(vertexArray, mode);
        s_Stats.DrawCalls += 1;
        s_Stats.Indices += vertexArray->GetIndexBufer()->GetCount();

    }

    void Renderer::Submit(const Ref<Mesh>& mesh, const Transform& transform, RendererAPI::DrawMode mode /*= RendererAPI::DrawMode::Triangles*/)
    {
        PROFILER_SCOPE("Renderer::Submit( Mesh )  -  " + mesh->GetPath());
        s_SceneData->RenderQueue.push_back([mesh, transform, mode](Ref<Shader>& shader)
            {
                auto& subMeshes = mesh->GetSubMeshes();
                for (auto& it : subMeshes)
                {
                    PROFILER_SCOPE("Renderer: submesh iteration");
                    it.GetMaterial().LoadTo(shader);
                    Submit(it.GetVertexArray(), transform, shader, mode);
                }
            }
        );
    }

    void Renderer::DrawVector(const glm::vec3& position, const glm::vec3& direction)
    {
        glm::mat4 trans;
        glm::mat4 translation{glm::translate(glm::mat4{1.f}, position)};
        auto q = glm::quatLookAt(glm::normalize(direction), { 0,1,0 });
        trans = translation * glm::toMat4(q);

        Transform transform{ trans };
        Submit(Storage.VectorMesh, transform);
    }

    //void Renderer::DrawCube(const Transform& transform, const Material& material, RendererAPI::DrawMode mode)
    //{
    //    Submit(Storage.CubeVertexArray, transform, material, mode);
    //}

    void Renderer::SubmitLight(const PointLight& light, const glm::vec3& position)
    {
        std::string uniformLightToken = "u_PointLights[" + std::to_string(s_SceneData->SubmittedPointLights++) + "].";
        Storage.BlinnPhongShader->Bind();
        Storage.BlinnPhongShader->SetVec3(uniformLightToken + "Position", position);
        Storage.BlinnPhongShader->SetVec3(uniformLightToken + "Color", light.Color);
        Storage.BlinnPhongShader->SetFloat(uniformLightToken + "Intensity", light.Intensity);
    }

    void Renderer::SubmitLight(const DirLight& light)
    {
        RA_ASSERT(s_SceneData->SubmittedDirLights == 0, "Multiple dir lights not supported!")
            std::string uniformLightToken = "u_DirLight.";
        Storage.BlinnPhongShader->Bind();
        Storage.BlinnPhongShader->SetVec3(uniformLightToken + "Direction", light.Direction);
        Storage.BlinnPhongShader->SetVec3(uniformLightToken + "Color", light.Color);
        Storage.BlinnPhongShader->SetFloat(uniformLightToken + "Intensity", light.Intensity);
        s_SceneData->SubmittedDirLights++;

        if (s_SceneData->DirLightObject.Direction != light.Direction)
        {
            s_SceneData->DirLightObject = light;
            auto lightSpaceView = glm::lookAt(
                -200.f * s_SceneData->DirLightObject.Direction,
                s_SceneData->DirLightObject.Direction,
                { 0.f, 1.f, 0.f });
            auto lightSpaceProj = glm::ortho(-200.f, 200.f, -200.f, 200.f, 0.1f, 1000.f);

            s_SceneData->LightSpaceMatrix = lightSpaceProj * lightSpaceView;
        }

    }

    void Renderer::SetLastTextureUnit(std::uint32_t unit)
    {
        s_SceneData->LastUsedTextureUnit = unit;
    }

    std::uint8_t Renderer::GetLastTextureUnit()
    {
        return s_SceneData->LastUsedTextureUnit;
    }

    Ra::RendererStats Renderer::GetStats()
    {
        return s_Stats;
    }

}