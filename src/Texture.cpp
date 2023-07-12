#include "rapch.h"
#include "Texture.hpp"
#include "Renderer.hpp"
#include "OpenGL/OpenGLTexture.hpp"

namespace Ra
{

    Scope<Texture> Texture::Create()
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL: return MakeScope<OpenGLTexture>();
        default: return nullptr;
        }
    }

}