#pragma once
#include "InputContext.hpp"
#include "InputProcessor.hpp"

#include "FcitxService.hpp"

namespace IngameIME::dbus {
    using namespace org::fcitx;
    class FcitxInputContext : public InputContext {
      protected:
        Fcitx::InputContext ctx;

        bool activated{false};
        bool fullscreen{false};

        friend class FcitxCompositionImpl;

      public:
        FcitxInputContext();
        ~FcitxInputContext()
        {
            ctx.DestroyIC();
        }

      public:
        /**
         * @brief Set InputContext activate state
         *
         * @param activated if InputContext activated
         */
        virtual void setActivated(const bool activated) noexcept override
        {
            this->activated = activated;
        }
        /**
         * @brief Get if InputContext activated
         *
         * @return true activated
         * @return false not activated
         */
        virtual bool getActivated() const noexcept override
        {
            return activated;
        }
        /**
         * @brief Set InputContext full screen state
         *
         * @param fullscreen if InputContext full screen
         */
        virtual void setFullScreen(const bool fullscreen) noexcept override
        {
            this->fullscreen = fullscreen;
        }
        /**
         * @brief Get if InputContext in full screen state
         *
         * @return true full screen mode
         * @return false window mode
         */
        virtual bool getFullScreen() const noexcept override
        {
            return fullscreen;
        }
    };
}// namespace libdbus