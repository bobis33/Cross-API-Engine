///
/// @file Mouse.hpp
/// @brief This file contains the gamepad keys
/// @namespace cae
///

#pragma once

#include <cstdint>

namespace cae
{
    enum class GamepadButton : uint8_t
    {
        A = 0,
        B,
        X,
        Y,
        Back,
        Guide,
        Start,
        LThumb,
        RThumb,
        LShoulder,
        RShoulder,
        DPadUp,
        DPadDown,
        DPadLeft,
        DPadRight
    };

    enum class GamepadAxis : uint8_t
    {
        LeftX = 0,
        LeftY,
        RightX,
        RightY,
        TriggerLeft,
        TriggerRight
    };
} // namespace cae
