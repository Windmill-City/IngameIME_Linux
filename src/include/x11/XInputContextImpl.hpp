#pragma once
#include <X11/Xlib.h>

#include "InputContext.hpp"
#include "InputProcessor.hpp"

namespace IngameIME::x {
    class InputContextImpl : public InputContext {
      protected:
        Display* display;
        Window   window;
        XIM      xim{nullptr};
        XIC      xic{nullptr};

        PreEditContext ctx;
        friend class CompositionImpl;

        bool activated{false};
        bool fullscreen{false};

      public:
        InputContextImpl(Display* display, Window window);
        ~InputContextImpl()
        {
            if (xic) XDestroyIC(xic);
            if (xim) XCloseIM(xim);
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
            if (activated)
                XSetICFocus(xic);
            else
                XUnsetICFocus(xic);
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

            // XIM not support fetching candidate list
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
}// namespace libxim