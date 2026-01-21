///
/// @file ANetwork.hpp
/// @brief This file contains the network abstract class
/// @namespace cae
///

#pragma once

#include "Interfaces/Network/INetwork.hpp"

namespace cae
{

    ///
    /// @interface ANetwork
    /// @brief Abstract class for network
    /// @namespace cae
    ///
    class ANetwork : public INetwork
    {

        public:
            ~ANetwork() override = default;

    }; // interface ANetwork

} // namespace cae
