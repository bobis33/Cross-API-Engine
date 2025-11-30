///
/// @file IUI.hpp
/// @brief This file contains the ui interface
/// @namespace cae
///

#pragma once

#include "Utils/Interfaces/IPlugin.hpp"

namespace cae
{

    ///
    /// @interface IUI
    /// @brief Interface for ui
    /// @namespace cae
    ///
    class IUI : public utl::IPlugin
    {

        public:
            ~IUI() override = default;

    }; // interface IUI

} // namespace cae
