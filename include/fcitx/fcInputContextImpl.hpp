#pragma once
#include "sdbus-c++/IProxy.h"
#include <IngameIME.hpp>
#include <memory>
#include <sdbus-c++/sdbus-c++.h>

namespace IngameIME::fcitx
{
class InputContextImpl : public InputContext
{
  protected:
    std::unique_ptr<sdbus::IConnection> conn;
    std::unique_ptr<sdbus::IProxy>      controller;
    std::unique_ptr<sdbus::IProxy>      inputCtx;

    bool                 activated = false;
    IngameIME::InputMode inputMode = IngameIME::InputMode::AlphaNumeric;

  public:
    InputContextImpl(std::string program);
    ~InputContextImpl();

  public:
    virtual InputMode getInputMode() override;
    virtual void      setActivated(const bool activated) override;
    virtual bool      getActivated() const override;
};
} // namespace IngameIME::fcitx