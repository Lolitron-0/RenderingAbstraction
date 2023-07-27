#pragma once
#include "RendererAPI.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include "ShaderUserData.hpp"
#include "Stopwatch.hpp"
#include "Lights.hpp"
#include "Skybox.hpp"
#include <glm/glm.hpp>

namespace Ra
{
    class VertexBuffer;
    class VertexArray;
    class IndexBuffer;
    class Shader;
    class Mesh;
    class Framebuffer;

    struct Renderer3DStorage
    {
        Ref<Mesh> VectorMesh;
        Ref<Shader> BlinnPhongShader;
        Ref<Shader> SkyboxShader;
        Ref<Shader> DepthShader;

        Material DebugMaterial;
    };

    struct RendererStats
    {
        int DrawCalls{ 0 };
        std::size_t Indices{ 0 };
        Stopwatch ScenesPerSecondSW{};
        float ScenesPerSecond{ -1.f };
    };

    /// High level rendering commands (Scene level)
    class Renderer
    {
    public:
        struct Transform
        {
            Transform(const glm::mat4& model)
                :Model(model), Normal(glm::transpose(glm::inverse(model)))
            {}
            Transform(const glm::mat4& model, const glm::mat4& normal)
                :Model(model), Normal(normal)
            {}
            glm::mat4 Model;
            glm::mat4 Normal;
        };

        static void Init();
        static void Shutdown();

        static RendererAPI::API GetAPI() { return s_RendererAPI; }

        /// Sets current rendering API. Can only be set once
        static void SetAPI(RendererAPI::API api)
        {
            RA_ASSERT(s_RendererAPI == RendererAPI::API::None, "Rendering API can only be set once!");
            s_RendererAPI = api;
        }

        static void ResizeViewport(const glm::vec2& size);
        static glm::vec2 GetViewportSize();
        static RendererId GetResultTextureHandle();

        /// Initializes scene with given view-projection matrix
        static void BeginScene(const glm::mat4& viewMatrix, const glm::mat4& projMatrix, const glm::vec3& cameraPosition, const Skybox& skybox);
        /// Marks scene as finished
        static void EndScene();

        //template<class F, class... Args>
        //static void Submit(const std::function<void(void)>& onEnd, const F& call, Args&&... args)
        //{
        //    s_RenderThreadMutex.lock();
        //    s_RenderQueue.push([onEnd, &call, &args...]() {
        //        call(std::forward<Args>(args)...);
        //        onEnd();

        //        });
        //    s_RenderThreadMutex.unlock();
        //}

        /**
         * @brief Submits draw command to a renderer
         * @param VertexArray vertex array to draw
         * @param material Material to use for drawing
         * @param mode Drawing mode (e.g. triangles, lines, points, etc.)
        */
        static void Submit(const Ref<VertexArray>& vertexArray, const Transform& transform, Ref<Shader>& shader, RendererAPI::DrawMode mode = RendererAPI::DrawMode::Triangles);

        static void Submit(const Ref<Mesh>& mesh, const Transform& transform, RendererAPI::DrawMode mode = RendererAPI::DrawMode::Triangles);

        static void DrawVector(const glm::vec3& position, const glm::vec3& direction);

        static void SubmitLight(const PointLight& light, const glm::vec3& position);

        static void SubmitLight(const DirLight& light);

        /// Sets last used texture unit
        static void SetLastTextureUnit(std::uint32_t unit);

        static std::uint8_t GetLastTextureUnit();

        static RendererStats GetStats();

        static Renderer3DStorage Storage;

    private:
        static void DrawSkybox(const Skybox& skybox);

    private:
        struct SceneData
        {
            glm::mat4 ViewMatrix;
            glm::mat4 ProjMatrix;
            glm::mat4 ViewProjectionMatrix;
            glm::mat4 LightSpaceMatrix{1.f};
            glm::vec3 CameraPosition;
            std::uint16_t SubmittedPointLights{ 0 };
            std::uint16_t SubmittedDirLights{ 0 };
            std::uint8_t LastUsedTextureUnit{ 0 };
            Skybox SkyboxObject;
            DirLight DirLightObject;
            std::vector<std::function<void(Ref<Shader>)>> RenderQueue;
        };

        struct Renderer3DData
        {
            Ref<Framebuffer> ShadowMap;
            Ref<Framebuffer> PostprocessingBuffer;
        };

        static RendererStats s_Stats;

        static RendererAPI::API s_RendererAPI;

        static Scope<SceneData> s_SceneData;
        static Renderer3DData s_RendererData;
    };
}
