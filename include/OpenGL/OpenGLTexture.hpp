#pragma once
#include "Texture.hpp"
#include <glad/glad.h>

namespace Ra
{
    class OpenGLTexture : public Texture
    {
    public:
        OpenGLTexture();
        ~OpenGLTexture();

        void Bind() override;
        void Bind(int id) override;
        void Unbind() override;

        void SetMinFilter(TextureFilter filter) override;
        void SetMagFilter(TextureFilter filter) override;
        void SetWrap(TextureWrap wrap) override;


        RendererId GetNativeTerxtureHandle() const override;
    private:
        void LoadFromFile_(const std::string& path, TextureFormat format = TextureFormat::Color, TextureType type = TextureType::Diffuse) override;
        void LoadFromData_(std::uint8_t* rawData, std::uint32_t width, std::uint32_t height, std::uint32_t channels, TextureFormat format = TextureFormat::Color, TextureType type = TextureType::Diffuse) override;

    private:
        RendererId m_Handle;
        GLenum m_Target;
    };

}
