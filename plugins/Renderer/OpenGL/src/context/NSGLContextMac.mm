#if defined(__APPLE__)

#include "OPGL/Context/NSGLContextMac.hpp"

#import <AppKit/AppKit.h>

cae::NSGLContextMac::~NSGLContextMac() {
    if (m_context) {
        [(NSOpenGLContext*)m_context clearDrawable];
        m_context = nil;
    }
}

void cae::NSGLContextMac::initialize(const NativeWindowHandle &window)
{
    NSWindow* cocoaWindow = (__bridge NSWindow*)window.window;
    NSView* contentView = [cocoaWindow contentView];

    m_context = [contentView openGLContext];
    if (!m_context) {
        throw std::runtime_error("No context");
    }

    [m_context makeCurrentContext];

    if (!gladLoadGLContext(&gl, nullptr)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }
}

void cae::NSGLContextMac::swapBuffers() {
    if (m_context)
        [(NSOpenGLContext*)m_context flushBuffer];
}

void cae::NSGLContextMac::setVSyncEnabled(const bool enabled) {
    if (m_context) {
        GLint sync = enabled ? 1 : 0;
        [(NSOpenGLContext*)m_context setValues:&sync forParameter:NSOpenGLContextParameterSwapInterval];
    }
}

bool cae::NSGLContextMac::isVSyncEnabled() const {
    if (m_context) {
        GLint sync = 0;
        [(NSOpenGLContext*)m_context getValues:&sync forParameter:NSOpenGLContextParameterSwapInterval];
        return sync != 0;
    }
    return false;
}

#endif // defined(__APPLE__)