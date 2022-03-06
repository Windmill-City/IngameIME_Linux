#pragma once
#include <memory>
#include <string>

#include "XInputContextImpl.hpp"

namespace libxim {
    struct InternalRect : public IngameIME::PreEditRect
    {
        operator XRectangle()
        {
            XRectangle rect;
            rect.x      = left;
            rect.y      = top;
            rect.height = bottom - top;
            rect.width  = right - left;
            return rect;
        }
    };
    class CompositionImpl : public IngameIME::Composition {
      protected:
        InputContextImpl* inputCtx;

      public:
        static int PreeditStartCallback(XIC ic, XPointer client_data, XPointer call_data)
        {
            auto inputCtx = reinterpret_cast<InputContextImpl*>(client_data);
            inputCtx->comp->IngameIME::PreEditCallbackHolder::runCallback(IngameIME::CompositionState::Begin, nullptr);
            // Max length of Preedit
            return 64;
        }

        static void PreeditDrawCallback(XIC ic, XPointer client_data, XIMPreeditDrawCallbackStruct* call_data)
        {
            auto inputCtx = reinterpret_cast<InputContextImpl*>(client_data);

            inputCtx->ctx.selStart = inputCtx->ctx.selEnd = call_data->caret;
            if (call_data->text->encoding_is_wchar)
                inputCtx->ctx.content = std::wstring(call_data->text->string.wide_char, call_data->text->length);
            else {
                wchar_t buf[64];
                swprintf(buf, sizeof(buf), L"%s", call_data->text->string.multi_byte);
                inputCtx->ctx.content = std::wstring(buf, call_data->text->length);
            }

            inputCtx->comp->IngameIME::PreEditCallbackHolder::runCallback(IngameIME::CompositionState::Update,
                                                                          &inputCtx->ctx);
        }

        static void PreeditCaretCallback(XIC ic, XPointer client_data, XIMPreeditCaretCallbackStruct* call_data)
        {
            auto inputCtx          = reinterpret_cast<InputContextImpl*>(client_data);
            inputCtx->ctx.selStart = inputCtx->ctx.selEnd = call_data->position;
            inputCtx->comp->IngameIME::PreEditCallbackHolder::runCallback(IngameIME::CompositionState::Update,
                                                                          &inputCtx->ctx);
        }

        static void PreeditDoneCallback(XIC ic, XPointer client_data, XPointer call_data)
        {
            auto inputCtx = reinterpret_cast<InputContextImpl*>(client_data);
            inputCtx->comp->IngameIME::PreEditCallbackHolder::runCallback(IngameIME::CompositionState::End, nullptr);
        }

        static void GeometryCallback(XIC ic, XPointer client_data, XPointer call_data)
        {
            auto inputCtx = reinterpret_cast<InputContextImpl*>(client_data);

            InternalRect rect;
            inputCtx->comp->IngameIME::PreEditRectCallbackHolder::runCallback(rect);
            XSetICValues(inputCtx->xic, XNArea, rect, NULL);
        }

      public:
        CompositionImpl(InputContextImpl* inputCtx) : inputCtx(inputCtx) {}

      public:
        /**
         * @brief Terminate active composition
         *
         */
        virtual void terminate() noexcept override
        {
            // XIM no support this operation
        }
    };
}// namespace libxim
