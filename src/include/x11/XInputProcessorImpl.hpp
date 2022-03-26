#pragma once
#include <algorithm>
#include <list>
#include <map>

#include "IngameIME.hpp"
#include "InputProcessor.hpp"

#include "StringUtil.hpp"

namespace IngameIME::x {
    class InputProcessorImpl : public InputProcessor {
      protected:
        XIM xim;

      public:
        InputProcessorImpl(XIM xim) : xim(xim)
        {
            type = InputProcessorType::TextService;

            auto locale    = std::make_shared<Locale>();
            locale->locale = convert(XLocaleOfIM(xim));
            locale->name   = format(L"[locale: %s]", locale->locale.c_str());
            this->locale   = locale;

            name = L"[XIM]";
            XCloseIM(xim);
        }

      public:
        virtual void setActivated() const override
        {
            // XIM no support this operation
        }
    };
}// namespace libxim