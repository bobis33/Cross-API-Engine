///
/// @file IModel.hpp
/// @brief This file contains the model interface
/// @namespace cae
///

#pragma once

#include "Utils/Interfaces/IPlugin.hpp"

namespace cae
{

    ///
    /// @interface IModel
    /// @brief Interface for model
    /// @namespace cae
    ///
    class IModel : public utl::IPlugin
    {

        public:
            ~IModel() override = default;

    }; // interface IModel

} // namespace cae
