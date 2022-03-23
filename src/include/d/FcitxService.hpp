#pragma once
#include <simppl/interface.h>
#include <string>

namespace org::fcitx::fcitx5 {
    using namespace simppl::dbus;

    INTERFACE(FCITX)
    {
        Method<in<std::string>, out<std::string>> echo;
    };
}// namespace org::fcitx::fcitx5