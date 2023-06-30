#pragma once
#include "RendererAPI.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include <glm/glm.hpp>

namespace Ra
{
    class Renderer
    {
    public:

        static void Init();
        static void Shutdown();

        static RendererAPI::API GetAPI() { return s_RendererAPI; }
        static void SetAPI(RendererAPI::API api)
        {
            RA_ASSERT(s_RendererAPI == RendererAPI::API::None, "Rendering API can only be set once!");
            s_RendererAPI = api;
        }

        static void BeginScene(const Camera& camera);
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

        static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader);

    private:
        static RendererAPI::API s_RendererAPI;

        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };

        static Scope<SceneData> s_SceneData;
    };
}