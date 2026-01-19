///
/// @file IAudio.hpp
/// @brief This file contains the audio abstract class
/// @namespace cae
///

#pragma once

#include "Interfaces/Audio/IAudio.hpp"

namespace cae
{

    ///
    /// @interface AAudio
    /// @brief Abstract class for audio
    /// @namespace cae
    ///
    class AAudio : public IAudio
    {

        public:
            ~AAudio() override = default;

    }; // interface AAudio

} // namespace cae
