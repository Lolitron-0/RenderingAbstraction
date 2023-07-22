#pragma once
namespace Ra
{
    class Cubemap
    {
    public:
        virtual void Bind(std::uint32_t unit = 0) = 0;
        virtual void Unbind() = 0;

        std::string GetPath() const;

        static Ref<Cubemap> Create(const std::string& sweepPath);
        static Ref<Cubemap> Create(const std::vector<std::string>& paths);
    protected:
        virtual void LoadFromSweep_(const std::string& path) = 0;
        virtual void LoadFromFiles_(const std::vector<std::string>& paths) = 0;
    protected:
        std::string m_Path;
    };
}
