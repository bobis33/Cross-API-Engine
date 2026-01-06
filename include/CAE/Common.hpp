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
} // namespace cae
