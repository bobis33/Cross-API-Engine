///
/// @file IKeyboard.hpp
/// @brief This file contains the input keyboard interface
/// @namespace cae
///

#pragma once

#include "Key/Keyboard.hpp"

#include "Utils/Interfaces/IPlugin.hpp"

#include <array>

namespace cae
{

    ///
    /// @interface IKeyboard
    /// @brief Interface for keyboard
    /// @namespace cae
    ///
    class IKeyboard : public utl::IPlugin
    {

        public:
            ~IKeyboard() override = default;

            virtual bool isKeyPressed(KeyCode keyCode) const = 0;

        private:
            std::array<KeyState, static_cast<size_t>(KeyCode::Count)> m_keyMap{};
    }; // interface IKeyboard

} // namespace cae
