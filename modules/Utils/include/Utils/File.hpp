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
    [[nodiscard]] std::vector<char> fileToVector(const std::filesystem::path &path);
    [[nodiscard]] std::string fileToString(const std::filesystem::path &path);
} // namespace utl
