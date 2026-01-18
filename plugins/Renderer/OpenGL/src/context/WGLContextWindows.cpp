#ifdef _WIN32

#include "OPGL/Context/WGLContextWindows.hpp"

#include "Utils/Logger.hpp"

#include <Windows.h>

#include <stdexcept>

typedef HGLRC(WINAPI *PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int *);
typedef const char *(WINAPI *PFNWGLGETEXTENSIONSSTRINGARBPROC)(HDC);

#ifndef WGL_CONTEXT_MAJOR_VERSION_ARB
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#endif
#ifndef WGL_CONTEXT_MINOR_VERSION_ARB
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#endif
#ifndef WGL_CONTEXT_FLAGS_ARB
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#endif
#ifndef WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002
#endif
#ifndef WGL_CONTEXT_PROFILE_MASK_ARB
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#endif
#ifndef WGL_CONTEXT_CORE_PROFILE_BIT_ARB
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#endif

static HMODULE g_opengl32 = nullptr;

static void *win32GetGLProc(const char *name)
{
    auto *proc = (void *)wglGetProcAddress(name);

    if (proc == nullptr || proc == (void *)0x1 || proc == (void *)0x2 || proc == (void *)0x3 || proc == (void *)-1)
    {
        if (g_opengl32 == nullptr)
        {
            g_opengl32 = LoadLibraryA("opengl32.dll");
        }

        proc = (void *)GetProcAddress(g_opengl32, name);
    }

    return proc;
}

cae::WGLContextWindows::~WGLContextWindows()
{
    if (m_hglrc != nullptr)
    {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(m_hglrc);
    }
    if ((m_hdc != nullptr) && (m_hwnd != nullptr))
    {
        ReleaseDC(m_hwnd, m_hdc);
    }
}

void cae::WGLContextWindows::initialize(const NativeWindowHandle &window)
{
    m_hwnd = static_cast<HWND>(window.window);
    m_hdc = GetDC(m_hwnd);
    if (m_hdc == nullptr)
        throw std::runtime_error("Failed to get HDC from HWND");

    PIXELFORMATDESCRIPTOR pfd{};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.iLayerType = PFD_MAIN_PLANE;

    const int pf = ChoosePixelFormat(m_hdc, &pfd);
    if (pf == 0)
    {
        throw std::runtime_error("Failed to choose pixel format");
    }
    if (SetPixelFormat(m_hdc, pf, &pfd) == 0)
    {
        throw std::runtime_error("Failed to set pixel format");
    }

    const HGLRC tempContext = wglCreateContext(m_hdc);
    if (tempContext == nullptr)
        throw std::runtime_error("Failed to create temporary WGL context");
    if (wglMakeCurrent(m_hdc, tempContext) == 0)
        throw std::runtime_error("Failed to make temporary context current");

    const auto wglCreateContextAttribsARB =
        reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));

    if (wglCreateContextAttribsARB != nullptr)
    {
        const int attribs[] = {WGL_CONTEXT_MAJOR_VERSION_ARB,
                               4,
                               WGL_CONTEXT_MINOR_VERSION_ARB,
                               6,
                               WGL_CONTEXT_FLAGS_ARB,
                               WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                               WGL_CONTEXT_PROFILE_MASK_ARB,
                               WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                               0};

        const HGLRC modernContext = wglCreateContextAttribsARB(m_hdc, nullptr, attribs);
        if (modernContext == nullptr)
        {
            throw std::runtime_error("Failed to create modern WGL context");
        }

        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(tempContext);

        m_hglrc = modernContext;
        if (wglMakeCurrent(m_hdc, m_hglrc) == 0)
        {
            throw std::runtime_error("Failed to make modern WGL context current");
        }
    }
    else
    {
        utl::Logger::log("wglCreateContextAttribsARB not available, using legacy context\n", utl::LogLevel::WARNING);
        m_hglrc = tempContext;
    }

    if (wglGetCurrentContext() != m_hglrc)
    {
        throw std::runtime_error("Current WGL context is not the one just created");
    }
    if (const int version = gladLoadGLContext(&gl, GLADloadfunc(win32GetGLProc)); version == 0)
    {
        throw std::runtime_error("Failed to initialize GLAD MX (Windows)");
    }
    if (gl.Enable != nullptr)
    {
        gl.Enable(GL_DEBUG_OUTPUT);
        gl.DebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                   const GLchar *message, const void *userParam)
                                { utl::Logger::log("[GL DEBUG] " + std::string(message), utl::LogLevel::WARNING); },
                                nullptr);
    }
}

void cae::WGLContextWindows::swapBuffers()
{
    if (m_hdc != nullptr)
    {
        SwapBuffers(m_hdc);
    }
}

void cae::WGLContextWindows::setVSyncEnabled(const bool enabled)
{
    using PFNWGLSWAPINTERVALEXTPROC = BOOL(WINAPI *)(int);
    static auto wglSwapIntervalEXT =
        reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));
    if (wglSwapIntervalEXT != nullptr)
    {
        wglSwapIntervalEXT(enabled ? 1 : 0);
    }
}

#endif
