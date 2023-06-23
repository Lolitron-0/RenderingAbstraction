#include "rapch.h"
#include "RenderingContext.hpp"
#include "OpenGL/OpenGLContext.hpp"

namespace ra
{
    Scope<RenderingContext> RenderingContext::Create(std::any windowHandle)
    {
        if (1) // TODO: check rendering api
            return std::make_unique<OpenGLContext>(std::any_cast<GLFWwindow*>(windowHandle));
    }
}