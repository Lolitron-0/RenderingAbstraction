#include "OpenGL/OpenGLTexture.hpp"
#include "rapch.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Renderer3D.hpp"
#include <stb_image/stb_image.h>

namespace Ra
{
static GLenum g_TextureFormatTable[] = {
    GL_R32I,
    GL_R16,
    GL_RG,
    GL_RG16,
    GL_R16F,
    GL_R32F,
    GL_RG16F,
    GL_RG32F,
    GL_RGB8,
    GL_RGBA8,
    GL_SRGB8,
    GL_SRGB8_ALPHA8,
    GL_RGB16,
    GL_RGB16F,
    GL_RGBA16,
    GL_RGBA16F,
    GL_RGB32F,
    GL_RGBA32F,
    GL_DEPTH24_STENCIL8,
    GL_DEPTH_COMPONENT,
};

static GLenum g_TextureFilterTable[] = { GL_NEAREST, GL_LINEAR };

static GLenum g_TextureWrapTable[] = { GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER,
                                       GL_MIRRORED_REPEAT, GL_REPEAT };

OpenGLTexture::OpenGLTexture()
    : Texture()
{
    m_Target = GL_TEXTURE_2D;
    glCreateTextures(m_Target, 1, &m_Handle);
}
OpenGLTexture::~OpenGLTexture()
{
    glDeleteTextures(1, &m_Handle);
}

void OpenGLTexture::LoadFromFile_(
    const std::string& path, TextureFormat format /*= TextureFormat::Color*/,
    TextureType type)
{
    this->m_Format = format;
    m_Path = path;

    Bind();
    int tWidth, tHeight, tNumChannels;
    std::uint8_t* tData =
        stbi_load(path.c_str(), &tWidth, &tHeight, &tNumChannels, 0);

    RA_ASSERT(tData, stbi_failure_reason());
    LoadFromData_(tData, tWidth, tHeight, tNumChannels, format);

    stbi_image_free(tData);
}

void OpenGLTexture::LoadFromData_(
    std::uint8_t* rawData, std::uint32_t width, std::uint32_t height,
    std::uint32_t channels, TextureFormat format /*= TextureFormat::Color*/,
    TextureType type)
{
    m_Type = type;
    Bind();
    this->m_Width = width;
    this->m_Height = height;

    GLenum dataChannels = GL_NONE;
    switch (channels)
    {
    case 1:
        dataChannels = GL_RED;
        break;
    case 2:
        dataChannels = GL_RG;
        break;
    case 3:
        dataChannels = GL_RGB;
        break;
    case 4:
        dataChannels = GL_RGBA;
        break;
    }

    RA_ASSERT(dataChannels != GL_NONE, "Unsupported number of channels!");

    glTexImage2D(m_Target, 0, g_TextureFormatTable[(int)this->m_Format],
                 (GLsizei)width, (GLsizei)height, 0, dataChannels,
                 GL_UNSIGNED_BYTE, rawData);
    glTexParameteri(m_Target, GL_TEXTURE_MIN_FILTER,
                    g_TextureFilterTable[(int)this->m_MinFilter]);
    glTexParameteri(m_Target, GL_TEXTURE_MAG_FILTER,
                    g_TextureFilterTable[(int)this->m_MagFilter]);
    glTexParameteri(m_Target, GL_TEXTURE_WRAP_S,
                    g_TextureWrapTable[(int)this->m_Wrap]);
    glTexParameteri(m_Target, GL_TEXTURE_WRAP_T,
                    g_TextureWrapTable[(int)this->m_Wrap]);
    glGenerateMipmap(m_Target);
}

void OpenGLTexture::Bind()
{
    Renderer3D::SetLastTextureUnit(m_TextureUnit);
    glActiveTexture(GL_TEXTURE0 + this->m_TextureUnit);
    glBindTexture(m_Target, m_Handle);
}

void OpenGLTexture::Bind(int id)
{
    m_TextureUnit = id;
    Bind();
}

void OpenGLTexture::Unbind()
{
    glBindTexture(m_Target, 0);
}

void OpenGLTexture::SetMinFilter(TextureFilter filter)
{
    this->m_MinFilter = filter;
    Bind();
    glTexParameteri(m_Target, GL_TEXTURE_MIN_FILTER,
                    g_TextureFilterTable[(int)this->m_MinFilter]);
}

void OpenGLTexture::SetMagFilter(TextureFilter filter)
{
    this->m_MagFilter = filter;
    Bind();
    glTexParameteri(m_Target, GL_TEXTURE_MAG_FILTER,
                    g_TextureFilterTable[(int)this->m_MagFilter]);
}

void OpenGLTexture::SetWrap(TextureWrap wrap)
{
    this->m_Wrap = wrap;
    Bind();
    glTexParameteri(m_Target, GL_TEXTURE_WRAP_S,
                    g_TextureWrapTable[(int)this->m_Wrap]);
    glTexParameteri(m_Target, GL_TEXTURE_WRAP_T,
                    g_TextureWrapTable[(int)this->m_Wrap]);
}

Ra::RendererId OpenGLTexture::GetNativeTerxtureHandle() const
{
    return m_Handle;
}

} // namespace Ra
