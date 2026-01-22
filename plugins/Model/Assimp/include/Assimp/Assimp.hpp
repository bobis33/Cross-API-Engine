///
/// @file Assimp.hpp
/// @brief This file contains the Assimp class declaration
/// @namespace cae
///

#pragma once

#include "Interfaces/Model/AModel.hpp"

namespace cae
{

    ///
    /// @class Assimp
    /// @brief Class for the Assimp plugin
    /// @namespace cae
    ///
    class Assimp final : public AModel
    {

        public:
            Assimp() = default;
            ~Assimp() override = default;

            Assimp(const Assimp &) = delete;
            Assimp &operator=(const Assimp &) = delete;
            Assimp(Assimp &&) = delete;
            Assimp &operator=(Assimp &&) = delete;

            [[nodiscard]] std::string getName() const override { return "Assimp"; }
            [[nodiscard]] utl::PluginType getType() const override { return utl::PluginType::MODEL; }
            [[nodiscard]] utl::PluginPlatform getPlatform() const override { return utl::PluginPlatform::ALL; }

    }; // class Assimp

} // namespace cae