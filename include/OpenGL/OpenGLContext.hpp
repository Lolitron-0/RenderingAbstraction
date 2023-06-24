#pragma once
#include "RenderingContext.hpp"
#include <GLFW/glfw3.h>

namespace ra
{
    class OpenGLContext final : public RenderingContext
    {
    public:
        OpenGLContext(GLFWwindow* windowHandle);

        void Init() override;
        void SwapBuffers() override;

        APIinfo GetInfo() const override;

        GLFWwindow* m_WindowHandle;
    };

}
