#pragma once
#include "RendererAPI.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include "ShaderUserData.hpp"
#include <glm/glm.hpp>

namespace Ra
{
    class VertexBuffer;
    class VertexArray;
    class IndexBuffer;
    class Shader;

    struct Renderer3DData
    {
        Ref<VertexBuffer>   CubeVertexBuffer;
        Ref<IndexBuffer>    CubeIndexBuffer;
        Ref<VertexArray>    CubeVertexArray;

        Ref<Shader> PhongShader;

        Material DebugMaterial;
    };

    struct RendererStats
    {
        int DrawCalls{ 0 };
        int Indices{ 0 };
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

        /// Initializes scene with given camera
        static void BeginScene(const Camera& camera);
        /// Initializes scene with given view-projection matrix (for custom camera systems)
        static void BeginScene(const glm::mat4& viewProjection, const glm::vec3& cameraPosition);
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
        static void Submit(const Ref<VertexArray>& vertexArray, const Transform& transform, const Material& material, RendererAPI::DrawMode mode = RendererAPI::DrawMode::Triangles);

        //static void DrawCube(const Transform& transform, const Material& material, RendererAPI::DrawMode mode = RendererAPI::DrawMode::Triangles);
        static void SubmitPointLight(const PointLight& light, const glm::vec3& position);

        static RendererStats GetStats();

        static Renderer3DData Storage;

    private:
        static RendererStats s_Stats;

        static RendererAPI::API s_RendererAPI;

        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
            glm::vec3 CameraPosition;
            int SubmittedLights{ 0 };
        };

        static Scope<SceneData> s_SceneData;
    };
}
