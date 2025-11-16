///
/// @file IGamepad.hpp
/// @brief This file contains the input gamepad interface
/// @namespace cae
///

#pragma once

#include <Utils/Interfaces/IPlugin.hpp>

namespace cae
{

    ///
    /// @interface IGamepad
    /// @brief Interface for gamepad
    /// @namespace cae
    ///
    class IGamepad : public utl::IPlugin
    {

        public:
            ~IGamepad() override = default;

    }; // interface IGamepad

} // namespace cae
