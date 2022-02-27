#include <stdarg.h>

#include "IngameIME.hpp"

#include "CompositionImpl.hpp"
#include "InputProcessorImpl.hpp"

namespace libxim {
    class GlobalImpl : public IngameIME::Global {
      public:
        /**
         * @brief Get Active InputProcessor
         *
         * @return std::shared_ptr<InputProcessor>
         */
        virtual std::shared_ptr<const IngameIME::InputProcessor> getActiveInputProcessor() const override {}

        /**
         * @brief Get system availiable InputProcessor
         *
         * @return std::list<std::shared_ptr<InputProcessor>>
         */
        virtual std::list<std::shared_ptr<const IngameIME::InputProcessor>> getInputProcessors() const override {}

        /**
         * @brief Get the InputContext object
         *
         * @param hWnd the window to create InputContext
         * @return std::shared_ptr<InputContext>
         */
        virtual std::shared_ptr<IngameIME::InputContext> getInputContext(void* hWnd, ...) override {}
    };
}// namespace libxim

namespace libwl {
    class GlobalImpl : public IngameIME::Global {
      public:
        /**
         * @brief Get Active InputProcessor
         *
         * @return std::shared_ptr<InputProcessor>
         */
        virtual std::shared_ptr<const IngameIME::InputProcessor> getActiveInputProcessor() const override {}

        /**
         * @brief Get system availiable InputProcessor
         *
         * @return std::list<std::shared_ptr<InputProcessor>>
         */
        virtual std::list<std::shared_ptr<const IngameIME::InputProcessor>> getInputProcessors() const override {}

        /**
         * @brief Get the InputContext object
         *
         * @param hWnd the window to create InputContext
         * @return std::shared_ptr<InputContext>
         */
        virtual std::shared_ptr<IngameIME::InputContext> getInputContext(void* hWnd, ...) override {}
    };
}// namespace libwl

IngameIME::Global& IngameIME::Global::getInstance()
{
    thread_local IngameIME::Global& Instance =
        false ? (IngameIME::Global&)*new libxim::GlobalImpl() : (IngameIME::Global&)*new libwl::GlobalImpl();
    return Instance;
}