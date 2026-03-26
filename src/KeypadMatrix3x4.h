#pragma once

#include "KeypadBase.h"
#include "MatrixKeypad.h"


class KeypadMatrix3x4 : public KeypadBase
{
  public:
    KeypadMatrix3x4();
    void init(bool testMode = false) override;
    void loop(bool testMode = false) override;
    void setInfoLed(uint32_t ledColor) override;
    void setBackgroundLed(uint8_t brightness) override;


  private:
    char mapKey(uint8_t index) const;
    MatrixKeypad* _keypad = nullptr;

    uint16_t _lastKeymap = 0;
};
