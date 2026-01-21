#include "GLFW/GLFW.hpp"

#include "Utils/Image.hpp"
#include "Utils/Logger.hpp"

#include <GLFW/glfw3native.h>

#include <unordered_map>

static cae::KeyCode translateKey(const int key)
{
    switch (key)
    {
        case GLFW_KEY_A:
            return cae::KeyCode::A;
        case GLFW_KEY_B:
            return cae::KeyCode::B;
        case GLFW_KEY_C:
            return cae::KeyCode::C;
        case GLFW_KEY_D:
            return cae::KeyCode::D;
        case GLFW_KEY_E:
            return cae::KeyCode::E;
        case GLFW_KEY_F:
            return cae::KeyCode::F;
        case GLFW_KEY_G:
            return cae::KeyCode::G;
        case GLFW_KEY_H:
            return cae::KeyCode::H;
        case GLFW_KEY_I:
            return cae::KeyCode::I;
        case GLFW_KEY_J:
            return cae::KeyCode::J;
        case GLFW_KEY_K:
            return cae::KeyCode::K;
        case GLFW_KEY_L:
            return cae::KeyCode::L;
        case GLFW_KEY_M:
            return cae::KeyCode::M;
        case GLFW_KEY_N:
            return cae::KeyCode::N;
        case GLFW_KEY_O:
            return cae::KeyCode::O;
        case GLFW_KEY_P:
            return cae::KeyCode::P;
        case GLFW_KEY_Q:
            return cae::KeyCode::Q;
        case GLFW_KEY_R:
            return cae::KeyCode::R;
        case GLFW_KEY_S:
            return cae::KeyCode::S;
        case GLFW_KEY_T:
            return cae::KeyCode::T;
        case GLFW_KEY_U:
            return cae::KeyCode::U;
        case GLFW_KEY_V:
            return cae::KeyCode::V;
        case GLFW_KEY_W:
            return cae::KeyCode::W;
        case GLFW_KEY_X:
            return cae::KeyCode::X;
        case GLFW_KEY_Y:
            return cae::KeyCode::Y;
        case GLFW_KEY_Z:
            return cae::KeyCode::Z;

        case GLFW_KEY_0:
            return cae::KeyCode::Num0;
        case GLFW_KEY_1:
            return cae::KeyCode::Num1;
        case GLFW_KEY_2:
            return cae::KeyCode::Num2;
        case GLFW_KEY_3:
            return cae::KeyCode::Num3;
        case GLFW_KEY_4:
            return cae::KeyCode::Num4;
        case GLFW_KEY_5:
            return cae::KeyCode::Num5;
        case GLFW_KEY_6:
            return cae::KeyCode::Num6;
        case GLFW_KEY_7:
            return cae::KeyCode::Num7;
        case GLFW_KEY_8:
            return cae::KeyCode::Num8;
        case GLFW_KEY_9:
            return cae::KeyCode::Num9;

        case GLFW_KEY_LEFT_SHIFT:
            return cae::KeyCode::LShift;
        case GLFW_KEY_RIGHT_SHIFT:
            return cae::KeyCode::RShift;
        case GLFW_KEY_LEFT_CONTROL:
            return cae::KeyCode::LCtrl;
        case GLFW_KEY_RIGHT_CONTROL:
            return cae::KeyCode::RCtrl;
        case GLFW_KEY_LEFT_ALT:
            return cae::KeyCode::LAlt;
        case GLFW_KEY_RIGHT_ALT:
            return cae::KeyCode::RAlt;
        case GLFW_KEY_LEFT_SUPER:
            return cae::KeyCode::LSuper;
        case GLFW_KEY_RIGHT_SUPER:
            return cae::KeyCode::RSuper;
        case GLFW_KEY_CAPS_LOCK:
            return cae::KeyCode::CapsLock;

        case GLFW_KEY_UP:
            return cae::KeyCode::Up;
        case GLFW_KEY_DOWN:
            return cae::KeyCode::Down;
        case GLFW_KEY_LEFT:
            return cae::KeyCode::Left;
        case GLFW_KEY_RIGHT:
            return cae::KeyCode::Right;
        case GLFW_KEY_HOME:
            return cae::KeyCode::Home;
        case GLFW_KEY_END:
            return cae::KeyCode::End;
        case GLFW_KEY_PAGE_UP:
            return cae::KeyCode::PageUp;
        case GLFW_KEY_PAGE_DOWN:
            return cae::KeyCode::PageDown;

        case GLFW_KEY_ENTER:
            return cae::KeyCode::Enter;
        case GLFW_KEY_BACKSPACE:
            return cae::KeyCode::Backspace;
        case GLFW_KEY_TAB:
            return cae::KeyCode::Tab;
        case GLFW_KEY_SPACE:
            return cae::KeyCode::Space;
        case GLFW_KEY_DELETE:
            return cae::KeyCode::Delete;
        case GLFW_KEY_INSERT:
            return cae::KeyCode::Insert;

        case GLFW_KEY_F1:
            return cae::KeyCode::F1;
        case GLFW_KEY_F2:
            return cae::KeyCode::F2;
        case GLFW_KEY_F3:
            return cae::KeyCode::F3;
        case GLFW_KEY_F4:
            return cae::KeyCode::F4;
        case GLFW_KEY_F5:
            return cae::KeyCode::F5;
        case GLFW_KEY_F6:
            return cae::KeyCode::F6;
        case GLFW_KEY_F7:
            return cae::KeyCode::F7;
        case GLFW_KEY_F8:
            return cae::KeyCode::F8;
        case GLFW_KEY_F9:
            return cae::KeyCode::F9;
        case GLFW_KEY_F10:
            return cae::KeyCode::F10;
        case GLFW_KEY_F11:
            return cae::KeyCode::F11;
        case GLFW_KEY_F12:
            return cae::KeyCode::F12;

        case GLFW_KEY_ESCAPE:
            return cae::KeyCode::Escape;
        case GLFW_KEY_PRINT_SCREEN:
            return cae::KeyCode::PrintScreen;
        case GLFW_KEY_PAUSE:
            return cae::KeyCode::Pause;
        case GLFW_KEY_MENU:
            return cae::KeyCode::Menu;

        default:
            return cae::KeyCode::Count;
    }
}

