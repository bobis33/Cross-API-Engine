#include "X11/X11.hpp"

#include "Utils/Image.hpp"
#include "Utils/Logger.hpp"

#include <utility>
#include <vector>

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

    XSelectInput(m_display, m_window, ExposureMask | KeyPressMask | StructureNotifyMask);

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

bool cae::X11::setIcon(const std::string &path) const
{
    if ((m_display == nullptr) || m_window == 0)
    {
        return false;
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
        return true;
    }
    catch (const std::exception &e)
    {
        utl::Logger::log(std::string("Failed to set X11 window icon: ") + e.what(), utl::LogLevel::WARNING);
        return false;
    }
}

bool cae::X11::shouldClose() const { return m_shouldClose; }

void cae::X11::pollEvents()
{
    while (XPending(m_display) != 0)
    {
        XEvent event;
        XNextEvent(m_display, &event);

        switch (event.type)
        {
            case Expose:
            {
                XGCValues gcValues;
                GC gc = XCreateGC(m_display, m_window, 0, &gcValues);

                XColor color;
                const Colormap colormap = DefaultColormap(m_display, DefaultScreen(m_display));
                color.red = 0x0000;
                color.green = 0x0000;
                color.blue = 0x0000;
                color.flags = DoRed | DoGreen | DoBlue;
                XAllocColor(m_display, colormap, &color);

                XSetForeground(m_display, gc, color.pixel);

                XFillRectangle(m_display, m_window, gc, 0, 0, m_frameBufferSize.width, m_frameBufferSize.height);

                XFreeGC(m_display, gc);
                break;
            }
            case ConfigureNotify:
                m_frameBufferResized = true;
                m_frameBufferSize.width = event.xconfigure.width;
                m_frameBufferSize.height = event.xconfigure.height;
                break;
            case ClientMessage:
                if (std::cmp_equal(event.xclient.data.l[0], m_wmDeleteMessage))
                {
                    m_shouldClose = true;
                }
                break;
            default:
                break;
        }
    }

    XFlush(m_display);
}
