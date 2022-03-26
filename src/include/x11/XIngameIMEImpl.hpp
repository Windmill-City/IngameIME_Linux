#pragma once
#include <stdarg.h>

#include "IngameIME.hpp"

#include "XInputContextImpl.hpp"
#include "XInputProcessorImpl.hpp"

namespace IngameIME::x {
    class GlobalImpl : public Global {
      protected:
        Display* display;

      public:
        GlobalImpl(Display* display) : display(display) {}

      public:
        /**
         * @brief Get Active InputProcessor
         *
         * @return std::shared_ptr<InputProcessor>
         */
        virtual std::shared_ptr<const InputProcessor> getActiveInputProcessor() const override
        {
            static auto inputProcessor = std::make_shared<InputProcessorImpl>(XOpenIM(display, NULL, NULL, NULL));
            return inputProcessor;
        }

        /**
         * @brief Get system availiable InputProcessor
         *
         * @return std::list<std::shared_ptr<InputProcessor>>
         */
        virtual std::list<std::shared_ptr<const InputProcessor>> getInputProcessors() const override
        {
            std::list<std::shared_ptr<const InputProcessor>> result;

            result.push_back(getActiveInputProcessor());

            return result;
        }

        /**
         * @brief Get the InputContext object
         *
         * @param hWnd the window to create InputContext
         * @return std::shared_ptr<InputContext>
         */
        virtual std::shared_ptr<InputContext> getInputContext(void* window, ...) override
        {
            auto ctx = std::make_shared<InputContextImpl>(display, reinterpret_cast<Window>(window));
            return ctx;
        }
    };
}// namespace libxim