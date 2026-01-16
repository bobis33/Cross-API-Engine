///
/// @file Common.hpp
/// @brief This file contains the common definitions.
/// @namespace cae
///

#pragma once

#include "CAE/Generated/Version.hpp"

#include "Engine/Common.hpp"

namespace cae
{
    namespace MESSAGE
    {
        static constexpr std::string_view HELP_MSG = "Usage: " PROJECT_NAME APP_EXTENSION " [options]\n\n"
                                                     "Options:\n"
                                                     "  -h, --help              Show this help message\n"
                                                     "  -v, --version           Show version information\n"
                                                     "  -c, --config <path>     Specify JSON configuration file";
        static constexpr std::string_view VERSION_MSG = PROJECT_NAME
            " v" PROJECT_VERSION " " BUILD_TYPE " (" GIT_TAG ", commit " GIT_COMMIT_HASH ") " __DATE__ " " __TIME__;
    } // namespace MESSAGE

    namespace PLUGINS::NAME
    {
        namespace RENDERER
        {
            inline constexpr auto OPENGL = "OpenGL";
            inline constexpr auto VULKAN = "Vulkan";
        } // namespace RENDERER

        namespace SHADER
        {
            namespace IR
            {
                inline constexpr auto SPIRV = "SPIRV";
            } // namespace IR

            namespace FRONTEND
            {
                inline constexpr auto GLSL = "GLSL";
            } // namespace FRONTEND

        } // namespace SHADER

        namespace WINDOW
        {
            inline constexpr auto GLFW = "GLFW";
            inline constexpr auto WIN32_ = "Win32";
            inline constexpr auto X11 = "X11";
        } // namespace WINDOW

    } // namespace PLUGINS::NAME

    namespace USER
    {
        inline constexpr auto NAME = "User";
    } // namespace USER

} // namespace cae
