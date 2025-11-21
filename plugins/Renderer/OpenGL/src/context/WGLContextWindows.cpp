#if defined(_WIN32)

#include <stdexcept>

#include "OPGL/glad/glad.h"

#include "OPGL/Context/WGLContextWindows.hpp"

cae::WGLContextWindows::~WGLContextWindows()
{
    if (m_hglrc)
    {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(m_hglrc);
    }
    if (m_hdc && m_hwnd)
        ReleaseDC(m_hwnd, m_hdc);
}

void cae::WGLContextWindows::initialize(const NativeWindowHandle &window)
{
    m_hwnd = reinterpret_cast<HWND>(window.window);
    m_hdc = GetDC(m_hwnd);

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pf = ChoosePixelFormat(m_hdc, &pfd);
    if (!pf || !SetPixelFormat(m_hdc, pf, &pfd))
        throw std::runtime_error("Failed to set pixel format");

    m_hglrc = wglCreateContext(m_hdc);
    if (!m_hglrc)
        throw std::runtime_error("Failed to create WGL context");

    if (!wglMakeCurrent(m_hdc, m_hglrc))
        throw std::runtime_error("Failed to make WGL context current");

    if (gladLoadGLLoader((GLADloadproc)wglGetProcAddress) == 0)
        throw std::runtime_error("Failed to initialize GLAD");
}

void cae::WGLContextWindows::swapBuffers()
{
    if (m_hdc)
    {
        ::SwapBuffers(m_hdc);
    }
}

void cae::WGLContextWindows::setVSyncEnabled(const bool enabled)
{
    typedef BOOL(WINAPI * PFNWGLSWAPINTERVALEXTPROC)(int);
    static PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT =
        reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));
    if (wglSwapIntervalEXT)
    {
        wglSwapIntervalEXT(enabled ? 1 : 0);
    }
}

#endif
