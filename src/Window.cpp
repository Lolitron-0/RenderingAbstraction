#include "Window.hpp"

#include <utility>

namespace Ra
{

Window::Window(const WindowProps& props)
{
    _init(props);
}

Window::~Window()
{
    glfwDestroyWindow(m_Handle);
}

void Window::_init(const WindowProps& props)
{
    m_Data.Width = props.Width;
    m_Data.Height = props.Height;
    m_Data.Title = props.Title;

    glfwWindowHint(GLFW_SAMPLES, 4);

    m_Handle = glfwCreateWindow(static_cast<int>(m_Data.Width),
                                static_cast<int>(m_Data.Height),
                                m_Data.Title.c_str(), nullptr, nullptr);
    m_Context = Ra::RenderingContext::Create(std::any{ m_Handle });


    glfwSetWindowUserPointer(m_Handle, &m_Data);
    SetVSync(true);
}

void Window::OnUpdate()
{
    m_Context->SwapBuffers();
}

auto Window::ShouldClose() const -> bool
{
    return glfwWindowShouldClose(m_Handle);
}

void Window::SetVSync(bool enabled)
{
    glfwSwapInterval(enabled ? 1 : 0);
    m_Data.VSync = enabled;
}

auto Window::IsVSync() const -> bool
{
    return m_Data.VSync;
}


} // namespace Ra
