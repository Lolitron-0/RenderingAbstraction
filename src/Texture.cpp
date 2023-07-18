#include "rapch.h"
#include "Texture.hpp"
#include "Renderer.hpp"
#include "OpenGL/OpenGLTexture.hpp"

namespace Ra
{

    Ref<Texture> Texture::Create(const std::string& path, TextureFormat format, TextureType type)
    {
        Ref<Texture> ret;
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL: ret = MakeScope<OpenGLTexture>(); break;
        default: ret = nullptr; break;
        }
        
        decltype(Renderer::LoadedTextures)::iterator sameIt;
        if ((sameIt = std::find_if(
            Renderer::LoadedTextures.begin(),
            Renderer::LoadedTextures.end(),
            [&path](const Ref<Texture>& tex) { return tex->GetSource() == path; }))
            != Renderer::LoadedTextures.end())
            return *sameIt;

        ret->LoadFromFile_(path, format, type);
        Renderer::LoadedTextures.push_back(ret);
        return ret;
    }

    Ref<Texture> Texture::Create(std::uint8_t* rawData, std::uint32_t width, std::uint32_t height, std::uint32_t channels, TextureFormat format /*= TextureFormat::Color*/, TextureType type /*= TextureType::Diffuse*/)
    {
        Ref<Texture> ret;
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL: ret = MakeScope<OpenGLTexture>(); break;
        default: ret = nullptr; break;
        }
        ret->LoadFromData_(rawData, width, height, channels, format, type);

        return ret;
    }

    Ref<Texture> Texture::NullTexture = nullptr;

}