#pragma once
#include <memory>
#include <string>

#include "WlInputContextImpl.hpp"

namespace libwl {
    class CompositionImpl : public IngameIME::Composition {
      protected:
        InputContextImpl* inputCtx;

        bool composing{false};

        std::pair<IngameIME::PreEditContext, bool> ctx;
        std::pair<std::string, bool>               commit;

      protected:
        void refreshPreEditRect()
        {
            IngameIME::PreEditRect rect;
            IngameIME::PreEditRectCallbackHolder::runCallback(rect);
            zwp_text_input_v3_set_cursor_rectangle(
                inputCtx->textInput, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
            zwp_text_input_v3_commit(inputCtx->textInput);
        }

      public:
        using Listener = zwp_text_input_v3_listener;
        static void onEnter(void* data, struct zwp_text_input_v3* zwp_text_input_v3, struct wl_surface* surface)
        {
            auto comp = reinterpret_cast<CompositionImpl*>(data);

            if (comp->inputCtx->activated) {
                zwp_text_input_v3_enable(comp->inputCtx->textInput);

                // Refresh content type on focus change
                zwp_text_input_v3_set_content_type(
                    comp->inputCtx->textInput,
                    zwp_text_input_v3_content_hint::ZWP_TEXT_INPUT_V3_CONTENT_HINT_NONE,
                    zwp_text_input_v3_content_purpose::ZWP_TEXT_INPUT_V3_CONTENT_PURPOSE_NORMAL);

                // Refresh PreEditRect position on focus change
                comp->refreshPreEditRect();
            }
        }

        static void onLeave(void* data, struct zwp_text_input_v3* zwp_text_input_v3, struct wl_surface* surface)
        {
            // Nothing to do
        }

        static void onDeleteSurrounding(void*                     data,
                                        struct zwp_text_input_v3* zwp_text_input_v3,
                                        uint32_t                  before_length,
                                        uint32_t                  after_length)
        {
            // Nothing to do
        }

        static void onPreedit(void*                     data,
                              struct zwp_text_input_v3* zwp_text_input_v3,
                              const char*               text,
                              int32_t                   cursor_begin,
                              int32_t                   cursor_end)
        {
            auto comp = reinterpret_cast<CompositionImpl*>(data);

            if (text) {
                comp->ctx.first.content  = text;
                comp->ctx.first.selStart = cursor_begin;
                comp->ctx.first.selEnd   = cursor_end;
                comp->ctx.second         = true;
            }
        }

        static void onCommit(void* data, struct zwp_text_input_v3* zwp_text_input_v3, const char* text)
        {
            auto comp = reinterpret_cast<CompositionImpl*>(data);

            if (text) {
                comp->commit.first  = text;
                comp->commit.second = true;
            }
        }

        static void onDone(void* data, struct zwp_text_input_v3* zwp_text_input_v3, uint32_t serial)
        {
            auto comp = reinterpret_cast<CompositionImpl*>(data);

            if (comp->ctx.second) {
                if (!comp->composing) {
                    comp->composing = true;
                    comp->IngameIME::PreEditCallbackHolder::runCallback(IngameIME::CompositionState::Begin, nullptr);
                }

                comp->IngameIME::PreEditCallbackHolder::runCallback(IngameIME::CompositionState::Update,
                                                                    &comp->ctx.first);
                comp->refreshPreEditRect();
                comp->ctx.second = false;
            }
            else {
                comp->IngameIME::PreEditCallbackHolder::runCallback(IngameIME::CompositionState::End, nullptr);
                comp->composing = false;
            }

            if (comp->commit.second) {
                comp->IngameIME::CommitCallbackHolder::runCallback(comp->commit.first);
                comp->commit.second = false;
            }
        }

        const Listener listener{onEnter, onLeave, onPreedit, onCommit, onDeleteSurrounding, onDone};

      public:
        CompositionImpl(InputContextImpl* inputCtx) : inputCtx(inputCtx)
        {
            zwp_text_input_v3_add_listener(inputCtx->textInput, &listener, this);
        }

      public:
        /**
         * @brief Terminate active composition
         *
         */
        virtual void terminate() noexcept override
        {
            if (inputCtx->activated) {
                // Disable text input to terminate Composition
                zwp_text_input_v3_disable(inputCtx->textInput);
                zwp_text_input_v3_commit(inputCtx->textInput);

                // Reopen
                zwp_text_input_v3_enable(inputCtx->textInput);
                zwp_text_input_v3_commit(inputCtx->textInput);
            }
        }
    };
}// namespace libwl
