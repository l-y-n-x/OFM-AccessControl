#pragma once

#include "KeypadBase.h"
#include "BS811X.h"

#ifdef KEYPAD_PCA9633_ADDR
    #include "PCA9633.h"
#endif

#define BS8116_I2C_CLOCK 100000
#define READ_KEYS_INTERVAL 50          // poll interval while the chip is responsive (normal mode)
#define READ_KEYS_INTERVAL_STANDBY 250 // backed-off interval while the chip is in standby (clock-stretching)

class KeypadForGira : public KeypadBase
{
  public:
    KeypadForGira();
    void init(bool testMode = false) override;
    void loop(bool testMode = false) override;
    void setInfoLed(uint32_t ledColor) override;
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
    bool tryApplyConfig();

    uint16_t _lastKeymap = 0;
    bool _configVerified = false;
    uint32_t _configRetryTimer = 0;
    uint32_t _readKeysTimer = 0;
    uint32_t _readKeysInterval = READ_KEYS_INTERVAL;
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
