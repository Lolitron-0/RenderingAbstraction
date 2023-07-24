#include "rapch.h"
#include "OpenGL/OpenGLCubemap.hpp"
#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include "Renderer.hpp"

namespace Ra
{
    OpenGLCubemap::OpenGLCubemap()
    {
        glGenTextures(1, &m_Handle);
    }

    void OpenGLCubemap::Bind(std::uint32_t unit)
    {
        Renderer::SetLastTextureUnit(unit);
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_Handle);
    }

    void OpenGLCubemap::Unbind()
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    void OpenGLCubemap::LoadFromSweep_(const std::string& path)
    {
        m_Path = path;

        int tWidth, tHeight, tNumChannels;
        std::uint8_t* tData = stbi_load(path.c_str(), &tWidth, &tHeight, &tNumChannels, 0);
        RA_ASSERT(tData, stbi_failure_reason());

        glBindTexture(GL_TEXTURE_CUBE_MAP, m_Handle);


        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        GLsizei quadSize = static_cast<GLsizei>(tHeight / 3);
        quadSize = 500;
        int xOffset[6] = { 2, 0, 1, 1, 1, 3 };
        int yOffset[6] = { 1, 1, 0, 2, 1, 1 };
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 0, 0, GL_SRGB, tWidth, tHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tData);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, GL_SRGB, tWidth, tHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tData);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, GL_SRGB, tWidth, tHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tData);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, GL_SRGB, tWidth, tHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tData);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, GL_SRGB, tWidth, tHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tData);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, GL_SRGB, tWidth, tHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tData);

        stbi_image_free(tData);
    }

    void OpenGLCubemap::LoadFromFiles_(const std::vector<std::string>& paths)
    {
        m_Path = paths[0];

        Bind();
        int tWidth, tHeight, tNumChannels;

        for (std::uint8_t i{ 0 }; i < paths.size(); i++)
        {
            std::uint8_t* tData = stbi_load(paths[i].c_str(), &tWidth, &tHeight, &tNumChannels, 0);
            RA_ASSERT(tData, stbi_failure_reason());
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB, tWidth, tHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tData);
            stbi_image_free(tData);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

}