///
/// @file IMouse.hpp
/// @brief This file contains the input mouse interface
/// @namespace cae
///

#pragma once

#include <Utils/Interfaces/IPlugin.hpp>

namespace cae
{

    ///
    /// @interface IMouse
    /// @brief Interface for mouse
    /// @namespace cae
    ///
    class IMouse : public utl::IPlugin
    {

        public:
            ~IMouse() override = default;

    }; // interface IMouse

} // namespace cae
