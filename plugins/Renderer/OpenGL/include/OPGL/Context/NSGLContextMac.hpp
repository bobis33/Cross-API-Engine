///
/// @file EGLContext.hpp
/// @brief This file contains the EGLContextLinux class declaration
/// @namespace cae
///

#pragma once

#include "OPGL/glad/glad.h"

#include "IContext.hpp"
#include "Interfaces/IWindow.hpp"

#if defined(__APPLE__)

namespace cae
{

    class NSGLContextMac : public IContext
    {
        public:
            NSGLContextMac();
            ~NSGLContextMac() override;

            void initialize(const NativeWindowHandle &window) override;

            void swapBuffers() override;

            void setVSyncEnabled(const bool enabled) override;
            [[nodiscard]] bool isVSyncEnabled() const override;

        private:
            void *m_context = nullptr;
    };

} // namespace cae
#endif
