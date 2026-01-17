///
/// @file File.hpp
/// @brief This file contains file utility functions
/// @namespace utl
///

#pragma once

#include <filesystem>
#include <vector>

namespace utl
{
    ///
    /// @param path Path to the file
    /// @return A vector of chars containing the file contents
    /// @brief Read a file and return its contents as a vector of chars
    ///
    [[nodiscard]] std::vector<char> fileToVector(const std::filesystem::path &path);

    ///
    /// @param path Path to the file
    /// @return A string containing the file contents
    /// @brief Read a file and return its contents as a string
    ///
    [[nodiscard]] std::string fileToString(const std::filesystem::path &path);
} // namespace utl
