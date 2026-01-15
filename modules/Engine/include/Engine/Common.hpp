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
    namespace Audio
    {
        inline constexpr auto VOLUME = 1.F;
        inline constexpr auto MUTED = false;
    } // namespace Audio

    namespace Network
    {
        inline constexpr auto HOST = "127.0.0.1";
        inline constexpr auto PORT = 4242;
    } // namespace Network

    namespace Plugins::Name
    {
        inline constexpr auto RENDERER_OPENGL = "OpenGL";
        inline constexpr auto RENDERER_VULKAN = "Vulkan";
        inline constexpr auto WINDOW_GLFW = "GLFW";
    } // namespace Plugins::Name

    namespace Renderer
    {
        inline constexpr auto VSYNC = false;
        inline constexpr auto FRAME_RATE_LIMIT = 90;
    } // namespace Renderer

    namespace User
    {
        inline constexpr auto NAME = "User";
    } // namespace User

    namespace Window
    {
        inline constexpr uint16_t WIDTH = 1920;
        inline constexpr uint16_t HEIGHT = 1080;
        inline constexpr auto NAME = "CAE - Cross API Engine";
        inline constexpr auto FULLSCREEN = false;
    } // namespace Window
} // namespace cae
