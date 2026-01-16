#include "Win32/Win32.hpp"

#include "Utils/Image.hpp"
#include "Utils/Logger.hpp"

#include <cstring>
#include <filesystem>
#include <string>

constexpr wchar_t WINDOW_CLASS_NAME[] = L"CAE_WindowsWindowClass";

LRESULT CALLBACK cae::Win32::WindowProc(const HWND hwnd, const UINT msg, const WPARAM wParam, const LPARAM lParam)
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
                self->m_frameBufferSize = {.width = LOWORD(lParam), .height = HIWORD(lParam)};
            }
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
}

bool cae::Win32::create(const std::string &name, const WindowSize size)
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

    if (m_hwnd == nullptr)
    {
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

bool cae::Win32::setIcon(const std::string &path) const
{
    try
    {
        const utl::Image image(path);

        for (size_t i = 0; i < static_cast<size_t>(image.width * image.height); ++i)
        {
            std::swap(image.pixels[(i * 4) + 0], image.pixels[(i * 4) + 2]);
        }

        ICONINFO iconInfo{};
        iconInfo.fIcon = TRUE;

        BITMAPV5HEADER bi{};
        bi.bV5Size = sizeof(BITMAPV5HEADER);
        bi.bV5Width = image.width;
        bi.bV5Height = -static_cast<LONG>(image.height);
        bi.bV5Planes = 1;
        bi.bV5BitCount = 32;
        bi.bV5Compression = BI_BITFIELDS;
        bi.bV5RedMask = 0x00FF0000;
        bi.bV5GreenMask = 0x0000FF00;
        bi.bV5BlueMask = 0x000000FF;
        bi.bV5AlphaMask = 0xFF000000;

        void *pBits = nullptr;
        const HDC hdc = GetDC(nullptr);
        const HBITMAP hBitmap =
            CreateDIBSection(hdc, reinterpret_cast<BITMAPINFO *>(&bi), DIB_RGB_COLORS, &pBits, nullptr, 0);
        ReleaseDC(nullptr, hdc);

        if (hBitmap == nullptr)
        {
            return false;
        }

        std::memcpy(pBits, image.pixels, static_cast<size_t>(image.width * image.height * 4));

        iconInfo.hbmColor = hBitmap;
        iconInfo.hbmMask = CreateBitmap(image.width, image.height, 1, 1, nullptr);

        HICON hIcon = CreateIconIndirect(&iconInfo);

        DeleteObject(hBitmap);
        DeleteObject(iconInfo.hbmMask);

        if (hIcon == nullptr)
        {
            return false;
        }

        SendMessageW(m_hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIcon));
        SendMessageW(m_hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIcon));

        return true;
    }
    catch (const std::exception &e)
    {
        utl::Logger::log("Failed to load icon: " + std::string(e.what()), utl::LogLevel::WARNING);
        return false;
    }
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
