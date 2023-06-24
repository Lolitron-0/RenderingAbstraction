#pragma once
#include "RenderingContext.hpp"
#include <GLFW/glfw3.h>

namespace Ra
{
    class OpenGLContext final : public RenderingContext
    {
    public:
        OpenGLContext(GLFWwindow* windowHandle);
        ~OpenGLContext();

        void Init() override;
        void SwapBuffers() override;

        APIinfo GetInfo() const override;

        GLFWwindow* m_WindowHandle;
    };

}
