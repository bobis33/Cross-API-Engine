///
/// @file IAudio.hpp
/// @brief This file contains the audio interface
/// @namespace cae
///

#pragma once

#include <Utils/Interfaces/IPlugin.hpp>

namespace cae
{

    ///
    /// @interface IAudio
    /// @brief Interface for audio
    /// @namespace cae
    ///
    class IAudio : public utl::IPlugin
    {

        public:
            ~IAudio() override = default;

    }; // interface IAudio

} // namespace cae
