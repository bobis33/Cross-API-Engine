///
/// @file Image.hpp
/// @brief This file contains image struct
/// @namespace cae::utl
///

#pragma once

#include <filesystem>

namespace cae::utl
{

    ///
    /// @struct Image
    /// @brief Struct for image
    /// @namespace cae::utl
    ///
    struct Image
    {

            using pixel = unsigned char *;

            ///
            /// @param path Path to the image file
            /// @param flip Whether to flip the image vertically
            /// @brief Load an image from a file
            ///
            explicit Image(const std::filesystem::path &path, bool flip = false);
            ~Image();

            pixel pixels = nullptr;
            int width = 0;
            int height = 0;
            int channels = 0;

    }; // struct Image

} // namespace cae::utl
