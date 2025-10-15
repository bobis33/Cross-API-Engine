#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdexcept>
#include <iostream>

#include "X11/X11.hpp"

    namespace
    {
        struct X11Context
        {
            Display *display = nullptr;
            ::Window window = 0;
            Atom wmDeleteMessage = 0;
            bool shouldClose = false;
        };
    }

    static X11Context g_ctx;

    bool cae::X11::create(const std::string &name, WindowSize size)
    {
        g_ctx.display = XOpenDisplay(nullptr);
        if (!g_ctx.display)
        {
            std::cerr << "[X11] Failed to open X display\n";
            return false;
        }

        const int screen = DefaultScreen(g_ctx.display);
        const Window root = RootWindow(g_ctx.display, screen);

        g_ctx.window = XCreateSimpleWindow(
            g_ctx.display,
            root,
            0, 0,
            size.width,
            size.height,
            1,
            BlackPixel(g_ctx.display, screen),
            WhitePixel(g_ctx.display, screen)
        );

        if (!g_ctx.window)
        {
            std::cerr << "[X11] Failed to create X11 window\n";
            return false;
        }

        XStoreName(g_ctx.display, g_ctx.window, name.c_str());

        XSelectInput(g_ctx.display, g_ctx.window,
                     ExposureMask | KeyPressMask | StructureNotifyMask);

        g_ctx.wmDeleteMessage = XInternAtom(g_ctx.display, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(g_ctx.display, g_ctx.window, &g_ctx.wmDeleteMessage, 1);

        XMapWindow(g_ctx.display, g_ctx.window);
        XFlush(g_ctx.display);

        m_window = reinterpret_cast<void *>(&g_ctx);
        m_frameBufferSize = size;

        return true;
    }

    void cae::X11::close()
    {
        if (g_ctx.display && g_ctx.window)
        {
            XDestroyWindow(g_ctx.display, g_ctx.window);
            XCloseDisplay(g_ctx.display);
            g_ctx.display = nullptr;
            g_ctx.window = 0;
        }
    }

    cae::WindowSize cae::X11::getWindowSize() const
    {
        if (!g_ctx.display || !g_ctx.window)
            return m_frameBufferSize;

        XWindowAttributes attrs;
        XGetWindowAttributes(g_ctx.display, g_ctx.window, &attrs);
        return { static_cast<uint16_t>(attrs.width), static_cast<uint16_t>(attrs.height) };
    }

    bool cae::X11::setIcon(const std::string &path) const
    {
        std::cerr << "[X11] setIcon() not implemented yet (" << path << ")\n";
        return false;
    }

    bool cae::X11::shouldClose() const
    {
        return g_ctx.shouldClose;
    }

void cae::X11::pollEvents()
    {
        while (XPending(g_ctx.display))
        {
            XEvent event;
            XNextEvent(g_ctx.display, &event);

            switch (event.type)
            {
                case Expose:
                {
                    XGCValues gcValues;
                    GC gc = XCreateGC(g_ctx.display, g_ctx.window, 0, &gcValues);

                    // Choisir la couleur rouge
                    unsigned long red_pixel = 0xff0000; // RGB hex pour rouge
                    // Attention : X11 ne prend pas directement 0xff0000, il faut créer une couleur
                    XColor color;
                    Colormap colormap = DefaultColormap(g_ctx.display, DefaultScreen(g_ctx.display));
                    color.red = 0x0000;   // rouge max
                    color.green = 0x0000; // vert = 0
                    color.blue = 0x0000;  // bleu = 0
                    color.flags = DoRed | DoGreen | DoBlue;
                    XAllocColor(g_ctx.display, colormap, &color);

                    XSetForeground(g_ctx.display, gc, color.pixel);

                    // Remplir la fenêtre
                    XFillRectangle(g_ctx.display, g_ctx.window, gc, 0, 0,
                                   m_frameBufferSize.width, m_frameBufferSize.height);

                    XFreeGC(g_ctx.display, gc);
                    break;
                }
                case ConfigureNotify:
                    m_frameBufferResized = true;
                    m_frameBufferSize.width = event.xconfigure.width;
                    m_frameBufferSize.height = event.xconfigure.height;
                    break;
                case ClientMessage:
                    if (static_cast<Atom>(event.xclient.data.l[0]) == g_ctx.wmDeleteMessage)
                        g_ctx.shouldClose = true;
                    break;
                default:
                    break;
            }
        }

        XFlush(g_ctx.display); // assure que le rendu est envoyé
    }
