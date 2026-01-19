#include "GLFW/GLFW.hpp"

#include "Utils/Image.hpp"
#include "Utils/Logger.hpp"

#include <GLFW/glfw3native.h>
#include <unordered_map>

static cae::KeyCode translateKey(const int key)
{
    static const std::unordered_map<int, cae::KeyCode> keyMap = {
        {GLFW_KEY_A, cae::KeyCode::A},
        {GLFW_KEY_B, cae::KeyCode::B},
        {GLFW_KEY_C, cae::KeyCode::C},
        {GLFW_KEY_D, cae::KeyCode::D},
        {GLFW_KEY_E, cae::KeyCode::E},
        {GLFW_KEY_F, cae::KeyCode::F},
        {GLFW_KEY_G, cae::KeyCode::G},
        {GLFW_KEY_H, cae::KeyCode::H},
        {GLFW_KEY_I, cae::KeyCode::I},
        {GLFW_KEY_J, cae::KeyCode::J},
        {GLFW_KEY_K, cae::KeyCode::K},
        {GLFW_KEY_L, cae::KeyCode::L},
        {GLFW_KEY_M, cae::KeyCode::M},
        {GLFW_KEY_N, cae::KeyCode::N},
        {GLFW_KEY_O, cae::KeyCode::O},
        {GLFW_KEY_P, cae::KeyCode::P},
        {GLFW_KEY_Q, cae::KeyCode::Q},
        {GLFW_KEY_R, cae::KeyCode::R},
        {GLFW_KEY_S, cae::KeyCode::S},
        {GLFW_KEY_T, cae::KeyCode::T},
        {GLFW_KEY_U, cae::KeyCode::U},
        {GLFW_KEY_V, cae::KeyCode::V},
        {GLFW_KEY_W, cae::KeyCode::W},
        {GLFW_KEY_X, cae::KeyCode::X},
        {GLFW_KEY_Y, cae::KeyCode::Y},
        {GLFW_KEY_Z, cae::KeyCode::Z},

        {GLFW_KEY_0, cae::KeyCode::Num0},
        {GLFW_KEY_1, cae::KeyCode::Num1},
        {GLFW_KEY_2, cae::KeyCode::Num2},
        {GLFW_KEY_3, cae::KeyCode::Num3},
        {GLFW_KEY_4, cae::KeyCode::Num4},
        {GLFW_KEY_5, cae::KeyCode::Num5},
        {GLFW_KEY_6, cae::KeyCode::Num6},
        {GLFW_KEY_7, cae::KeyCode::Num7},
        {GLFW_KEY_8, cae::KeyCode::Num8},
        {GLFW_KEY_9, cae::KeyCode::Num9},
        {GLFW_KEY_ESCAPE, cae::KeyCode::Escape},
        {GLFW_KEY_LEFT, cae::KeyCode::Left},
        {GLFW_KEY_RIGHT, cae::KeyCode::Right},
        {GLFW_KEY_UP, cae::KeyCode::Up},
        {GLFW_KEY_DOWN, cae::KeyCode::Down},
        {GLFW_KEY_SPACE, cae::KeyCode::Space},
        {GLFW_KEY_ENTER, cae::KeyCode::Enter},
        {GLFW_KEY_BACKSPACE, cae::KeyCode::Backspace},
        {GLFW_KEY_TAB, cae::KeyCode::Tab},
        {GLFW_KEY_LEFT_SHIFT, cae::KeyCode::LShift},
        {GLFW_KEY_LEFT_CONTROL, cae::KeyCode::LCtrl},
        {GLFW_KEY_LEFT_ALT, cae::KeyCode::LAlt}
        // ...
    };
    const auto it = keyMap.find(key);
    return it != keyMap.end() ? it->second : cae::KeyCode::Count;
}

void cae::GLFW::keyCallback(GLFWwindow *window, const int key, int, const int action, int)
{
    auto *self = static_cast<GLFW *>(glfwGetWindowUserPointer(window));
    if (!self)
        return;

    WindowEvent e{};
    if (action == GLFW_PRESS)
        e.type = WindowEventType::KeyDown;
    else if (action == GLFW_RELEASE)
        e.type = WindowEventType::KeyUp;
    else
        return;

    e.key.key = translateKey(key);
    self->m_eventQueue.push(e);
}

