#include "rapch.h"
#include "Cubemap.hpp"
#include "Renderer.hpp"
#include "ResourceManager.hpp"
#include "OpenGL/OpenGLCubemap.hpp"
#include <Profiler.hpp>

namespace Ra
{

    std::string Cubemap::GetPath() const
    {
        return m_Path;
    }

    Ref<Cubemap> Cubemap::Create(const std::string& sweepPath)
    {
        PROFILER_SCOPE("Cubemap::Create(sweep)");
        Ref<Cubemap> ret;
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL: ret = MakeRef<OpenGLCubemap>(); break;
        default: ret = nullptr; break;
        }

        if (ResourceManager::TryFindAlreadyLoaded<Cubemap>(ret, sweepPath))
            return ret;

        {
            PROFILER_SCOPE("Cubemap: creating from sweep: " + sweepPath);
            ret->LoadFromSweep_(sweepPath);
        }
        return ret;
    }

    Ref<Cubemap> Cubemap::Create(const std::vector<std::string>& paths)
    {
        PROFILER_SCOPE("Cubemap::Create(files)");
        Ref<Cubemap> ret;
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL: ret = MakeRef<OpenGLCubemap>(); break;
        default: ret = nullptr; break;
        }

        if (ResourceManager::TryFindAlreadyLoaded<Cubemap>(ret, paths[0]))
            return ret;

        {
            PROFILER_SCOPE("Cubemap: creating from files: " + paths[0] + "...");
            ret->LoadFromFiles_(paths);
        }
        return ret;
    }

}
