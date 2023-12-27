#pragma once
#include <IngameIME.hpp>

namespace IngameIME::fcitx {
class InputContextImpl : public InputContext {

public:
  virtual InputMode getInputMode() override;
  virtual void setActivated(const bool activated) override;
  virtual bool getActivated() const override;
};
} // namespace IngameIME::fcitx