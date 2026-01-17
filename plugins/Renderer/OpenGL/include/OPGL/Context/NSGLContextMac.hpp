///
/// @file NSGLContextMac.hpp
/// @brief This file contains the NSGLContextMac class declaration
/// @namespace cae
///

#pragma once

#ifdef __APPLE__

#include "OPGL/Context/IContext.hpp"

namespace cae
{

    ///
    /// @class NSGLContextMac
    /// @brief Implementation of IContext for macOS using NSGL
    /// @namespace cae
    ///
    class NSGLContextMac final : public IContext
    {
        public:
            NSGLContextMac() = default;
            ~NSGLContextMac() override;

            void initialize(const NativeWindowHandle &window) override;

            void swapBuffers() override;

            void setVSyncEnabled(bool enabled) override;
            [[nodiscard]] bool isVSyncEnabled() const override;

        private:
            void *m_context = nullptr;

    }; // class NSGLContextMac

} // namespace cae

#endif
