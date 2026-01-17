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
    ///
    /// @param env Pointer to environment variables
    /// @return A map of environment variables
    /// @brief Get environment variables as a map
    ///
    [[nodiscard]] std::unordered_map<std::string, std::string> getEnvMap(const char *const *env);
} // namespace utl
