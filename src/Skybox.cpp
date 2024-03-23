#include "rapch.h"
#include "Skybox.hpp"
#include "Cubemap.hpp"
#include "Buffer.hpp"
#include "VertexArray.hpp"
#include "RendererAPI.hpp"

namespace Ra
{

    Skybox::Skybox(const std::string& sweepPath)
    {
        m_Cubemap = Cubemap::Create(sweepPath);
        if (!s_SkyboxVAO)
            SetupBox_();
    }

    Skybox::Skybox(const std::vector<std::string>& paths)
    {
        m_Cubemap = Cubemap::Create(paths);
        if (!s_SkyboxVAO)
            SetupBox_();
    }


    void Skybox::Bind(std::uint32_t unit) const
    {
        m_Cubemap->Bind(unit);
        s_SkyboxVAO->Bind();
    }

    void Skybox::Unbind() const
    {
        m_Cubemap->Unbind();
        s_SkyboxVAO->Unbind();
    }

    void Skybox::DrawBoundsCube() const
    {
        RendererAPI::GetInstance().DrawIndexed(s_SkyboxVAO, RendererAPI::DrawMode::Triangles);
    }

    void Skybox::SetupBox_()
    {
        Ref<VertexBuffer> vbo = VertexBuffer::Create(s_SkyboxVertices, sizeof(s_SkyboxVertices));
        vbo->SetLayout({
            {BufferDataType::Float3, "Position"},
            });

        s_SkyboxVAO = VertexArray::Create();
        s_SkyboxVAO->AddVertexBuffer(vbo);
        s_SkyboxVAO->SetDefaultIndexBuffer(36);
    }

    Ref<VertexArray> Skybox::s_SkyboxVAO{ nullptr };

    const float Skybox::s_SkyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

}
