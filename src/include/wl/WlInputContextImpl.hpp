#pragma once
#include <wayland-client.h>
#include <wayland-text-input-client-protocol.h>

#include "InputContext.hpp"
#include "InputProcessor.hpp"

namespace IngameIME::wl {
    class InputContextImpl : public InputContext {
      protected:
        wl_surface*        surface;
        zwp_text_input_v3* textInput;

        bool focusing{false};

        bool activated{false};
        bool fullscreen{false};

        friend class CompositionImpl;

      public:
        InputContextImpl(zwp_text_input_manager_v3*, wl_seat*, wl_surface*);
        ~InputContextImpl()
        {
            zwp_text_input_v3_destroy(textInput);
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

            if (focusing) {
                if (activated)
                    zwp_text_input_v3_enable(textInput);
                else
                    zwp_text_input_v3_disable(textInput);

                // Apply status change
                zwp_text_input_v3_commit(textInput);
            }
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
}// namespace libwl