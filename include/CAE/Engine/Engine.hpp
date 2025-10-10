///
/// @file Engine.hpp
/// @brief This file contains the engine class declaration
/// @namespace cae
///

#pragma once

#include <memory>

#include "Interfaces/IAudio.hpp"
#include "Interfaces/INetwork.hpp"
#include "Interfaces/IRenderer.hpp"
#include "Interfaces/IWindow.hpp"
#include "Interfaces/Input/IInput.hpp"
#include "Utils/Clock.hpp"

namespace cae
{

    ///
    /// @class Engine
    /// @brief Engine class
    /// @namespace cae
    ///
    class Engine
    {

        public:
            Engine();
            ~Engine() = default;

            Engine(const Engine &) = delete;
            Engine &operator=(const Engine &) = delete;
            Engine(Engine &&) = delete;
            Engine &operator=(Engine &&) = delete;

        private:
            std::unique_ptr<IAudio> m_audioPlugin;
            std::unique_ptr<IInput> m_inputPlugin;
            std::unique_ptr<INetwork> m_networkPlugin;
            std::unique_ptr<IRenderer> m_rendererPlugin;
            std::unique_ptr<IWindow> m_windowPlugin;

            std::unique_ptr<utl::Clock> m_clock;

    }; // class Engine

} // namespace cae