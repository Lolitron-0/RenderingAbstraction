#include "OpenGL/OpenGLRendererAPI.hpp"
#include "Buffer.hpp"
#include "GLFW/glfw3.h"
#include "VertexArray.hpp"
#include "rapch.h"
#include <glad/glad.h>

namespace Ra
{
void GLDebugMessageCallback(GLenum /*source*/, GLenum /*type*/, GLuint /*id*/,
                            GLenum severity, GLsizei /*length*/,
                            const GLchar* message, const void* /*userParam*/)
{
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_LOW:
    case GL_DEBUG_SEVERITY_MEDIUM:
    case GL_DEBUG_SEVERITY_HIGH:
        RA_ASSERT(false, message);
        break;
    default:
        break;
    }
}

OpenGLRendererAPI::OpenGLRendererAPI()
{
    Init();
}

void OpenGLRendererAPI::Init()
{
#ifdef RA_DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(GLDebugMessageCallback, nullptr);
#endif
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //
    // glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    //glEnable(GL_CULL_FACE);
    glPointSize(10);
}

void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::SetViewport(std::uint32_t x, std::uint32_t y,
                                    std::uint32_t width, std::uint32_t height)
{
    glViewport(x, y, width, height);
}

void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& array,
                                    DrawMode mode, std::size_t indexCount)
{
    array->Bind();
    glDrawElements(
        ToAPIDrawMode_(mode),
        static_cast<GLsizei>(
            indexCount != 0 ? indexCount : array->GetIndexBufer()->GetCount()),
        GL_UNSIGNED_INT, nullptr);
}

void OpenGLRendererAPI::SetDepthBufferReadOnly(bool readOnly)
{
    glDepthMask(readOnly ? GL_FALSE : GL_TRUE);
}

void OpenGLRendererAPI::SetDepthFunc(DepthFunc func)
{
    glDepthFunc(ToAPIDepthFunc_(func));
}

int OpenGLRendererAPI::ToAPIDrawMode_(RendererAPI::DrawMode mode)
{
    return s_ToAPIDrawModeMap.at(mode);
}

int OpenGLRendererAPI::ToAPIDepthFunc_(RendererAPI::DepthFunc func)
{
    switch (func)
    {
    case RendererAPI::DepthFunc::Always:
        return GL_ALWAYS;
    case RendererAPI::DepthFunc::Never:
        return GL_NEVER;
    case RendererAPI::DepthFunc::Less:
        return GL_LESS;
    case RendererAPI::DepthFunc::Equal:
        return GL_EQUAL;
    case RendererAPI::DepthFunc::Lequal:
        return GL_LEQUAL;
    case RendererAPI::DepthFunc::Greater:
        return GL_GREATER;
    case RendererAPI::DepthFunc::Gequal:
        return GL_GEQUAL;
    case RendererAPI::DepthFunc::NotEqual:
        return GL_NOTEQUAL;
    }
    return GL_LESS;
}

void OpenGLRendererAPI::SetCullFace(CullFace face)
{
    GLenum f;

    switch (face)
    {
    case Ra::RendererAPI::CullFace::Both:
        f = GL_FRONT_AND_BACK;
        break;
    case Ra::RendererAPI::CullFace::Back:
        f = GL_BACK;
        break;
    case Ra::RendererAPI::CullFace::Front:
        f = GL_FRONT;
        break;
    }

    glCullFace(f);
}

const std::unordered_map<Ra::RendererAPI::DrawMode, int>
    OpenGLRendererAPI::s_ToAPIDrawModeMap = {
        { RendererAPI::DrawMode::Triangles, GL_TRIANGLES },
        { RendererAPI::DrawMode::Lines, GL_LINES },
        { RendererAPI::DrawMode::Points, GL_POINTS },
        { RendererAPI::DrawMode::TriangleFan, GL_TRIANGLE_FAN },
        { RendererAPI::DrawMode::LineLoop, GL_LINE_LOOP },
        { RendererAPI::DrawMode::TriangleStrip, GL_TRIANGLE_STRIP },
        { RendererAPI::DrawMode::LineStrip, GL_LINE_STRIP },
    };

} // namespace Ra
