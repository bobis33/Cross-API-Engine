#include "X11/X11.hpp"

#include "Utils/Image.hpp"
#include "Utils/Logger.hpp"

#include <X11/Xlib.h>
#include <X11/keysym.h>

#include <vector>

static cae::KeyCode translateKeycode(const unsigned int keycode)
{
    switch (keycode)
    {
        case 25:
            return cae::KeyCode::W;
        case 38:
            return cae::KeyCode::A;
        case 39:
            return cae::KeyCode::S;
        case 40:
            return cae::KeyCode::D;

        case 111:
            return cae::KeyCode::Up;
        case 116:
            return cae::KeyCode::Down;
        case 113:
            return cae::KeyCode::Left;
        case 114:
            return cae::KeyCode::Right;

        case 65:
            return cae::KeyCode::Space;
        case 37:
            return cae::KeyCode::LCtrl;
        case 50:
            return cae::KeyCode::LShift;
        case 64:
            return cae::KeyCode::LAlt;

        case 9:
            return cae::KeyCode::Escape;
        default:
            return cae::KeyCode::Count;
    }
}

bool cae::X11::create(const std::string &name, const WindowSize size)
{
    m_display = XOpenDisplay(nullptr);
    if (m_display == nullptr)
    {
        utl::Logger::log("Failed to open X display", utl::LogLevel::WARNING);
        return false;
    }

    const int screen = DefaultScreen(m_display);
    const Window root = RootWindow(m_display, screen);

    m_window = XCreateSimpleWindow(m_display, root, 0, 0, size.width, size.height, 1, BlackPixel(m_display, screen),
                                   WhitePixel(m_display, screen));

    if (m_window == 0U)
    {
        utl::Logger::log("Failed to create X11 window", utl::LogLevel::WARNING);
        return false;
    }

    XStoreName(m_display, m_window, name.c_str());

    XSelectInput(m_display, m_window,
                 ExposureMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask | PointerMotionMask |
                     ButtonPressMask | ButtonReleaseMask);
    m_wmDeleteMessage = XInternAtom(m_display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(m_display, m_window, &m_wmDeleteMessage, 1);

    XMapWindow(m_display, m_window);
    XFlush(m_display);

    m_frameBufferSize = size;

    return true;
}

void cae::X11::close()
{
    if (m_display != nullptr && m_window != 0U)
    {
        XDestroyWindow(m_display, m_window);
        XCloseDisplay(m_display);
        m_display = nullptr;
        m_window = 0;
    }
}

cae::WindowSize cae::X11::getWindowSize() const
{
    if (m_display == nullptr || m_window == 0U)
    {
        return m_frameBufferSize;
    }

    XWindowAttributes attrs;
    XGetWindowAttributes(m_display, m_window, &attrs);
    return {.width = static_cast<uint16_t>(attrs.width), .height = static_cast<uint16_t>(attrs.height)};
}

void cae::X11::setIcon(const std::string &path) const
{
    if ((m_display == nullptr) || m_window == 0)
    {
        utl::Logger::log("Failed to create window icon", utl::LogLevel::WARNING);
        return;
    }

    try
    {
        const utl::Image image(path);

        const auto pixelCount = static_cast<const size_t>(image.width * image.height);
        std::vector<unsigned long> iconData;
        iconData.reserve(2 + pixelCount);

        iconData.push_back(static_cast<unsigned long>(image.width));
        iconData.push_back(static_cast<unsigned long>(image.height));

        const uint8_t *pixels = image.pixels;
        for (size_t i = 0; i < pixelCount; ++i)
        {
            const uint8_t r = pixels[i * 4 + 0];
            const uint8_t g = pixels[i * 4 + 1];
            const uint8_t b = pixels[i * 4 + 2];
            const uint8_t a = pixels[i * 4 + 3];

            const unsigned long argb = (static_cast<unsigned long>(a) << 24) | (static_cast<unsigned long>(r) << 16) |
                                       (static_cast<unsigned long>(g) << 8) | (static_cast<unsigned long>(b));

            iconData.push_back(argb);
        }

        const Atom netWmIcon = XInternAtom(m_display, "_NET_WM_ICON", False);
        const Atom cardinal = XInternAtom(m_display, "CARDINAL", False);

        XChangeProperty(m_display, m_window, netWmIcon, cardinal, 32, PropModeReplace,
                        reinterpret_cast<const unsigned char *>(iconData.data()), static_cast<int>(iconData.size()));

        XFlush(m_display);
    }
    catch (const std::exception &e)
    {
        utl::Logger::log(std::string("Failed to set X11 window icon: ") + e.what(), utl::LogLevel::WARNING);
    }
}

bool cae::X11::shouldClose() const { return m_shouldClose; }

void cae::X11::pollEvents() {}

bool cae::X11::pollEvent(WindowEvent &outEvent)
{
    if (m_eventQueue.empty() && XPending(m_display) == 0)
    {
        return false;
    }

    while (XPending(m_display) > 0)
    {
        XEvent event;
        XNextEvent(m_display, &event);
        WindowEvent e{};

        switch (event.type)
        {
            case KeyPress:
                e.type = WindowEventType::KeyDown;
                e.key.key = translateKeycode(event.xkey.keycode);
                m_eventQueue.push(e);
                break;

            case KeyRelease:
                e.type = WindowEventType::KeyUp;
                e.key.key = translateKeycode(event.xkey.keycode);
                m_eventQueue.push(e);
                break;

            case ConfigureNotify:
                m_frameBufferResized = true;
                m_frameBufferSize.width = event.xconfigure.width;
                m_frameBufferSize.height = event.xconfigure.height;
                e.type = WindowEventType::Resize;
                e.resize.w = event.xconfigure.width;
                e.resize.h = event.xconfigure.height;
                m_eventQueue.push(e);
                break;

            case ClientMessage:
                if (static_cast<Atom>(event.xclient.data.l[0]) == m_wmDeleteMessage)
                {
                    m_shouldClose = true;
                    e.type = WindowEventType::Close;
                    m_eventQueue.push(e);
                }
                break;

            case MotionNotify:
                e.type = WindowEventType::MouseMove;
                e.mouseMove.x = event.xmotion.x;
                e.mouseMove.y = event.xmotion.y;
                m_eventQueue.push(e);
                break;

            case ButtonPress:
            case ButtonRelease:
                e.type =
                    (event.type == ButtonPress) ? WindowEventType::MouseButtonDown : WindowEventType::MouseButtonUp;
                e.mouseButton.button = static_cast<MouseButton>(event.xbutton.button);
                m_eventQueue.push(e);
                break;

            case Expose:
                break;

            default:
                break;
        }
    }

    if (!m_eventQueue.empty())
    {
        outEvent = m_eventQueue.front();
        m_eventQueue.pop();
        return true;
    }

    return false;
}
