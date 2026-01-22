#ifdef __APPLE__

#include "OPGL/Context/NSGLContext.hpp"

#include "Utils/Logger.hpp"

#import <AppKit/AppKit.h>

cae::NSGLContext::~NSGLContext() {
    if (m_context) {
        [(NSOpenGLContext*)m_context clearDrawable];
        m_context = nil;
    }
}

void cae::NSGLContext::initialize(const NativeWindowHandle &window) {
    NSView* nsview = (__bridge NSView*)window.display;

    NSOpenGLPixelFormatAttribute attrs[] = {
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
        NSOpenGLPFAColorSize, 24,
        NSOpenGLPFADepthSize, 24,
        NSOpenGLPFAAccelerated,
        NSOpenGLPFADoubleBuffer,
        0
    };

    NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    if (!pixelFormat) throw std::runtime_error("Failed to create NSOpenGLPixelFormat");

    m_context = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:nil];
    [(NSOpenGLContext*)m_context setView:nsview];
    [(NSOpenGLContext*)m_context makeCurrentContext];

    if (!gladLoadGLContext(&gl, nullptr)) {
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

void cae::NSGLContext::swapBuffers() {
    if (m_context)
        [(NSOpenGLContext*)m_context flushBuffer];
}

void cae::NSGLContext::setVSyncEnabled(const bool enabled) {
    if (m_context) {
        GLint sync = enabled ? 1 : 0;
        [(NSOpenGLContext*)m_context setValues:&sync forParameter:NSOpenGLContextParameterSwapInterval];
    }
}

bool cae::NSGLContext::isVSyncEnabled() const {
    if (m_context) {
        GLint sync = 0;
        [(NSOpenGLContext*)m_context getValues:&sync forParameter:NSOpenGLContextParameterSwapInterval];
        return sync != 0;
    }
    return false;
}

#endif