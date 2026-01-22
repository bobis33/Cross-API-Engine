///
/// @file File.hpp
/// @brief This file contains file utility functions
/// @namespace cae::utl
///

#pragma once

#include <filesystem>
#include <vector>

namespace cae::utl
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

} // namespace cae::utl
