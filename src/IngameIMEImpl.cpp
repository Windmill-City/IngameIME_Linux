#include <stdarg.h>
#include <stdexcept>

#include "StringUtil.hpp"

#include "FcitxCompositionImpl.hpp"
#include "FcitxIngameIMEImpl.hpp"

#include "WlCompositionImpl.hpp"
#include "WlIngameIMEImpl.hpp"

#include "XCompositionImpl.hpp"
#include "XIngameIMEImpl.hpp"

namespace IngameIME {
    x::InputContextImpl::InputContextImpl(Display* display, Window window) : display(display), window(window)
    {
        comp = std::make_shared<CompositionImpl>(this);

        if (xic) XDestroyIC(xic);
        if (xim) XCloseIM(xim);

        if (!(xim = XOpenIM(display, NULL, NULL, NULL))) throw std::runtime_error("No input method could be opened!");

        XIMCallback cbPreeditStart{(XPointer)this, (XIMProc)CompositionImpl::PreeditStartCallback};
        XIMCallback cbPreeditDraw{(XPointer)this, (XIMProc)CompositionImpl::PreeditDrawCallback};
        XIMCallback cbPreeditCaret{(XPointer)this, (XIMProc)CompositionImpl::PreeditCaretCallback};
        XIMCallback cbPreeditDone{(XPointer)this, (XIMProc)CompositionImpl::PreeditDoneCallback};
        xic = XCreateIC(xim,
                        XNInputStyle,
                        XIMPreeditCallbacks | XIMStatusNothing,
                        XNPreeditStartCallback,
                        cbPreeditStart,
                        XNPreeditDrawCallback,
                        cbPreeditDraw,
                        XNPreeditCaretCallback,
                        cbPreeditCaret,
                        XNPreeditDoneCallback,
                        cbPreeditDone,
                        NULL);
        if (!xic) {
            XIMCallback cbGeometry{(XPointer)this, (XIMProc)CompositionImpl::GeometryCallback};
            xic = XCreateIC(xim, XNInputStyle, XIMPreeditArea | XIMStatusNothing, XNGeometryCallback, cbGeometry, NULL);
        }
        if (!xic) xic = XCreateIC(xim, XNInputStyle, XIMPreeditNothing | XIMStatusNothing, NULL);
        if (!xic) throw std::runtime_error("Failed to create Input Context!");

        XSetICValues(xic, XNClientWindow, window, XNFocusWindow, window, NULL);
    }

    wl::InputContextImpl::InputContextImpl(zwp_text_input_manager_v3* mgr, wl_seat* seat, wl_surface* surface)
        : surface(surface)
    {
        textInput = zwp_text_input_manager_v3_get_text_input(mgr, seat);
        comp      = std::make_shared<CompositionImpl>(this);
    }

    Global& getInstanceInternal(bool is_wayland, va_list args)
    {
        simppl::dbus::enable_threads();
        return
            // is_wayland ? (Global&)*new libwl::GlobalImpl(va_arg(args, wl_display*)) :
            //  (Global&)*new libxim::GlobalImpl(va_arg(args, Display*));
            *new dbus::fcitx::GlobalImpl();
    }

    Global& Global::getInstance(void* is_wayland, ...)
    {
        va_list args;
        va_start(args, is_wayland);

        thread_local Global& Instance = getInstanceInternal(is_wayland, args);

        va_end(args);

        return Instance;
    }
}// namespace IngameIME