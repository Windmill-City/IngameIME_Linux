#pragma once
#include <algorithm>
#include <future>

#include "IngameIME.hpp"

#include "FcitxInputProcessor.hpp"

namespace IngameIME::dbus {
    using namespace org::fcitx;
    class FcitxGlobal : public Global {
      protected:
        Dispatcher  disp;
        std::thread dispRunner;

      public:
        Fcitx::Controller ctrl{disp};

      public:
        FcitxGlobal()
        {
            dispRunner = std::thread([this]() { this->disp.run(); });
            dispRunner.detach();
        }

        ~FcitxGlobal()
        {
            disp.stop();
            dispRunner.join();
        }

      public:
        /**
         * @brief Get Active InputProcessor
         *
         * @return std::shared_ptr<InputProcessor>
         */
        virtual std::shared_ptr<const InputProcessor> getActiveInputProcessor() const override
        {
            Fcitx::Controller& ctrl = const_cast<Fcitx::Controller&>(this->ctrl);

            auto ims      = ctrl.AvailableInputMethods();
            auto activeIM = ctrl.CurrentInputMethod();

            auto iter = std::find_if(ims.begin(), ims.end(), [=](auto&& im) { return im.uniqueName == activeIM; });

            if (iter == ims.end())
                throw std::runtime_error(format("Active InputMethod: %s not in InputMethod List!", activeIM));

            return FcitxInputProcessor::getInputProcessor(ctrl, *iter);
        }

        /**
         * @brief Get system availiable InputProcessor
         *
         * @return std::list<std::shared_ptr<InputProcessor>>
         */
        virtual std::list<std::shared_ptr<const InputProcessor>> getInputProcessors() const override
        {
            std::list<std::shared_ptr<const InputProcessor>> result;

            Fcitx::Controller& ctrl = const_cast<Fcitx::Controller&>(this->ctrl);

            auto ims = ctrl.AvailableInputMethods();

            std::string                   defaultLayout;
            std::vector<Fcitx::GroupInfo> infos;
            std::tie(defaultLayout, infos) = ctrl.InputMethodGroupInfo(ctrl.CurrentInputMethodGroup());

            for (auto&& im : ims) {
                for (auto&& info : infos) {
                    if (info.name == im.uniqueName) result.push_back(FcitxInputProcessor::getInputProcessor(ctrl, im));
                }
            }

            return result;
        }

        /**
         * @brief Get the InputContext object
         *
         * @param hWnd the window to create InputContext
         * @return std::shared_ptr<InputContext>
         */
        virtual std::shared_ptr<InputContext> getInputContext(void* surface, ...) override {}
    };
}// namespace libdbus