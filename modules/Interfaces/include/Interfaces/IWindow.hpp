///
/// @file IWindow.hpp
/// @brief This file contains the Window interface
/// @namespace cae
///

#pragma once

#include "Utils/Interfaces/IPlugin.hpp"

namespace cae
{

    struct WindowSize
    {
        uint16_t width;
        uint16_t height;
    };

    ///
    /// @interface IWindow
    /// @brief Interface for window
    /// @namespace cae
    ///
    class IWindow : public utl::IPlugin
    {

        public:
            ~IWindow() override = default;

            virtual bool create(const std::string &name, WindowSize size) = 0;

    private:
    }; // interface IWindow

} // namespace cae
