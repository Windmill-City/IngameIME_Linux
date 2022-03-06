#pragma once
#include <algorithm>
#include <list>
#include <map>

#include "IngameIME.hpp"
#include "InputProcessor.hpp"

#include "FormatUtil.hpp"

namespace libxim {
    class InternalLocale : public IngameIME::Locale {
      protected:
        static std::map<std::wstring, std::weak_ptr<InternalLocale>> WeakRefs;

      public:
        InternalLocale(std::wstring locale)
        {
            this->locale = locale;
            // XIM not support locale name fetch
            this->name = format(L"[locale: %ls]", this->locale.c_str());
        }
        ~InternalLocale()
        {
            WeakRefs.erase(locale);
        }

      public:
        static std::shared_ptr<InternalLocale> getLocale(char* locale)
        {
            auto locale_ = format(L"%hs", locale);

            auto                            iter = WeakRefs.find(locale_);
            std::shared_ptr<InternalLocale> result;
            if (iter == WeakRefs.end() || !(result = (*iter).second.lock())) {
                result            = std::make_shared<InternalLocale>(locale_);
                WeakRefs[locale_] = result;
            }

            return result;
        }
    };
    std::map<std::wstring, std::weak_ptr<InternalLocale>> InternalLocale::WeakRefs = {};

    class InputProcessorImpl : public IngameIME::InputProcessor {
      protected:
        XIM xim;

      public:
        InputProcessorImpl(XIM xim) : xim(xim)
        {
            type   = IngameIME::InputProcessorType::TextService;
            locale = InternalLocale::getLocale(XLocaleOfIM(xim));
            name   = format(L"[XIM: %08x]", xim);
        }

        ~InputProcessorImpl()
        {
            if (xim) XCloseIM(xim);
        }

      public:
        virtual void setActivated() const override
        {
            // XIM no support this operation
        }
    };
}// namespace libxim