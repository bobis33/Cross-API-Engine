#import <Cocoa/Cocoa.h>

#include "Cocoa/Cocoa.hpp"

#include <queue>

using namespace cae;

Cocoa::~Cocoa()
{
    close();
}

bool Cocoa::create(const std::string& title, WindowSize size)
{
    @autoreleasepool
    {
        m_app = [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

        NSRect rect = NSMakeRect(0, 0, size.width, size.height);
        NSUInteger style =
            NSWindowStyleMaskTitled |
            NSWindowStyleMaskClosable |
            NSWindowStyleMaskResizable;

        NSWindow* window = [[NSWindow alloc]
            initWithContentRect:rect
                      styleMask:style
                        backing:NSBackingStoreBuffered
                          defer:NO];

        [window setTitle:[NSString stringWithUTF8String:title.c_str()]];
        [window makeKeyAndOrderFront:nil];
        [NSApp activateIgnoringOtherApps:YES];

        m_window = window;
        m_view = [window contentView];
        m_size = size;
    }

    return true;
}

void Cocoa::close()
{
    if (m_window)
    {
        NSWindow* window = (NSWindow*)m_window;
        [window close];
        m_window = nullptr;
        m_view = nullptr;
    }
}

void Cocoa::pollEvents()
{
    @autoreleasepool
    {
        NSEvent* event;
        while ((event = [NSApp
            nextEventMatchingMask:NSEventMaskAny
            untilDate:[NSDate distantPast]
            inMode:NSDefaultRunLoopMode
            dequeue:YES]))
        {
            switch ([event type])
            {
                case NSEventTypeKeyDown:
                case NSEventTypeKeyUp:
                {
                    WindowEvent e{};
                    e.type = ([event type] == NSEventTypeKeyDown)
                        ? WindowEventType::KeyDown
                        : WindowEventType::KeyUp;
                    // mapping clavier à faire ici
                    m_eventQueue.push(e);
                    break;
                }

                case NSEventTypeLeftMouseDown:
                case NSEventTypeLeftMouseUp:
                {
                    WindowEvent e{};
                    e.type = ([event type] == NSEventTypeLeftMouseDown)
                        ? WindowEventType::MouseButtonDown
                        : WindowEventType::MouseButtonUp;
                    e.mouseButton.button = MouseButton::Left;
                    m_eventQueue.push(e);
                    break;
                }

                default:
                    break;
            }

            [NSApp sendEvent:event];
        }
    }
}

bool Cocoa::pollEvent(WindowEvent& event)
{
    if (m_eventQueue.empty())
        return false;

    event = m_eventQueue.front();
    m_eventQueue.pop();
    return true;
}

bool Cocoa::shouldClose() const
{
    return m_shouldClose;
}

WindowSize Cocoa::getWindowSize() const
{
    if (!m_window)
        return {};

    NSWindow* window = (NSWindow*)m_window;
    NSRect rect = [window frame];
    return {
        .width  = (uint16_t)rect.size.width,
        .height = (uint16_t)rect.size.height
    };
}

NativeWindowHandle Cocoa::getNativeHandle() const
{
    NativeWindowHandle handle{};
    handle.window = m_window; // NSWindow*
    handle.display = m_view;  // NSView*
    return handle;
}
