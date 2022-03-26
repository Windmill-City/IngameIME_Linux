#pragma once
#include <cstring>

#include "IngameIME.hpp"

#include "WlInputProcessorImpl.hpp"

namespace IngameIME::wl {
    class GlobalImpl : public Global {
      protected:
        wl_display*                                     display;
        std::pair<wl_seat*, uint32_t>                   seat;
        std::pair<zwp_text_input_manager_v3*, uint32_t> inputMgr;

      protected:
        static void onGlobalAdded(void*               data,
                                  struct wl_registry* wl_registry,
                                  uint32_t            name,
                                  const char*         interface,
                                  uint32_t            version)
        {
            auto global = reinterpret_cast<GlobalImpl*>(data);

            // Wl Seat
            if (strcmp(interface, wl_seat_interface.name) == 0) {
                global->seat.first =
                    reinterpret_cast<wl_seat*>(wl_registry_bind(wl_registry, name, &wl_seat_interface, version));
                global->seat.second = name;
            }

            // Input Manager
            if (strcmp(interface, zwp_text_input_manager_v3_interface.name) == 0) {
                global->inputMgr.first = reinterpret_cast<zwp_text_input_manager_v3*>(
                    wl_registry_bind(wl_registry, name, &zwp_text_input_manager_v3_interface, version));
                global->inputMgr.second = name;
            }
        }

        static void onGlobalRemoved(void* data, struct wl_registry* wl_registry, uint32_t name)
        {
            auto global = reinterpret_cast<GlobalImpl*>(data);

            if (global->seat.second == name) {
                wl_seat_destroy(global->seat.first);
                global->seat = {};
            }

            if (global->inputMgr.second == name) {
                zwp_text_input_manager_v3_destroy(global->inputMgr.first);
                global->inputMgr = {};
            }
        }

        const struct wl_registry_listener RegistryListener
        {
            .global = GlobalImpl::onGlobalAdded, .global_remove = GlobalImpl::onGlobalRemoved
        };

      public:
        GlobalImpl(wl_display* display) : display(display)
        {
            auto reg = wl_display_get_registry(display);
            if (!reg) throw std::runtime_error("Failed to get Wayland Registry!");

            wl_registry_add_listener(reg, &RegistryListener, this);
            // Wait until all the global object has been registered
            wl_display_roundtrip(display);
        }

        ~GlobalImpl()
        {
            zwp_text_input_manager_v3_destroy(inputMgr.first);
            wl_seat_destroy(seat.first);
            wl_display_disconnect(display);
        }

      public:
        /**
         * @brief Get Active InputProcessor
         *
         * @return std::shared_ptr<InputProcessor>
         */
        virtual std::shared_ptr<const InputProcessor> getActiveInputProcessor() const override
        {
            return std::make_shared<InputProcessorImpl>();
        }

        /**
         * @brief Get system availiable InputProcessor
         *
         * @return std::list<std::shared_ptr<InputProcessor>>
         */
        virtual std::list<std::shared_ptr<const InputProcessor>> getInputProcessors() const override
        {
            std::list<std::shared_ptr<const InputProcessor>> result;
            return result;
        }

        /**
         * @brief Get the InputContext object
         *
         * @param hWnd the window to create InputContext
         * @return std::shared_ptr<InputContext>
         */
        virtual std::shared_ptr<InputContext> getInputContext(void* surface, ...) override
        {
            return std::make_shared<InputContextImpl>(
                inputMgr.first, seat.first, reinterpret_cast<wl_surface*>(surface));
        }
    };
}// namespace libwl