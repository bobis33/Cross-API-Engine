///
/// @file Utils.hpp
/// @brief This file contains utility functions
/// @namespace utl
///

#pragma once

#include <filesystem>
#include <vector>

namespace utl
{
    [[nodiscard]] std::vector<char> fileToVector(const std::string &filename);
    [[nodiscard]] std::string fileToString(const std::filesystem::path& path);
} // namespace utl
