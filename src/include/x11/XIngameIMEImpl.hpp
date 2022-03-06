#pragma once
#include <stdarg.h>

#include "IngameIME.hpp"

#include "XInputContextImpl.hpp"
#include "XInputProcessorImpl.hpp"

namespace libxim {
    class GlobalImpl : public IngameIME::Global {
      public:
        /**
         * @brief Get Active InputProcessor
         *
         * @return std::shared_ptr<InputProcessor>
         */
        virtual std::shared_ptr<const IngameIME::InputProcessor> getActiveInputProcessor() const override
        {
            // Get default display
            auto display = XOpenDisplay(NULL);
            auto xim     = XOpenIM(display, NULL, NULL, NULL);
            return std::make_shared<InputProcessorImpl>(xim);
        }

        /**
         * @brief Get system availiable InputProcessor
         *
         * @return std::list<std::shared_ptr<InputProcessor>>
         */
        virtual std::list<std::shared_ptr<const IngameIME::InputProcessor>> getInputProcessors() const override
        {
            std::list<std::shared_ptr<const IngameIME::InputProcessor>> result;

            result.push_back(getActiveInputProcessor());

            return result;
        }

        /**
         * @brief Get the InputContext object
         *
         * @param hWnd the window to create InputContext
         * @return std::shared_ptr<InputContext>
         */
        virtual std::shared_ptr<IngameIME::InputContext> getInputContext(void* display, ...) override
        {
            va_list args;
            va_start(args, display);
            auto ctx = std::make_shared<InputContextImpl>(reinterpret_cast<Display*>(display), va_arg(args, Window));
            va_end(args);
            return ctx;
        }
    };
}// namespace libxim