///
/// @file NSGLContext.hpp
/// @brief This file contains the NSGLContext class declaration
/// @namespace cae
///

#pragma once

#ifdef __APPLE__

#include "OPGL/Context/IContext.hpp"

namespace cae
{

    ///
    /// @class NSGLContext
    /// @brief Implementation of IContext for macOS using NSGL
    /// @namespace cae
    ///
    class NSGLContext final : public IContext
    {
        public:
            NSGLContext() = default;
            ~NSGLContext() override;

            void initialize(const NativeWindowHandle &window) override;

            void swapBuffers() override;

            void setVSyncEnabled(bool enabled) override;
            [[nodiscard]] bool isVSyncEnabled() const override;

        private:
            void *m_context = nullptr;

    }; // class NSGLContext

} // namespace cae

#endif
