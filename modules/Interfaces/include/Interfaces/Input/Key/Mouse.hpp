///
/// @file Mouse.hpp
/// @brief This file contains the mouse keys
/// @namespace cae
///

#pragma once

#include <cstdint>

namespace cae
{
        enum class MouseButton : std::uint8_t
        {
            Unknown = 0,
            Left,
            Right,
            Middle,
            XButton1,
            XButton2,

            Count
        };

        enum class MouseAxis : std::uint8_t
        {
            Unknown = 0,
            X,
            Y,
            Wheel,

            Count
        };

} // namespace cae
