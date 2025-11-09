#pragma once

#include "KeypadBase.h"

class KeypadEmpty : public KeypadBase
{
  public:
    KeypadEmpty() {};
    void init(bool testMode = false) override {}
    void loop(bool testMode = false) override {}
    void setInfoLed(uint32_t ledColor) override {};
    void setBackgroundLed(uint8_t brightness) override {};
};
