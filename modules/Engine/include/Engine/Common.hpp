///
/// @file Common.hpp
/// @brief This file contains common constants used across the engine
/// @namespace cae
///

#pragma once

#include <cstdint>

#ifdef _WIN32
#define APP_EXTENSION ".exe"
#else
#define APP_EXTENSION ""
#endif

namespace cae
{
    namespace AUDIO
    {
        inline constexpr auto VOLUME = 1.F;
        inline constexpr auto MUTED = false;
    } // namespace AUDIO

    namespace CAMERA
    {
        inline constexpr auto NAME = "Default name";
        inline constexpr auto MOVE_SPEED = 2.5F;
        inline constexpr auto LOOK_SPEED = 100.F;
        inline constexpr auto FOV = 45.F;
        inline constexpr auto NEAR_PLANE = 0.1F;
        inline constexpr auto FAR_PLANE = 100.F;
    } // namespace CAMERA

    namespace NETWORK
    {
        inline constexpr auto HOST = "127.0.0.1";
        inline constexpr auto PORT = 4242;
    } // namespace NETWORK

    namespace RENDERER
    {
        inline constexpr auto VSYNC = false;
        inline constexpr auto FRAME_RATE_LIMIT = 90;
        inline constexpr auto CLEAR_COLOR_R = 1.0F;
        inline constexpr auto CLEAR_COLOR_G = 1.0F;
        inline constexpr auto CLEAR_COLOR_B = 1.0F;
        inline constexpr auto CLEAR_COLOR_A = 1.0F;
    } // namespace RENDERER

    namespace WINDOW
    {
        inline constexpr uint16_t WIDTH = 960;
        inline constexpr uint16_t HEIGHT = 540;
        inline constexpr auto NAME = "Default name";
        inline constexpr auto FULLSCREEN = false;
        inline constexpr auto ICON_PATH = "";
    } // namespace WINDOW

} // namespace cae
