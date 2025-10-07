///
/// @file Common.hpp
/// @brief This file contains
/// @namespace cae
///

#pragma once

namespace cae
{
    namespace Audio
    {
        inline constexpr auto VOLUME = 50.F;
        inline constexpr auto MUTED = false;
    } // namespace Audio
    namespace Network
    {
        inline constexpr auto HOST = "127.0.0.1";
        inline constexpr auto PORT = 4242;
    } // namespace Network
    namespace User
    {
        inline constexpr auto NAME = "User";
    }
    namespace Window
    {
        inline constexpr auto HEIGHT = 1920;
        inline constexpr auto WIDTH = 1080;
        inline constexpr auto NAME = "CAE - Cross API Engine";
        inline constexpr auto FULLSCREEN = false;
        inline constexpr auto VSYNC = false;
        inline constexpr auto MAX_FPS = 90;
    } // namespace Window
} // namespace cae