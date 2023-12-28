#include "fcitx/fcInputContextImpl.hpp"

#include <array>
#include <bits/uses_allocator_args.h>
#include <cstdint>
#include <string>

#include "IngameIME.hpp"
#include "sdbus-c++/IConnection.h"
#include "sdbus-c++/IProxy.h"
#include "sdbus-c++/Message.h"
#include "sdbus-c++/Types.h"

static const char* FCITX_BUS_NAME         = "org.fcitx.Fcitx5";
static const char* FCITX_CONTROLLER_PATH  = "/controller";
static const char* FCITX_INPUTMETHOD_PATH = "/org/freedesktop/portal/inputmethod";
static const char* FCITX_INPUTMETHOD_INF  = "org.fcitx.Fcitx.InputMethod1";
static const char* FCITX_INPUTCONTEXT_INF = "org.fcitx.Fcitx.InputContext1";
static const char* FCITX_CONTROLLER_INF   = "org.fcitx.Fcitx.Controller1";

namespace IngameIME::fcitx
{
// https://github.com/fcitx/fcitx5/blob/a4e4311226ead198a17248a62ac1ad05a964d43c/src/lib/fcitx-utils/capabilityflags.h#L21
enum class CapabilityFlag : uint64_t
{
    NoFlag                 = 0,
    // Deprecated, because this flag is not compatible with fcitx 4.
    ClientSideUI           = (1 << 0),
    Preedit                = (1 << 1),
    ClientSideControlState = (1 << 2),
    Password               = (1 << 3),
    FormattedPreedit       = (1 << 4),
    ClientUnfocusCommit    = (1 << 5),
    SurroundingText        = (1 << 6),
    Email                  = (1 << 7),
    Digit                  = (1 << 8),
    Uppercase              = (1 << 9),
    Lowercase              = (1 << 10),
    NoAutoUpperCase        = (1 << 11),
    Url                    = (1 << 12),
    Dialable               = (1 << 13),
    Number                 = (1 << 14),
    NoOnScreenKeyboard     = (1 << 15),
    SpellCheck             = (1 << 16),
    NoSpellCheck           = (1 << 17),
    WordCompletion         = (1 << 18),
    UppercaseWords         = (1 << 19),
    UppwercaseSentences    = (1 << 20),
    Alpha                  = (1 << 21),
    Name                   = (1 << 22),
    GetIMInfoOnFocus       = (1 << 23),
    RelativeRect           = (1 << 24),
    // 25 ~ 31 are reserved for fcitx 4 compatibility.
    // New addition in fcitx 5.
    Terminal               = (1ull << 32),
    Date                   = (1ull << 33),
    Time                   = (1ull << 34),
    Multiline              = (1ull << 35),
    Sensitive              = (1ull << 36),
    KeyEventOrderFix       = (1ull << 37),
    /**
     * Whether client will set KeyState::Repeat on the key event.
     *
     * @see KeyState::Repeat
     * @since 5.0.4
     */
    ReportKeyRepeat        = (1ull << 38),
    /**
     * @brief Whether client display input panel by itself.
     *
     * @since 5.0.5
     */
    ClientSideInputPanel   = (1ull << 39),
    /**
     * Whether client request input method to be disabled.
     *
     * Usually this means only allow to type with raw keyboard.
     *
     * @since 5.0.20
     */
    Disable                = (1ull << 40),
    /**
     * Whether client support commit string with cursor location.
     * @since 5.1.2
     */
    CommitStringWithCursor = (1ull << 41),
    PasswordOrSensitive    = Password | Sensitive,
};

uint64_t operator|(CapabilityFlag flag1, CapabilityFlag flag2)
{
    return (uint64_t)flag1 | (uint64_t)flag2;
}

InputContextImpl::InputContextImpl(std::string program)
{
    conn       = sdbus::createSessionBusConnection();
    controller = sdbus::createProxy(*conn, FCITX_BUS_NAME, FCITX_CONTROLLER_PATH);

    auto InputMethodProxy = sdbus::createProxy(*conn, FCITX_BUS_NAME, FCITX_INPUTMETHOD_PATH);

    using Arg = sdbus::Struct<std::string, std::string>;

    sdbus::ObjectPath inputCtxPath;

    InputMethodProxy->callMethod("CreateInputContext")
        .onInterface(FCITX_INPUTMETHOD_INF)
        .withArguments(std::array<Arg, 1>{
            Arg{"program", program}
    })
        .storeResultsTo(inputCtxPath);

    inputCtx = sdbus::createProxy(*conn, FCITX_BUS_NAME, inputCtxPath);
    inputCtx->callMethod("SetSupportedCapability")
        .onInterface(FCITX_INPUTCONTEXT_INF)
        .withArguments(CapabilityFlag::Disable | CapabilityFlag::ClientSideInputPanel)
        .storeResultsTo();
    inputCtx->callMethod("SetCapability")
        .onInterface(FCITX_INPUTCONTEXT_INF)
        .withArguments(CapabilityFlag::Disable | CapabilityFlag::ClientSideInputPanel)
        .storeResultsTo();

    // Disable at initial
    inputCtx->callMethod("FocusIn").onInterface(FCITX_INPUTCONTEXT_INF).storeResultsTo();
    controller->callMethod("Activate").onInterface(FCITX_CONTROLLER_INF).storeResultsTo();
}

InputContextImpl::~InputContextImpl()
{
    inputCtx->callMethod("DestroyIC").storeResultsTo();
}

InputMode InputContextImpl::getInputMode()
{
    return inputMode;
}

void InputContextImpl::setActivated(const bool activated)
{
    if (this->activated == activated) return;

    if (activated)
    {
        // why clang-format put it at the same line??
        inputCtx->callMethod("FocusIn").onInterface(FCITX_INPUTCONTEXT_INF).storeResultsTo();
    }
    else
    {
        inputCtx->callMethod("FocusIn").onInterface(FCITX_INPUTCONTEXT_INF).storeResultsTo();
    }
    this->activated = activated;
}

bool InputContextImpl::getActivated() const
{
    return activated;
}
} // namespace IngameIME::fcitx
