///
/// @file AInput.hpp
/// @brief This file contains the input abstract class
/// @namespace cae
///

#pragma once

#include "Interfaces/Input/IInput.hpp"

namespace cae
{

    ///
    /// @interface AInput
    /// @brief Abstract class for inputs
    /// @namespace cae
    ///
    class AInput : public IInput
    {

        public:
            ~AInput() override = default;

            const std::unique_ptr<IKeyboard> &getKeyboard() const override { return m_keyboard; }
            const std::unique_ptr<IMouse> &getMouse() const override { return m_mouse; }
            const std::vector<std::unique_ptr<IGamepad>> &getGamepads() const override { return m_gamepads; }

            void setGamepads(std::vector<std::unique_ptr<IGamepad>> &gamepads) override
            {
                m_gamepads = std::move(gamepads);
            }
            void setKeyboard(std::unique_ptr<IKeyboard> &keyboard) override { m_keyboard = std::move(keyboard); }
            void setMouse(std::unique_ptr<IMouse> &mouse) override { m_mouse = std::move(mouse); }

        private:
            std::unique_ptr<IKeyboard> m_keyboard;
            std::unique_ptr<IMouse> m_mouse;
            std::vector<std::unique_ptr<IGamepad>> m_gamepads;
    }; // interface AInput

} // namespace cae
