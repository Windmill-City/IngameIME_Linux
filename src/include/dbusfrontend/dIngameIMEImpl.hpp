#pragma once

#include "IngameIME.hpp"

#include "dInputProcessorImpl.hpp"

namespace libdbus {
    class GlobalImpl : public IngameIME::Global {
      protected:
      public:
        GlobalImpl() {}

        ~GlobalImpl() {}

      public:
        /**
         * @brief Get Active InputProcessor
         *
         * @return std::shared_ptr<InputProcessor>
         */
        virtual std::shared_ptr<const IngameIME::InputProcessor> getActiveInputProcessor() const override
        {
            return std::make_shared<InputProcessorImpl>();
        }

        /**
         * @brief Get system availiable InputProcessor
         *
         * @return std::list<std::shared_ptr<InputProcessor>>
         */
        virtual std::list<std::shared_ptr<const IngameIME::InputProcessor>> getInputProcessors() const override
        {
            std::list<std::shared_ptr<const IngameIME::InputProcessor>> result;
            return result;
        }

        /**
         * @brief Get the InputContext object
         *
         * @param hWnd the window to create InputContext
         * @return std::shared_ptr<InputContext>
         */
        virtual std::shared_ptr<IngameIME::InputContext> getInputContext(void* surface, ...) override {}
    };
}// namespace libdbus