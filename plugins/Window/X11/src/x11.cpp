#include "X11/X11.hpp"

#include "Utils/Image.hpp"
#include "Utils/Logger.hpp"

#include <X11/keysym.h>

#include <X11/XKBlib.h>
#include <utility>
#include <vector>

static cae::KeyCode translateKeysym(const KeySym sym)
{
    switch (sym)
    {
        case XK_a:
            return cae::KeyCode::A;
        case XK_b:
            return cae::KeyCode::B;
        case XK_c:
            return cae::KeyCode::C;
        case XK_d:
            return cae::KeyCode::D;
        case XK_e:
            return cae::KeyCode::E;
        case XK_f:
            return cae::KeyCode::F;
        case XK_g:
            return cae::KeyCode::G;
        case XK_h:
            return cae::KeyCode::H;
        case XK_i:
            return cae::KeyCode::I;
        case XK_j:
            return cae::KeyCode::J;
        case XK_k:
            return cae::KeyCode::K;
        case XK_l:
            return cae::KeyCode::L;
        case XK_m:
            return cae::KeyCode::M;
        case XK_n:
            return cae::KeyCode::N;
        case XK_o:
            return cae::KeyCode::O;
        case XK_p:
            return cae::KeyCode::P;
        case XK_q:
            return cae::KeyCode::Q;
        case XK_r:
            return cae::KeyCode::R;
        case XK_s:
            return cae::KeyCode::S;
        case XK_t:
            return cae::KeyCode::T;
        case XK_u:
            return cae::KeyCode::U;
        case XK_v:
            return cae::KeyCode::V;
        case XK_w:
            return cae::KeyCode::W;
        case XK_x:
            return cae::KeyCode::X;
        case XK_y:
            return cae::KeyCode::Y;
        case XK_z:
            return cae::KeyCode::Z;

        // Numbers
        case XK_0:
            return cae::KeyCode::Num0;
        case XK_1:
            return cae::KeyCode::Num1;
        case XK_2:
            return cae::KeyCode::Num2;
        case XK_3:
            return cae::KeyCode::Num3;
        case XK_4:
            return cae::KeyCode::Num4;
        case XK_5:
            return cae::KeyCode::Num5;
        case XK_6:
            return cae::KeyCode::Num6;
        case XK_7:
            return cae::KeyCode::Num7;
        case XK_8:
            return cae::KeyCode::Num8;
        case XK_9:
            return cae::KeyCode::Num9;

        // Modifiers
        case XK_Shift_L:
            return cae::KeyCode::LShift;
        case XK_Shift_R:
            return cae::KeyCode::RShift;
        case XK_Control_L:
            return cae::KeyCode::LCtrl;
        case XK_Control_R:
            return cae::KeyCode::RCtrl;
        case XK_Alt_L:
            return cae::KeyCode::LAlt;
        case XK_Alt_R:
            return cae::KeyCode::RAlt;
        case XK_Super_L:
            return cae::KeyCode::LSuper;
        case XK_Super_R:
            return cae::KeyCode::RSuper;
        case XK_Caps_Lock:
            return cae::KeyCode::CapsLock;

        // Navigation
        case XK_Up:
            return cae::KeyCode::Up;
        case XK_Down:
            return cae::KeyCode::Down;
        case XK_Left:
            return cae::KeyCode::Left;
        case XK_Right:
            return cae::KeyCode::Right;
        case XK_Home:
            return cae::KeyCode::Home;
        case XK_End:
            return cae::KeyCode::End;
        case XK_Page_Up:
            return cae::KeyCode::PageUp;
        case XK_Page_Down:
            return cae::KeyCode::PageDown;

        // Editing
        case XK_Return:
            return cae::KeyCode::Enter;
        case XK_BackSpace:
            return cae::KeyCode::Backspace;
        case XK_Tab:
            return cae::KeyCode::Tab;
        case XK_space:
            return cae::KeyCode::Space;
        case XK_Delete:
            return cae::KeyCode::Delete;
        case XK_Insert:
            return cae::KeyCode::Insert;

        // Function keys
        case XK_F1:
            return cae::KeyCode::F1;
        case XK_F2:
            return cae::KeyCode::F2;
        case XK_F3:
            return cae::KeyCode::F3;
        case XK_F4:
            return cae::KeyCode::F4;
        case XK_F5:
            return cae::KeyCode::F5;
        case XK_F6:
            return cae::KeyCode::F6;
        case XK_F7:
            return cae::KeyCode::F7;
        case XK_F8:
            return cae::KeyCode::F8;
        case XK_F9:
            return cae::KeyCode::F9;
        case XK_F10:
            return cae::KeyCode::F10;
        case XK_F11:
            return cae::KeyCode::F11;
        case XK_F12:
            return cae::KeyCode::F12;

        // System
        case XK_Escape:
            return cae::KeyCode::Escape;
        case XK_Print:
            return cae::KeyCode::PrintScreen;
        case XK_Pause:
            return cae::KeyCode::Pause;
        case XK_Menu:
            return cae::KeyCode::Menu;

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

        const auto pixelCount = image.width * image.height;
        std::vector<unsigned long> iconData;
        iconData.reserve(2 + pixelCount);

        iconData.push_back(static_cast<unsigned long>(image.width));
        iconData.push_back(static_cast<unsigned long>(image.height));

        const uint8_t *pixels = image.pixels;
        for (size_t i = 0; std::cmp_less(i, pixelCount); ++i)
        {
            const uint8_t r = pixels[(i * 4) + 0];
            const uint8_t g = pixels[(i * 4) + 1];
            const uint8_t b = pixels[(i * 4) + 2];
            const uint8_t a = pixels[(i * 4) + 3];

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
            {
                e.type = WindowEventType::KeyDown;

                const KeySym sym = XkbKeycodeToKeysym(m_display, event.xkey.keycode, 0, 0);

                e.key.key = translateKeysym(sym);
                m_eventQueue.push(e);
                break;
            }

            case KeyRelease:
            {
                e.type = WindowEventType::KeyUp;

                const KeySym sym = XkbKeycodeToKeysym(m_display, event.xkey.keycode, 0, 0);

                e.key.key = translateKeysym(sym);
                m_eventQueue.push(e);
                break;
            }

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
                if (std::cmp_equal(event.xclient.data.l[0], m_wmDeleteMessage))
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
