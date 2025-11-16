///
/// @file INetwork.hpp
/// @brief This file contains the network interface
/// @namespace cae
///

#pragma once

#include <Utils/Interfaces/IPlugin.hpp>

namespace cae
{

    ///
    /// @interface INetwork
    /// @brief Interface for network
    /// @namespace cae
    ///
    class INetwork : public utl::IPlugin
    {

        public:
            ~INetwork() override = default;

            virtual bool connect(const std::string &host, uint16_t port) = 0;

    }; // interface INetwork

} // namespace cae
