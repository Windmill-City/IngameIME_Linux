#pragma once
#include <algorithm>
#include <list>
#include <map>

#include "IngameIME.hpp"
#include "InputProcessor.hpp"

#include "FcitxService.hpp"

namespace IngameIME::dbus {
    struct InternalLocale : public Locale
    {
      protected:
        static std::map<std::wstring, std::weak_ptr<const InternalLocale>> WeakRefs;

      public:
        InternalLocale(std::wstring locale)
        {
            this->locale = locale;
            name         = format(L"[locale: %ls", locale.c_str());
        }
        ~InternalLocale()
        {
            WeakRefs.erase(locale);
        }

      public:
        static std::shared_ptr<const InternalLocale> getLocale(std::wstring locale)
        {
            auto iter = WeakRefs.find(locale);

            std::shared_ptr<const InternalLocale> result;
            // Create new locale if not exist or expired
            if (iter == WeakRefs.end() || !(result = (*iter).second.lock())) {
                result           = std::make_shared<InternalLocale>(locale);
                WeakRefs[locale] = result;
            }

            return result;
        }
    };
    std::map<std::wstring, std::weak_ptr<const InternalLocale>> InternalLocale::WeakRefs = {};

    using namespace org::fcitx;
    class FcitxInputProcessor : public InputProcessor {
      protected:
        struct CompareInputMethodInfo
        {
            bool operator()(const Fcitx::InputMethodInfo& s1, const Fcitx::InputMethodInfo& s2) const
            {
                return s1.uniqueName.compare(s2.uniqueName) < 0;
            }
        };
        static std::map<Fcitx::InputMethodInfo, std::weak_ptr<const FcitxInputProcessor>, CompareInputMethodInfo>
            WeakRefs;

      protected:
        Fcitx::Controller&     ctrl;
        Fcitx::InputMethodInfo im;

      public:
        FcitxInputProcessor(Fcitx::Controller& ctrl, Fcitx::InputMethodInfo im) : ctrl(ctrl), im(im)
        {
            type = im.uniqueName.compare(0, 8, "keyboard") ? InputProcessorType::KeyboardLayout :
                                                             InputProcessorType::TextService;

            this->locale = InternalLocale::getLocale(convert(im.languageCode));

            name = convert(im.name);
        }

        ~FcitxInputProcessor()
        {
            WeakRefs.erase(im);
        }

      public:
        static std::shared_ptr<const FcitxInputProcessor> getInputProcessor(Fcitx::Controller&     ctrl,
                                                                            Fcitx::InputMethodInfo im)
        {
            auto iter = WeakRefs.find(im);

            std::shared_ptr<const FcitxInputProcessor> proc;
            // Create new proc if not exist or expired
            if (iter == WeakRefs.end() || !(proc = (*iter).second.lock())) {
                proc         = std::make_shared<FcitxInputProcessor>(ctrl, im);
                WeakRefs[im] = proc;
            }

            return proc;
        }

      public:
        virtual void setActivated() const override
        {
            ctrl.SetCurrentIM(im.uniqueName);
        }
    };
    std::map<Fcitx::InputMethodInfo,
             std::weak_ptr<const FcitxInputProcessor>,
             FcitxInputProcessor::CompareInputMethodInfo>
        FcitxInputProcessor::WeakRefs = {};
}// namespace libdbus