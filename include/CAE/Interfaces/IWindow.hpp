///
/// @file IWindow.hpp
/// @brief This file contains the Window interface
/// @namespace CAE
///

#pragma once

#include "CAE/Interfaces/IPlugin.hpp"

namespace CAE {

    ///
    /// @interface IWindow
    /// @brief Interface for window
    /// @namespace CAE
    ///
    class IWindow : public IPlugin {

        public:
            ~IWindow() override = default;

    }; // interface IWindow

} // namespace CAE
