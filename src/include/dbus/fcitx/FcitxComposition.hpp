#pragma once
#include <memory>
#include <string>

#include "FcitxInputContext.hpp"

namespace IngameIME::dbus {
    class FcitxCompositionImpl : public Composition {
      protected:
        FcitxInputContext* inputCtx;

      public:
        FcitxCompositionImpl(FcitxInputContext* inputCtx) : inputCtx(inputCtx) {}

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
