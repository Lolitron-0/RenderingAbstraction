#pragma once
namespace Ra
{
    class Cubemap;
    class VertexArray;

    class Skybox
    {
    public:
        Skybox() = default;
        Skybox(const std::string& sweepPath);
        Skybox(const std::vector<std::string>& paths);

        void Bind(std::uint32_t unit = 0) const;
        void Unbind() const;

        void DrawBoundsCube() const;

    private:
        void SetupBox_();

        Ref<Cubemap> m_Cubemap{ nullptr };
        static Ref<VertexArray> s_SkyboxVAO;
        static const float s_SkyboxVertices[3 * 36];
    };
}
