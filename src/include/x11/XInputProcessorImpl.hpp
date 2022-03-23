#pragma once
#include <algorithm>
#include <list>
#include <map>

#include "IngameIME.hpp"
#include "InputProcessor.hpp"

#include "StringUtil.hpp"

namespace libxim {
    class InputProcessorImpl : public IngameIME::InputProcessor {
      protected:
        XIM xim;

      public:
        InputProcessorImpl(XIM xim) : xim(xim)
        {
            type = IngameIME::InputProcessorType::TextService;

            auto locale    = std::make_shared<IngameIME::Locale>();
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