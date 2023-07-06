#include "rapch.h"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "OpenGL/OpenGLShader.hpp"
#include <fstream>

namespace Ra
{
    Ref<Shader> Shader::Create(const std::string& vertPath, const std::string& fragPath)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(vertPath, fragPath);
        default: { RA_ASSERT(false, "No rendering API has been set!"); return nullptr; }
        }
    }

    Ref<Shader> Shader::Create(const std::string& shaderPath)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(shaderPath);
        default: { RA_ASSERT(false, "No rendering API has been set!"); return nullptr; }
        }
    }

    std::string Shader::LoadSourceFromFile_(const std::string& path)
    {
        std::ifstream stream(path);

        RA_ASSERT(stream, "Could not open " + path + " file!");

        std::string line;
        std::string source;

        while (getline(stream, line))
            source += line + '\n';

        stream.close();
        return source;
    }
}