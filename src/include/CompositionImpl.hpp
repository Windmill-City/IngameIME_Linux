#pragma once
#include <memory>
#include <string>

#include "InputContextImpl.hpp"

namespace libxim {
    class CompositionImpl : public IngameIME::Composition {
      public:
        /**
         * @brief Terminate active composition
         *
         */
        virtual void terminate() override {}
    };
}// namespace libxim

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
        virtual void terminate() noexcept override
        {
        }
    };
}// namespace libimm
