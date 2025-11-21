///
/// @file IRenderDevice.hpp
/// @brief This file contains the render device interface
/// @namespace cae
///

#pragma once

#include <Utils/Interfaces/IPlugin.hpp>

namespace cae
{

    ///
    /// @interface IRenderDevice
    /// @brief Interface for render device
    /// @namespace cae
    ///
    class IRenderDevice : public utl::IPlugin
    {

        public:
            ~IRenderDevice() override = default;

    }; // interface IRenderDevice

} // namespace cae