void cae::GLFW::mouseButtonCallback(GLFWwindow *window, int button, const int action, int)
{
    auto *self = static_cast<GLFW *>(glfwGetWindowUserPointer(window));
    if (!self)
        return;

    WindowEvent e{};
    e.type = (action == GLFW_PRESS) ? WindowEventType::MouseButtonDown : WindowEventType::MouseButtonUp;

    e.mouseButton.button = static_cast<MouseButton>(button);
    self->m_eventQueue.push(e);
}

void cae::GLFW::cursorPosCallback(GLFWwindow *window, const double x, const double y)
{
    auto *self = static_cast<GLFW *>(glfwGetWindowUserPointer(window));
    if (!self)
        return;

    WindowEvent e{};
    e.type = WindowEventType::MouseMove;
    e.mouseMove.x = static_cast<int>(x);
    e.mouseMove.y = static_cast<int>(y);

    self->m_eventQueue.push(e);
}

void cae::GLFW::scrollCallback(GLFWwindow *window, const double xoffset, const double yoffset)
{
    auto *self = static_cast<GLFW *>(glfwGetWindowUserPointer(window));
    if (!self)
        return;

    WindowEvent e{};
    e.type = WindowEventType::MouseScroll;
    e.scroll.x = static_cast<float>(xoffset);
    e.scroll.y = static_cast<float>(yoffset);

    self->m_eventQueue.push(e);
}

void cae::GLFW::frameBufferResizeCallback(GLFWwindow *window, const int width, const int height)
{
    auto *self = static_cast<GLFW *>(glfwGetWindowUserPointer(window));
    if (!self)
        return;

    self->m_frameBufferResized = true;
    self->m_frameBufferSize = {static_cast<uint16_t>(width), static_cast<uint16_t>(height)};

    WindowEvent e{};
    e.type = WindowEventType::Resize;
    e.resize.w = self->m_frameBufferSize.width;
    e.resize.h = self->m_frameBufferSize.height;

    self->m_eventQueue.push(e);
}

bool cae::GLFW::create(const std::string &name, const WindowSize size)
{
    m_window = nullptr;
    if (glfwInit() == 0)
    {
        utl::Logger::log("Failed to init glfw", utl::LogLevel::WARNING);
        return false;
    }
#ifdef __APPLE__
#else
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif

    m_window = glfwCreateWindow(size.width, size.height, name.c_str(), nullptr, nullptr);
    if (m_window == nullptr)
    {
        glfwTerminate();
        utl::Logger::log("Failed to create GLFW window", utl::LogLevel::WARNING);

        return false;
    }
    glfwSetWindowUserPointer(m_window, this);

    glfwSetFramebufferSizeCallback(m_window, frameBufferResizeCallback);
    glfwSetKeyCallback(m_window, keyCallback);
    glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
    glfwSetCursorPosCallback(m_window, cursorPosCallback);
    glfwSetScrollCallback(m_window, scrollCallback);
#ifdef __APPLE__
    glfwMakeContextCurrent((GLFWwindow*)m_window);
#endif
    return true;
}

void cae::GLFW::close()
{
    if (m_window != nullptr)
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

cae::NativeWindowHandle cae::GLFW::getNativeHandle() const
{
    NativeWindowHandle handle{};
#ifdef _WIN32
    handle.window = glfwGetWin32Window(m_window);
    handle.display = GetModuleHandle(nullptr);
#elifdef __linux__
    handle.window = reinterpret_cast<void *>(glfwGetX11Window(m_window));
    handle.display = glfwGetX11Display();
#elifdef __APPLE__
    handle.window = glfwGetCocoaWindow(m_window);
    handle.display = nullptr;
#endif
    return handle;
}

void cae::GLFW::setIcon(const std::string &path) const
{
    static const utl::Image image(path);
    if (image.pixels == nullptr)
    {
        utl::Logger::log("Failed to create icon.", utl::LogLevel::WARNING);
    }
    static const GLFWimage appIcon{.width = image.width, .height = image.height, .pixels = image.pixels};
    glfwSetWindowIcon(m_window, 1, &appIcon);
}

bool cae::GLFW::pollEvent(WindowEvent &event)
{
    if (m_eventQueue.empty())
        return false;

    event = m_eventQueue.front();
    m_eventQueue.pop();
    return true;
}
