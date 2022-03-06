#pragma once
#include <algorithm>
#include <list>
#include <map>

#include "IngameIME.hpp"
#include "InputProcessor.hpp"

namespace IngameIME {
    class InputProcessorImpl : public IngameIME::InputProcessor {
      public:
        virtual void setActivated() const override {}
    };
}// namespace IngameIME