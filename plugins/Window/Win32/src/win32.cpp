#include <string>

#include <Utils/Logger.hpp>

#include "Win32/Win32.hpp"

LRESULT CALLBACK cae::Win32::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    auto* self = reinterpret_cast<Win32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (msg)
    {
        case WM_CLOSE:
            if (self) self->m_shouldClose = true;
            return 0;
        case WM_SIZE:
            if (self)
            {
                self->m_frameBufferResized = true;
                self->m_frameBufferSize.width = LOWORD(lParam);
                self->m_frameBufferSize.height = HIWORD(lParam);
            }
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

bool cae::Win32::create(const std::string &name, WindowSize size)
{
    m_hInstance = GetModuleHandle(nullptr);
    m_frameBufferSize = size;
    m_shouldClose = false;
    m_frameBufferResized = false;

    WNDCLASSW wc{};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = m_hInstance;
    wc.lpszClassName = L"CAE_WindowsWindowClass";
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);

    if (!RegisterClassW(&wc) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
    {
        utl::Logger::log("Failed to register window class", utl::LogLevel::WARNING);
        return false;
    }

    m_hwnd = CreateWindowEx(
        0,
        reinterpret_cast<LPCSTR>(L"CAE_WindowsWindowClass"),
        reinterpret_cast<LPCSTR>(std::wstring(name.begin(), name.end()).c_str()),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        size.width, size.height,
        nullptr,
        nullptr,
        m_hInstance,
        nullptr
    );

    if (!m_hwnd)
    {
        utl::Logger::log("Failed to create Win32 window", utl::LogLevel::WARNING);
        return false;
    }

    SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);

    return true;
}

void cae::Win32::close()
{
    if (m_hwnd)
    {
        DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
    }
    UnregisterClass(reinterpret_cast<LPCSTR>(L"CAE_WindowsWindowClass"), m_hInstance);
}

 cae::NativeWindowHandle cae::Win32::getNativeHandle() const
{
    return {.window = m_hwnd, .display = m_hInstance};

}

 cae::WindowSize cae::Win32::getWindowSize() const
{
    RECT rect{};
    GetClientRect(m_hwnd, &rect);
    return {.width = static_cast<uint16_t>(rect.right - rect.left),
            .height = static_cast<uint16_t>(rect.bottom - rect.top)};
}

 bool cae::Win32::setIcon(const std::string &path) const
{
    return false;
}

void cae::Win32::pollEvents()
{
    MSG msg{};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            m_shouldClose = true;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
