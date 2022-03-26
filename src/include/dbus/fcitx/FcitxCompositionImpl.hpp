#pragma once
#include <memory>
#include <string>

#include "FcitxInputContextImpl.hpp"

namespace IngameIME::dbus::fcitx {
    class CompositionImpl : public Composition {
      protected:
        InputContextImpl* inputCtx;

      public:
        CompositionImpl(InputContextImpl* inputCtx) : inputCtx(inputCtx) {}

      public:
        /**
         * @brief Terminate active composition
         *
         */
        virtual void terminate() noexcept override
        {
            inputCtx->ctx.Reset();
        }
    };
}// namespace IngameIME::dbus
