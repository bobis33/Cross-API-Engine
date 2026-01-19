#include "Win32/Win32.hpp"

#include "Utils/Image.hpp"
#include "Utils/Logger.hpp"

#include <windowsx.h>

#include <cstring>
#include <unordered_map>

constexpr wchar_t WINDOW_CLASS_NAME[] = L"CAE_WindowsWindowClass";

cae::KeyCode cae::Win32::mapWinKey(const WPARAM key)
{
    static const std::unordered_map<WPARAM, KeyCode> keyMap = {
        {'A', KeyCode::A},
        {'B', KeyCode::B},
        {'C', KeyCode::C},
        {'D', KeyCode::D},
        {'E', KeyCode::E},
        {'F', KeyCode::F},
        {'G', KeyCode::G},
        {'H', KeyCode::H},
        {'I', KeyCode::I},
        {'J', KeyCode::J},
        {'K', KeyCode::K},
        {'L', KeyCode::L},
        {'M', KeyCode::M},
        {'N', KeyCode::N},
        {'O', KeyCode::O},
        {'P', KeyCode::P},
        {'Q', KeyCode::Q},
        {'R', KeyCode::R},
        {'S', KeyCode::S},
        {'T', KeyCode::T},
        {'U', KeyCode::U},
        {'V', KeyCode::V},
        {'W', KeyCode::W},
        {'X', KeyCode::X},
        {'Y', KeyCode::Y},
        {'Z', KeyCode::Z},

        {'0', KeyCode::Num0},
        {'1', KeyCode::Num1},
        {'2', KeyCode::Num2},
        {'3', KeyCode::Num3},
        {'4', KeyCode::Num4},
        {'5', KeyCode::Num5},
        {'6', KeyCode::Num6},
        {'7', KeyCode::Num7},
        {'8', KeyCode::Num8},
        {'9', KeyCode::Num9},

        {VK_ESCAPE, KeyCode::Escape},
        {VK_LEFT, KeyCode::Left},
        {VK_RIGHT, KeyCode::Right},
        {VK_UP, KeyCode::Up},
        {VK_DOWN, KeyCode::Down},
        {VK_SPACE, KeyCode::Space},
        {VK_RETURN, KeyCode::Enter},
        {VK_BACK, KeyCode::Backspace},
        {VK_TAB, KeyCode::Tab},
        {VK_LSHIFT, KeyCode::LShift},
        {VK_RSHIFT, KeyCode::RShift},
        {VK_LCONTROL, KeyCode::LCtrl},
        {VK_RCONTROL, KeyCode::RCtrl},
        {VK_LMENU, KeyCode::LAlt},
        {VK_RMENU, KeyCode::RAlt}
        // ...
    };

    const auto it = keyMap.find(key);
    return it != keyMap.end() ? it->second : KeyCode::Count;
}

LRESULT CALLBACK cae::Win32::WindowProc(const HWND hwnd, const UINT msg, const WPARAM wParam, const LPARAM lParam)
{
    Win32 *self = nullptr;

    if (msg == WM_NCCREATE)
    {
        const auto *cs = reinterpret_cast<CREATESTRUCTW *>(lParam);
        self = static_cast<Win32 *>(cs->lpCreateParams);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
        return TRUE;
    }

    self = reinterpret_cast<Win32 *>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    if (!self)
        return DefWindowProcW(hwnd, msg, wParam, lParam);

    WindowEvent e{};
    switch (msg)
    {
        case WM_SIZE:
            self->m_frameBufferResized = true;
            self->m_frameBufferSize = {.width = LOWORD(lParam), .height = HIWORD(lParam)};
            e.type = WindowEventType::Resize;
            e.resize = {.w=LOWORD(lParam), .h=HIWORD(lParam)};
            self->m_eventQueue.push(e);
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            e.type = WindowEventType::Close;
            self->m_eventQueue.push(e);
            return 0;

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            e.type = WindowEventType::KeyDown;
            e.key.key = mapWinKey(wParam);
            self->m_eventQueue.push(e);
            return 0;

        case WM_KEYUP:
        case WM_SYSKEYUP:
            e.type = WindowEventType::KeyUp;
            e.key.key = mapWinKey(wParam);
            self->m_eventQueue.push(e);
            return 0;

            // mouse, scroll, ...
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
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
    m_hwnd = CreateWindowExW(0, WINDOW_CLASS_NAME, L"", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, size.width,
                             size.height, nullptr, nullptr, m_hInstance, this);

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

void cae::Win32::setIcon(const std::string &path) const
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
            utl::Logger::log("Failed to create window icon", utl::LogLevel::WARNING);
            return;
        }

        std::memcpy(pBits, image.pixels, static_cast<size_t>(image.width * image.height * 4));

        iconInfo.hbmColor = hBitmap;
        iconInfo.hbmMask = CreateBitmap(image.width, image.height, 1, 1, nullptr);

        HICON hIcon = CreateIconIndirect(&iconInfo);

        DeleteObject(hBitmap);
        DeleteObject(iconInfo.hbmMask);

        if (hIcon == nullptr)
        {
            utl::Logger::log("Failed to create window icon", utl::LogLevel::WARNING);
            return;
        }

        SendMessageW(m_hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIcon));
        SendMessageW(m_hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIcon));

    }
    catch (const std::exception &e)
    {
        utl::Logger::log("Failed to load icon: " + std::string(e.what()), utl::LogLevel::WARNING);
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

bool cae::Win32::pollEvent(WindowEvent &event)
{
    MSG msg{};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (!m_eventQueue.empty())
    {
        event = m_eventQueue.front();
        m_eventQueue.pop();
        return true;
    }

    return false;
}
