#include <Utils/Image.hpp>
#include <Utils/Logger.hpp>

#include "GLFW/GLFW.hpp"

void cae::GLFW::frameBufferResizeCallback(GLFWwindow *window, int width, int height)
{
    auto *const self = static_cast<GLFW *>(glfwGetWindowUserPointer(window));
    self->m_frameBufferResized = true;
    self->m_frameBufferSize = {static_cast<uint16_t>(width), static_cast<uint16_t>(height)};
}

bool cae::GLFW::create(const std::string &name, const WindowSize size)
{
    m_window = nullptr;
    if (glfwInit() == 0)
    {
        return false;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    m_window = glfwCreateWindow(size.width, size.height, name.c_str(), nullptr, nullptr);
    if (m_window == nullptr)
    {
        glfwTerminate();
        utl::Logger::log("Failed to create GLFW window", utl::LogLevel::WARNING);

        return false;
    }
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, frameBufferResizeCallback);

    return true;
}

void cae::GLFW::close()
{
    if (m_window)
    {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    glfwTerminate();
}

cae::WindowSize cae::GLFW::getWindowSize() const
{
    int width = 0;
    int height = 0;
    glfwGetWindowSize(m_window, &width, &height);
    return {.width = static_cast<uint16_t>(width), .height = static_cast<uint16_t>(height)};
}

bool cae::GLFW::setIcon(const std::string &path) const
{
    static const utl::Image image(path);
    if (image.pixels == nullptr)
    {
        return false;
    }
    static const GLFWimage appIcon{.width = image.width, .height = image.height, .pixels = image.pixels};
    glfwSetWindowIcon(m_window, 1, &appIcon);
    return true;
}
