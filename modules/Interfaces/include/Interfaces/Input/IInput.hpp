///
/// @file IInput.hpp
/// @brief This file contains the input interface
/// @namespace cae
///

#pragma once

#include "Interfaces/Input/IGamepad.hpp"
#include "Interfaces/Input/IKeyboard.hpp"
#include "Interfaces/Input/IMouse.hpp"

#include <memory>
#include <vector>

namespace cae
{

    ///
    /// @interface IInput
    /// @brief Interface for inputs
    /// @namespace cae
    ///
    class IInput : public utl::IPlugin
    {

        public:
            ~IInput() override = default;

            virtual const std::unique_ptr<IKeyboard> &getKeyboard() const = 0;
            virtual const std::unique_ptr<IMouse> &getMouse() const = 0;
            virtual const std::vector<std::unique_ptr<IGamepad>> &getGamepads() const = 0;

            virtual void setGamepads(std::vector<std::unique_ptr<IGamepad>> &gamepads) = 0;
            virtual void setKeyboard(std::unique_ptr<IKeyboard> &keyboard) = 0;
            virtual void setMouse(std::unique_ptr<IMouse> &mouse) = 0;

    }; // interface IInput

} // namespace cae
