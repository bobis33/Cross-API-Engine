///
/// @file AUI.hpp
/// @brief This file contains the ui abstract class
/// @namespace cae
///

#pragma once

#include "Interfaces/UI/IUI.hpp"

namespace cae
{

    ///
    /// @interface AUI
    /// @brief Abstract class for ui
    /// @namespace cae
    ///
    class AUI : public IUI
    {

        public:
            ~AUI() override = default;

    }; // interface AUI

} // namespace cae
