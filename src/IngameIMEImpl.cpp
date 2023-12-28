#include <IngameIME.hpp>

#include "fcitx/fcInputContextImpl.hpp"

IngameIME::InputContext* IngameIME::CreateInputContextFcitx(std::string program)
{
    return new IngameIME::fcitx::InputContextImpl(program);
}