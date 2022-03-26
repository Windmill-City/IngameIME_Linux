#pragma once
#include <string>
#include <vector>

#include <simppl/dispatcher.h>
#include <simppl/interface.h>
#include <simppl/objectpath.h>
#include <simppl/pair.h>
#include <simppl/string.h>
#include <simppl/struct.h>
#include <simppl/stub.h>
#include <simppl/vector.h>

using namespace simppl::dbus;
namespace org::fcitx::Fcitx {
    struct InputMethodInfo
    {
        typedef make_serializer<std::string, std::string, std::string, std::string, std::string, std::string, bool>::
            type serializer_type;

        std::string uniqueName;
        std::string name;
        std::string nativeName;
        std::string icon;
        std::string label;
        std::string languageCode;
        bool        isConfigurable;
    };

    struct GroupInfo
    {
        typedef make_serializer<std::string, std::string>::type serializer_type;

        std::string name;
        std::string layout;
    };

    INTERFACE(Controller1)
    {
        Method<> Activate;
        Method<> Deactivate;

        Method<out<std::vector<InputMethodInfo>>>                              AvailableInputMethods;
        Method<out<std::string>>                                               CurrentInputMethod;
        Method<out<std::string>>                                               CurrentInputMethodGroup;
        Method<in<std::string>, out<std::string>, out<std::vector<GroupInfo>>> InputMethodGroupInfo;
        Method<in<std::string>>                                                SetCurrentIM;

        Method<out<std::string>> DebugInfo;

        Controller1()
            : INIT(Activate), INIT(Deactivate), INIT(AvailableInputMethods), INIT(CurrentInputMethod),
              INIT(CurrentInputMethodGroup), INIT(InputMethodGroupInfo), INIT(SetCurrentIM), INIT(DebugInfo)
        {
        }
    };

    INTERFACE(InputMethod1)
    {
        Method<in<std::vector<std::pair<std::string, std::string>>>,
               out<simppl::dbus::ObjectPath>,
               out<std::vector<std::byte>>>
            CreateInputContext;

        InputMethod1() : INIT(CreateInputContext) {}
    };

    INTERFACE(InputContext1)
    {
        Method<> DestroyIC;

        Method<> FocusIn;
        Method<> FocusOut;

        Method<> Reset;

        Method<in<int32_t>, in<int32_t>, in<int32_t>, in<int32_t>> SetCursorRect;

        Signal<std::string, std::string, std::string> CurrentIM;

        Signal<std::string> CommitString;

        Signal<std::vector<std::pair<std::string, int32_t>>,
               int32_t,
               std::vector<std::pair<std::string, int32_t>>,
               std::vector<std::pair<std::string, int32_t>>,
               std::vector<std::pair<std::string, std::string>>,
               int32_t,
               int32_t,
               bool,
               bool>
                                                                      UpdateClientSideUI;
        Signal<std::vector<std::pair<std::string, int32_t>>, int32_t> UpdateFormattedPreedit;

        InputContext1()
            : INIT(DestroyIC), INIT(FocusIn), INIT(FocusOut), INIT(Reset), INIT(SetCursorRect), INIT(CommitString),
              INIT(CurrentIM), INIT(UpdateClientSideUI), INIT(UpdateFormattedPreedit)
        {
        }
    };

#define Fcitx(Name, Inf, ObjPath)                                                                                      \
    struct Name : Stub<Inf>                                                                                            \
    {                                                                                                                  \
        Name(Dispatcher& disp) : Stub<Inf>(disp, "org.fcitx.Fcitx5", ObjPath) {}                                       \
    };

#define FcitxDynamic(Name, Inf)                                                                                        \
    struct Name : Stub<Inf>                                                                                            \
    {                                                                                                                  \
        Name(Dispatcher& disp, const char* objPath) : Stub<Inf>(disp, "org.fcitx.Fcitx5", objPath) {}                  \
    };

    Fcitx(Controller, Controller1, "/controller");
    Fcitx(InputMethod, InputMethod1, "/org/freedesktop/portal/inputmethod");
    FcitxDynamic(InputContext, InputContext1);
}// namespace org::fcitx::Fcitx
