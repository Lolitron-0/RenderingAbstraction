#include "Shader.hpp"
#include "OpenGL/OpenGLShader.hpp"
#include "RenderCommand.hpp"
#include "rapch.h"
#include <fstream>

namespace Ra
{
Scope<Shader> Shader::Create(const std::string& vertPath,
                             const std::string& fragPath)
{
    switch (RenderCommand::GetAPI())
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
    switch (RenderCommand::GetAPI())
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

auto Shader::LoadSourceFromFile_(const std::string& path) -> std::string
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
