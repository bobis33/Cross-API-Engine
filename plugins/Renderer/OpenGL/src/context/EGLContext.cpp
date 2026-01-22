#ifdef __linux__

#include "OPGL/Context/EGLContext.hpp"

#include "Utils/Logger.hpp"

#include <EGL/eglext.h>

#include <iostream>
#include <stdexcept>
#include <vector>

cae::EGLContext_::~EGLContext_()
{
    if (m_display != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (m_context != EGL_NO_CONTEXT)
        {
            eglDestroyContext(m_display, m_context);
        }
        if (m_surface != EGL_NO_SURFACE)
        {
            eglDestroySurface(m_display, m_surface);
        }
        eglTerminate(m_display);
    }
}

void selectDevice(EGLDisplay &display, const PFNEGLQUERYDEVICESEXTPROC eglQueryDevicesEXT, const PFNEGLQUERYDEVICESTRINGEXTPROC eglQueryDeviceStringEXT, const PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT)
{
    EGLint numDevices = 0;
    eglQueryDevicesEXT(0, nullptr, &numDevices);

    if (numDevices == 0)
    {
        throw std::runtime_error("No EGL devices found");
    }

    std::vector<EGLDeviceEXT> devices(numDevices);
    eglQueryDevicesEXT(numDevices, devices.data(), &numDevices);

    for (EGLint i = 0; i < numDevices; ++i)
    {
        const char* vendor =
            eglQueryDeviceStringEXT(devices[i], EGL_VENDOR);

        const char* extensions =
            eglQueryDeviceStringEXT(devices[i], EGL_EXTENSIONS);

        std::cout << "[EGL] Device " << i << "\n";
        std::cout << "  Vendor     : " << ((vendor != nullptr) ? vendor : "unknown") << "\n";
        std::cout << "  Extensions : " << ((extensions != nullptr) ? extensions : "none") << "\n";
    }

    display = eglGetPlatformDisplayEXT(
        EGL_PLATFORM_DEVICE_EXT,
        devices[0],
        nullptr
    );
}

void cae::EGLContext_::initialize(const NativeWindowHandle &window)
{
    const auto eglQueryDevicesEXT =
    reinterpret_cast<PFNEGLQUERYDEVICESEXTPROC>(
        eglGetProcAddress("eglQueryDevicesEXT"));

    const auto eglQueryDeviceStringEXT =
        reinterpret_cast<PFNEGLQUERYDEVICESTRINGEXTPROC>(
            eglGetProcAddress("eglQueryDeviceStringEXT"));

    const auto eglGetPlatformDisplayEXT =
    reinterpret_cast<PFNEGLGETPLATFORMDISPLAYEXTPROC>(
        eglGetProcAddress("eglGetPlatformDisplayEXT"));

    if ((eglQueryDevicesEXT == nullptr) || (eglQueryDeviceStringEXT == nullptr))
    {
        throw std::runtime_error("EGL device enumeration not supported");
    }

    if (eglBindAPI(EGL_OPENGL_API) == EGL_FALSE)
    {
        throw std::runtime_error("Failed to bind OpenGL API");
    }

    selectDevice(m_display, eglQueryDevicesEXT, eglQueryDeviceStringEXT, eglGetPlatformDisplayEXT);

    if (m_display == EGL_NO_DISPLAY)
    {
        throw std::runtime_error("Failed to get EGL display");
    }

    if (eglInitialize(m_display, nullptr, nullptr) == EGL_FALSE)
    {
        throw std::runtime_error("Failed to initialize EGL");
    }

    constexpr EGLint configAttribs[] = {EGL_RED_SIZE,
                                        8,
                                        EGL_GREEN_SIZE,
                                        8,
                                        EGL_BLUE_SIZE,
                                        8,
                                        EGL_DEPTH_SIZE,
                                        24,
                                        EGL_SURFACE_TYPE,
                                        EGL_WINDOW_BIT,
                                        EGL_RENDERABLE_TYPE,
                                        EGL_OPENGL_BIT,
                                        EGL_NONE};

    EGLConfig config = nullptr;
    EGLint numConfigs = 0;
    if (eglChooseConfig(m_display, configAttribs, &config, 1, &numConfigs) == EGL_FALSE || numConfigs == 0)
    {
        throw std::runtime_error("Failed to choose EGL config");
    }

    m_surface =
        eglCreateWindowSurface(m_display, config, reinterpret_cast<EGLNativeWindowType>(window.window), nullptr);
    if (m_surface == EGL_NO_SURFACE)
    {
        throw std::runtime_error("Failed to create EGL surface");
    }

    constexpr EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
    m_context = eglCreateContext(m_display, config, EGL_NO_CONTEXT, contextAttribs);
    if (m_context == EGL_NO_CONTEXT)
    {
        throw std::runtime_error("Failed to create EGL context");
    }

    if (eglMakeCurrent(m_display, m_surface, m_surface, m_context) == EGL_FALSE)
    {
        throw std::runtime_error("Failed to make EGL context current");
    }

    eglSwapInterval(m_display, 0);

    if (gladLoadGLContext(&gl, eglGetProcAddress) == 0)
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }
#ifdef CAE_DEBUG
    if (gl.Enable != nullptr)
    {
        gl.Enable(GL_DEBUG_OUTPUT);

        gl.DebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                               const GLchar *message, const void *userParam)
                            { utl::Logger::log("[GL DEBUG] " + std::string(message), utl::LogLevel::WARNING); },
                            nullptr);
    }
#endif
}

void cae::EGLContext_::swapBuffers()
{
    if (m_display != EGL_NO_DISPLAY && m_surface != EGL_NO_SURFACE)
    {
        eglSwapBuffers(m_display, m_surface);
    }
}

void cae::EGLContext_::setVSyncEnabled(const bool enabled)
{
    if (m_display != EGL_NO_DISPLAY)
    {
        eglSwapInterval(m_display, enabled ? 1 : 0);
    }
}

#endif
