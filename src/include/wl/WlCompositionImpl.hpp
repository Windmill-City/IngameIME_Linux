#pragma once
#include <memory>
#include <string>

#include "WlInputContextImpl.hpp"

namespace libwl {
    class CompositionImpl : public IngameIME::Composition {
      protected:
        InputContextImpl* inputCtx;

      public:
        CompositionImpl(InputContextImpl* inputCtx) : inputCtx(inputCtx) {}

      public:
        /**
         * @brief Terminate active composition
         *
         */
        virtual void terminate() noexcept override {}
    };
}// namespace libwl
