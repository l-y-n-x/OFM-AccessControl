#pragma once

#include <functional>
#include "Arduino.h"
#include "OpenKNX.h"
// #include "hardware.h"
// #include "knxprod.h"

class KeypadBase
{
  public:
    using KeyPressedCallback = std::function<void(char)>;
    KeypadBase();
    virtual std::string logPrefix();
    virtual void init(bool testMode = false) = 0;
    virtual void loop(bool testMode = false) = 0;
    virtual void registerCallback(KeyPressedCallback callback);
    virtual void setInfoLed(uint8_t red, uint8_t green, uint8_t blue) = 0;
    virtual void setBackgroundLed(uint8_t brightness) = 0;
    virtual void runTestMode();

  protected:
    KeyPressedCallback _callback;
    bool _initialized = false;
};
