///
/// @file Keyboard.hpp
/// @brief This file contains the keyboard keys
/// @namespace cae
///

#pragma once

#include <cstdint>

namespace cae
{

    enum KeyState : std::uint8_t
    {
        Pressed = 0,
        Released = 1,
        Held = 2,
        Toggled = 3,
    };

    enum class KeyCode : uint8_t
    {
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,

        Num0,
        Num1,
        Num2,
        Num3,
        Num4,
        Num5,
        Num6,
        Num7,
        Num8,
        Num9,

        Escape,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,

        Left,
        Right,
        Up,
        Down,
        Home,
        End,
        PageUp,
        PageDown,
        Insert,
        Delete,
        Backspace,
        Tab,
        Enter,
        Space,

        LShift,
        RShift,
        LCtrl,
        RCtrl,
        LAlt,
        RAlt,
        LSuper,
        RSuper,

        Numpad0,
        Numpad1,
        Numpad2,
        Numpad3,
        Numpad4,
        Numpad5,
        Numpad6,
        Numpad7,
        Numpad8,
        Numpad9,
        NumpadAdd,
        NumpadSubtract,
        NumpadMultiply,
        NumpadDivide,

        CapsLock,
        NumLock,
        ScrollLock,

        Count
    };

} // namespace cae