void cae::GLFW::keyCallback(GLFWwindow *window, const int key, int, const int action, int)
{
    auto *self = static_cast<GLFW *>(glfwGetWindowUserPointer(window));
    if (self == nullptr)
    {
        return;
    }

    WindowEvent e{};
    if (action == GLFW_PRESS)
    {
        e.type = WindowEventType::KeyDown;
    }
    else if (action == GLFW_RELEASE)
    {
        e.type = WindowEventType::KeyUp;
    }
    else
    {
        return;
    }

    e.key.key = translateKey(key);
    self->m_eventQueue.push(e);
}

void cae::GLFW::mouseButtonCallback(GLFWwindow *window, int button, const int action, int)
{
    auto *self = static_cast<GLFW *>(glfwGetWindowUserPointer(window));
    if (self == nullptr)
    {
        return;
    }

    WindowEvent e{};
    e.type = (action == GLFW_PRESS) ? WindowEventType::MouseButtonDown : WindowEventType::MouseButtonUp;

    e.mouseButton.button = static_cast<MouseButton>(button);
    self->m_eventQueue.push(e);
}

void cae::GLFW::cursorPosCallback(GLFWwindow *window, const double x, const double y)
{
    auto *self = static_cast<GLFW *>(glfwGetWindowUserPointer(window));
    if (self == nullptr)
    {
        return;
    }

    WindowEvent e{};
    e.type = WindowEventType::MouseMove;
    e.mouseMove.x = static_cast<int>(x);
    e.mouseMove.y = static_cast<int>(y);

    self->m_eventQueue.push(e);
}

void cae::GLFW::scrollCallback(GLFWwindow *window, const double xoffset, const double yoffset)
{
    auto *self = static_cast<GLFW *>(glfwGetWindowUserPointer(window));
    if (self == nullptr)
    {
        return;
    }

    WindowEvent e{};
    e.type = WindowEventType::MouseScroll;
    e.scroll.x = static_cast<float>(xoffset);
    e.scroll.y = static_cast<float>(yoffset);

    self->m_eventQueue.push(e);
}

void cae::GLFW::frameBufferResizeCallback(GLFWwindow *window, const int width, const int height)
{
    auto *self = static_cast<GLFW *>(glfwGetWindowUserPointer(window));
    if (self == nullptr)
    {
        return;
    }

    self->m_frameBufferResized = true;
    self->m_frameBufferSize = {.width = static_cast<uint16_t>(width), .height = static_cast<uint16_t>(height)};

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
    glfwMakeContextCurrent((GLFWwindow *)m_window);
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
        return;
    }
    static const GLFWimage appIcon{.width = image.width, .height = image.height, .pixels = image.pixels};
    glfwSetWindowIcon(m_window, 1, &appIcon);
}

bool cae::GLFW::pollEvent(WindowEvent &event)
{
    if (m_eventQueue.empty())
    {
        return false;
    }

    event = m_eventQueue.front();
    m_eventQueue.pop();
    return true;
}
