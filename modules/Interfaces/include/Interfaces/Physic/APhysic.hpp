///
/// @file APhysic.hpp
/// @brief This file contains the physic abstract class
/// @namespace cae
///

#pragma once

#include "Interfaces/Physic/IPhysic.hpp"

namespace cae
{

    ///
    /// @interface APhysic
    /// @brief Abstract class for physic
    /// @namespace cae
    ///
    class APhysic : public IPhysic
    {

        public:
            ~APhysic() override = default;

    }; // interface APhysic

} // namespace cae
