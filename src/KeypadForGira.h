#pragma once

#include "KeypadBase.h"
#include "BS811X.h"

#ifdef KEYPAD_PCA9633_ADDR
    #include "PCA9633.h"
#endif

class KeypadForGira : public KeypadBase
{
  public:
    KeypadForGira();
    void init(bool testMode = false) override;
    void loop(bool testMode = false) override;
    void setInfoLed(uint8_t red, uint8_t green, uint8_t blue) override;
    void setBackgroundLed(uint8_t brightness) override;
    
#ifdef KEYPAD_PCA9633_ADDR
    void runTestMode() override;
#endif

  // protected:
  //   KeyPressedCallback _callback;
  //   bool _initialized = false;

  private:
    char mapKey(uint8_t index) const;
    void updateLeds();

    uint16_t _lastKeymap = 0;
    uint32_t _lastKeypress = 0;
    BS811X _keypad;

#ifdef KEYPAD_PCA9633_ADDR
    PCA9633 _ledController = PCA9633(REG_PWM0, REG_PWM1, REG_PWM2, REG_PWM3);
    bool _ledInitialized = false;
    uint8_t _ledRed = 0;
    uint8_t _ledGreen = 0;
    uint8_t _ledBlue = 0;
    uint8_t _ledBackground = 0;
#endif
};
