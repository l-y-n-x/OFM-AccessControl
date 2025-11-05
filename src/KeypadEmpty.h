#pragma once

#include "KeypadBase.h"

class KeypadEmpty : public KeypadBase
{
  public:
    KeypadEmpty() {};
    void init(bool testMode = false) override {}
    void loop(bool testMode = false) override {}
    void setInfoLed(uint8_t red, uint8_t green, uint8_t blue) override {};
    void setBackgroundLed(uint8_t brightness) override {};
};
