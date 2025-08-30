///
/// @file IWindow.hpp
/// @brief This file contains the Window interface
/// @namespace cae
///

#pragma once

#include "CAE/Interfaces/IPlugin.hpp"

namespace cae
{

    ///
    /// @interface IWindow
    /// @brief Interface for window
    /// @namespace cae
    ///
    class IWindow : public IPlugin {

        public:
            ~IWindow() override = default;

    }; // interface IWindow

} // namespace cae
