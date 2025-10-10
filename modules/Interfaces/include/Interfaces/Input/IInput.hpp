///
/// @file IInput.hpp
/// @brief This file contains the input interface
/// @namespace cae
///

#pragma once

#include <memory>
#include <vector>

#include "Interfaces/Input/IGamepad.hpp"
#include "Interfaces/Input/IKeyboard.hpp"
#include "Interfaces/Input/IMouse.hpp"
#include "Utils/Interfaces/IPlugin.hpp"

namespace cae
{

    ///
    /// @interface IInput
    /// @brief Interface for audio
    /// @namespace cae
    ///
    class IInput : public utl::IPlugin
    {

        public:
            ~IInput() override = default;

            virtual const std::unique_ptr<IKeyboard> &getKeyboard() const = 0;
            virtual const std::unique_ptr<IMouse> &getMouse() const = 0;
            virtual const std::vector<std::unique_ptr<IGamepad>> &getGamepads() const = 0;

        private:
            std::unique_ptr<IKeyboard> m_keyboard;
            std::unique_ptr<IMouse> m_mouse;
            std::vector<std::unique_ptr<IGamepad>> m_gamepads;
    }; // interface IInput

} // namespace cae
