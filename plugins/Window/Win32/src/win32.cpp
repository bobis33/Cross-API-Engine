#include <string>

#include "Utils/Logger.hpp"

#include "Win32/Win32.hpp"

constexpr wchar_t WINDOW_CLASS_NAME[] = L"CAE_WindowsWindowClass";

LRESULT CALLBACK cae::Win32::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Win32 *self = nullptr;

    if (msg == WM_NCCREATE)
    {
        auto *cs = reinterpret_cast<CREATESTRUCTW *>(lParam);
        self = static_cast<Win32 *>(cs->lpCreateParams);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
        return TRUE;
    }

    self = reinterpret_cast<Win32 *>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));

    switch (msg)
    {
        case WM_SIZE:
            if (self != nullptr)
            {
                self->m_frameBufferResized = true;
                self->m_frameBufferSize = {.width=LOWORD(lParam), .height=HIWORD(lParam)};
            }
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
}

bool cae::Win32::create(const std::string &name, WindowSize size)
{
    m_hInstance = GetModuleHandleW(nullptr);
    m_frameBufferSize = size;
    m_shouldClose = false;
    m_frameBufferResized = false;

    const int len = MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, nullptr, 0);

    m_title.resize(len);

    MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, m_title.data(), len);

    if (!m_title.empty() && m_title.back() == L'\0')
    {
        m_title.pop_back();
    }

    static bool classRegistered = false;
    if (!classRegistered)
    {
        WNDCLASSW wc{};
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = m_hInstance;
        wc.lpszClassName = WINDOW_CLASS_NAME;
        wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        wc.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
        wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);

        if (RegisterClassW(&wc) == 0U)
        {
            utl::Logger::log("Failed to register Win32 window class", utl::LogLevel::WARNING);
            return false;
        }
        classRegistered = true;
    }
    m_hwnd = CreateWindowExW(0, WINDOW_CLASS_NAME, L"TEST TITLE VISIBLE", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                             CW_USEDEFAULT, size.width, size.height, nullptr, nullptr, m_hInstance, this);

    if (m_hwnd == nullptr) {
        return false;
    }

    SetWindowTextW(m_hwnd, m_title.c_str());

    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);
    return true;
}

void cae::Win32::close()
{
    if (m_hwnd != nullptr)
    {
        DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
    }
    UnregisterClassW(WINDOW_CLASS_NAME, m_hInstance);
}

cae::NativeWindowHandle cae::Win32::getNativeHandle() const { return {.window = m_hwnd, .display = m_hInstance}; }

cae::WindowSize cae::Win32::getWindowSize() const
{
    RECT rect{};
    GetClientRect(m_hwnd, &rect);
    return {.width = static_cast<uint16_t>(rect.right - rect.left),
            .height = static_cast<uint16_t>(rect.bottom - rect.top)};
}

bool cae::Win32::setIcon(const std::string &path) const { return false; }

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
