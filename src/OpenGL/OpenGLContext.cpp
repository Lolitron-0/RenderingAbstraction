#include "OpenGL/OpenGLContext.hpp"
#include "rapch.h"
#include <glad/glad.h>

namespace Ra
{

OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
    : m_WindowHandle(windowHandle)
{
    Init();
}

void OpenGLContext::Init()
{
    glfwMakeContextCurrent(m_WindowHandle);

    int status{ gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) };
    RA_ASSERT(status, "Failed to load OpenGL!");

    s_ContextLoadCallback();
}

void OpenGLContext::SwapBuffers()
{
    glfwSwapBuffers(m_WindowHandle);
}

RenderingContext::APIinfo OpenGLContext::GetInfo() const
{
    APIinfo info;
    info.Vendor = (const char*)glGetString(GL_VENDOR);
    info.Renderer = (const char*)glGetString(GL_RENDERER);
    info.Version = (const char*)glGetString(GL_VERSION);
    info.NativeInfo = std::any(GLVersion);
    return info;
}

} // namespace Ra
