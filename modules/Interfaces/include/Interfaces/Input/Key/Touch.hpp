///
/// @file Touch.hpp
/// @brief This file contains the touch keys (mobile, web)
/// @namespace cae
///

#pragma once

#include <cstdint>

namespace cae
{
    enum class TouchPhase
    {
        Began,
        Moved,
        Ended,
        Cancelled
    };

    struct TouchPoint
    {
        std::uint32_t id;
        float x;
        float y;
        TouchPhase phase;
    };

} // namespace cae
