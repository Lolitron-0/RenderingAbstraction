#include "rapch.h"
#include "OpenGL/OpenGLContext.hpp"
#include <glad/glad.h>

namespace ra
{

    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        :m_WindowHandle(windowHandle)
    {
        Init();
    }

    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle);

        int status{ gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) };
        LITHE_CORE_ASSERT(status, "Failed to initialize Glad!");
        LITHE_LOG_CORE_TRACE("OpenGL info:");
        LITHE_LOG_CORE_TRACE("Vendor: {0}", (const char*)glGetString(GL_VENDOR));
        LITHE_LOG_CORE_TRACE("Renderer: {0}", (const char*)glGetString(GL_RENDERER));
        LITHE_LOG_CORE_TRACE("Version: {0}", (const char*)glGetString(GL_VERSION));
        LITHE_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Lithe needs at least OpenGL version 4.5!");

        LITHE_LOG_CORE_INFO("Created OpenGL rendering context");
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
        return std::move(info);
    }

}