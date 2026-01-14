#if defined(__APPLE__)

#include "OPGL/Context/NSGLContextMac.hpp"
#include "OPGL/glad/glad.h"

#import <AppKit/AppKit.h>

cae::NSGLContextMac::~NSGLContextMac() {
    if (m_context) {
        [(NSOpenGLContext*)m_context clearDrawable];
        m_context = nil;
    }
}

void cae::NSGLContextMac::initialize(const NativeWindowHandle &window) {
    NSView* nsview = (__bridge NSView*)window.window;

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

    if (!gladLoadGL()) {
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