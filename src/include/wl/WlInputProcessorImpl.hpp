#pragma once
#include <algorithm>
#include <list>
#include <map>

#include "IngameIME.hpp"
#include "InputProcessor.hpp"

namespace libwl {
    class InputProcessorImpl : public IngameIME::InputProcessor {
      public:
        InputProcessorImpl()
        {
            type = IngameIME::InputProcessorType::TextService;

            auto locale    = std::make_shared<IngameIME::Locale>();
            locale->locale = convert(setlocale(LC_ALL, ""));
            locale->name   = format(L"[locale: %ls", locale->locale.c_str());
            this->locale   = locale;

            name = L"[Wayland IM]";
        }

      public:
        virtual void setActivated() const override
        {
            // Wayland IM not support this operation
        }
    };
}// namespace libwl