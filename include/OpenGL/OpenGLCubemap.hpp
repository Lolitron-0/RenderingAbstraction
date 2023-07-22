#pragma once
#include "Cubemap.hpp"

namespace Ra
{
    class OpenGLCubemap : public Cubemap
    {
    public:
        OpenGLCubemap();

        void Bind(std::uint32_t unit = 0) override;
        void Unbind() override;

    protected:
        void LoadFromSweep_(const std::string& path) override;
        void LoadFromFiles_(const std::vector<std::string>& paths) override;

    private:
        RendererId m_Handle;
    };
}