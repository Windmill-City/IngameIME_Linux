#include <stdarg.h>
#include <stdexcept>

#include "FormatUtil.hpp"

// #include "WlCompositionImpl.hpp"
#include "XCompositionImpl.hpp"

// #include "WlIngameIMEImpl.hpp"
#include "XIngameIMEImpl.hpp"

libxim::InputContextImpl::InputContextImpl(Display* display, Window window) : display(display), window(window)
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

IngameIME::Global& IngameIME::Global::getInstance()
{
    thread_local IngameIME::Global& Instance = (IngameIME::Global&)*new libxim::GlobalImpl();
    return Instance;
}