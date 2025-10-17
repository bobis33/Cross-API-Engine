///
/// @file Mouse.hpp
/// @brief This file contains the mouse keys
/// @namespace cae
///

#pragma once

#include <cstdint>

namespace cae
{
    enum class MouseButton : uint8_t
    {
        Left = 0,
        Right,
        Middle,
        XButton1,
        XButton2,
        WheelUp,
        WheelDown
    };
} // namespace cae
