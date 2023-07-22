#include "rapch.h"
#include "Texture.hpp"
#include "Renderer.hpp"
#include "OpenGL/OpenGLTexture.hpp"
#include "ResourceManager.hpp"
#include <Profiler.hpp>

namespace Ra
{

    Ref<Texture> Texture::Create(const std::string& path, TextureFormat format, TextureType type)
    {
        PROFILER_SCOPE("Texture::Create(file)")
        Ref<Texture> ret;
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL: ret = MakeRef<OpenGLTexture>(); break;
        default: ret = nullptr; break;
        }
        
        if (ResourceManager::TryFindAlreadyLoaded<Texture>(ret, path))
            return ret;

        {
            PROFILER_SCOPE("Texture: creating from file " + path)
            ret->LoadFromFile_(path, format, type);
        }
        return ret;
    }

    Ref<Texture> Texture::Create(std::uint8_t* rawData, std::uint32_t width, std::uint32_t height, std::uint32_t channels, TextureFormat format /*= TextureFormat::Color*/, TextureType type /*= TextureType::Diffuse*/)
    {
        Ref<Texture> ret;
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL: ret = MakeRef<OpenGLTexture>(); break;
        default: ret = nullptr; break;
        }
        {
            PROFILER_SCOPE("Texture: creating from data")
            ret->LoadFromData_(rawData, width, height, channels, format, type);
        }
        return ret;
    }

    Ref<Texture> Texture::NullTexture = nullptr;

}