///
/// @file IPhysic.hpp
/// @brief This file contains the physic interface
/// @namespace cae
///

#pragma once

#include <Utils/Interfaces/IPlugin.hpp>

namespace cae
{

    ///
    /// @interface IPhysic
    /// @brief Interface for physics
    /// @namespace cae
    ///
    class IPhysic : public utl::IPlugin
    {

        public:
            ~IPhysic() override = default;

    }; // interface IPhysic

} // namespace cae
