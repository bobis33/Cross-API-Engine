///
/// @file Env.hpp
/// @brief This file contains env utility functions
/// @namespace utl
///

#pragma once

#include <string>
#include <unordered_map>

namespace utl
{
    [[nodiscard]] std::unordered_map<std::string, std::string> getEnvMap(const char *const *env);
} // namespace utl
