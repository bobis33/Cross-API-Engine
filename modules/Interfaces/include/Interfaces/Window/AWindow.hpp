///
/// @file AWindow.hpp
/// @brief This file contains the Window abstract class
/// @namespace cae
///

#pragma once

#include "Interfaces/Window/IWindow.hpp"

namespace cae
{

    ///
    /// @interface IWindow
    /// @brief Abstract class for window
    /// @namespace cae
    ///
    class AWindow : public IWindow
    {

        public:
            ~AWindow() override = default;

    }; // interface AWindow

} // namespace cae
