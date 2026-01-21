///
/// @file Gamepad.hpp
/// @brief This file contains the gamepad keys
/// @namespace cae
///

#pragma once

#include <cstdint>

namespace cae
{
    enum class GamepadButton : std::uint8_t
    {
        Unknown = 0,
        A,
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
        DPadRight,

        Count
    };

    enum class GamepadAxis : std::uint8_t
    {
        Unknown = 0,
        LeftX,
        LeftY,
        RightX,
        RightY,

        TriggerLeft,
        TriggerRight,

        Count
    };

} // namespace cae
