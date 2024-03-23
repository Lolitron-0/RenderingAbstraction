#include "Shader.hpp"
#include "OpenGL/OpenGLShader.hpp"
#include "Renderer3D.hpp"
#include "rapch.h"
#include <fstream>

namespace Ra
{
Scope<Shader> Shader::Create(const std::string& vertPath,
                             const std::string& fragPath)
{
    switch (Renderer3D::GetAPI())
    {
    case RendererAPI::API::OpenGL:
        return MakeScope<OpenGLShader>(vertPath, fragPath);
    default:
    {
        RA_ASSERT(false, "No rendering API has been set!");
        return nullptr;
    }
    }
}

Scope<Shader> Shader::Create(const std::string& shaderPath)
{
    switch (Renderer3D::GetAPI())
    {
    case RendererAPI::API::OpenGL:
        return MakeScope<OpenGLShader>(shaderPath);
    default:
    {
        RA_ASSERT(false, "No rendering API has been set!");
        return nullptr;
    }
    }
}

std::string Shader::LoadSourceFromFile_(const std::string& path)
{
    std::ifstream stream(path);

    RA_ASSERT(stream.is_open(), "Could not open " + path + " file!");

    std::string line;
    std::string source;

    while (getline(stream, line))
    {
        source += line + '\n';
    }

    stream.close();
    return source;
}
} // namespace Ra
