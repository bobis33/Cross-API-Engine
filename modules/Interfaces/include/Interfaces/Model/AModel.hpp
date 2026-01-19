///
/// @file AModel.hpp
/// @brief This file contains the model abstract class
/// @namespace cae
///

#pragma once

#include "Interfaces/Model/IModel.hpp"

namespace cae
{

    ///
    /// @interface AModel
    /// @brief Abstract class for model
    /// @namespace cae
    ///
    class AModel : public IModel
    {

        public:
            ~AModel() override = default;

    }; // interface AModel

} // namespace cae
