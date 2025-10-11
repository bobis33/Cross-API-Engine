///
/// @file Engine.hpp
/// @brief This file contains the engine class declaration
/// @namespace cae
///

#pragma once

#include <functional>
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
            Engine(const std::function<std::shared_ptr<IAudio>()> &audioFactory,
                   const std::function<std::shared_ptr<IInput>()> &inputFactory,
                   const std::function<std::shared_ptr<INetwork>()> &networkFactory,
                   const std::function<std::shared_ptr<IRenderer>()> &rendererFactory,
                   const std::function<std::shared_ptr<IWindow>()> &windowFactory);
            ~Engine() = default;

            Engine(const Engine &) = delete;
            Engine &operator=(const Engine &) = delete;
            Engine(Engine &&) = delete;
            Engine &operator=(Engine &&) = delete;

            [[nodiscard]] const std::shared_ptr<IAudio> &getAudio() const { return m_audioPlugin; }
            [[nodiscard]] const std::shared_ptr<IInput> &getInput() const { return m_inputPlugin; }
            [[nodiscard]] const std::shared_ptr<INetwork> &getNetwork() const { return m_networkPlugin; }
            [[nodiscard]] const std::shared_ptr<IRenderer> &getRenderer() const { return m_rendererPlugin; }
            [[nodiscard]] const std::shared_ptr<IWindow> &getWindow() const { return m_windowPlugin; }

            [[nodiscard]] const std::unique_ptr<utl::Clock> &getClock() { return m_clock; }

            void run() const;
            void stop();

        private:
            std::shared_ptr<IAudio> m_audioPlugin = nullptr;
            std::shared_ptr<IInput> m_inputPlugin = nullptr;
            std::shared_ptr<INetwork> m_networkPlugin = nullptr;
            std::shared_ptr<IRenderer> m_rendererPlugin = nullptr;
            std::shared_ptr<IWindow> m_windowPlugin = nullptr;

            std::unique_ptr<utl::Clock> m_clock = nullptr;

    }; // class Engine
} // namespace cae