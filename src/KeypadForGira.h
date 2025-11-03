#pragma once

#include <functional>
#include "Arduino.h"
#include "OpenKNX.h"
#include "hardware.h"
#include "knxprod.h"
#include "BS811X.h"

#ifdef KEYPAD_PCA9633_ADDR
    #include "PCA9633.h"
#endif

class KeypadForGira
{
  public:
    using KeyPressedCallback = std::function<void(char)>;

    std::string logPrefix();
    void init(bool testMode = false);
    void loop(bool testMode = false);
    void registerCallback(KeyPressedCallback callback);

#ifdef KEYPAD_PCA9633_ADDR
    void runLedTestSequence();
#endif

  private:
    char mapKey(uint8_t index) const;

    KeyPressedCallback _callback;
    bool _initialized = false;
    uint16_t _lastKeymap = 0;
    BS811X _keypad;

#ifdef KEYPAD_PCA9633_ADDR
    PCA9633 _ledController = PCA9633(REG_PWM0, REG_PWM1, REG_PWM2, REG_PWM3);
    bool _ledInitialized = false;
#endif
};
