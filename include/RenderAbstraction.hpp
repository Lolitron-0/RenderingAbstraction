#pragma once

#include "Buffer.hpp"
#include "Camera.hpp"
#include "Framebuffer.hpp"
#include "GLFW/glfw3.h"
#include "Mesh.hpp"
#include "RenderCommand.hpp"
#include "Renderer2D.hpp"
#include "Renderer3D.hpp"
#include "RenderingContext.hpp"
#include "Shader.hpp"
#include "SubMesh.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "Window.hpp"
#include <Profiler.hpp>

namespace Ra
{

template <typename T>
void Init(RendererAPI::API api)
{
    static bool s_Initialized{ false };
    static_assert(std::is_same_v<T, Renderer2D> ||
                      std::is_same_v<T, Renderer3D>,
                  "Ra should be initialized with Renderer temlate parameter");

    RA_ASSERT(!s_Initialized, "Ra should be initialized once!");

    auto success{ glfwInit() };
    RA_ASSERT(success, "Glfw initialization falied!");

	Ra::RendererAPI::SetAPI(api);
    Ra::RenderingContext::SetOnContextLoadCallback([]() { T::Init(); });

    s_Initialized = true;
}
} // namespace Ra
