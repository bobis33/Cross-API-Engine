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
    namespace Message
    {
        static constexpr std::string_view HELP_MSG = "Usage: " PROJECT_NAME APP_EXTENSION " [options]\n\n"
                                                     "Options:\n"
                                                     "  -h, --help              Show this help message\n"
                                                     "  -v, --version           Show version information\n"
                                                     "  -c, --config <path>     Specify JSON configuration file";
        static constexpr std::string_view VERSION_MSG = PROJECT_NAME
            " v" PROJECT_VERSION " " BUILD_TYPE " (" GIT_TAG ", commit " GIT_COMMIT_HASH ") " __DATE__ " " __TIME__;
    } // namespace Message

    namespace Plugins::Name
    {
        namespace Render
        {
            inline constexpr auto OPENGL = "OpenGL";
            inline constexpr auto VULKAN = "Vulkan";
        } // namespace Render

        namespace Shader
        {
            namespace IR
            {
                inline constexpr auto SPIRV = "SPIRV";
            } // namespace IR

            namespace Frontend
            {
                inline constexpr auto GLSL = "GLSL";
            } // namespace Frontend

        } // namespace Shader

        namespace Window
        {
            inline constexpr auto GLFW = "GLFW";
            inline constexpr auto WIN32 = "Win32";
            inline constexpr auto X11 = "X11";
        } // namespace Window

    } // namespace Plugins::Name

    namespace User
    {
        inline constexpr auto NAME = "User";
    } // namespace User

} // namespace cae
