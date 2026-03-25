#pragma once

#include "KeypadBase.h"
#include "BS811X.h"

#ifdef KEYPAD_PCA9633_ADDR
    #include "PCA9633.h"
#endif

class KeypadMatrix3x4 : public KeypadBase
{
  public:
    KeypadMatrix3x4();
    void init(bool testMode = false) override;
    void loop(bool testMode = false) override;  


  private:
    char mapKey(uint8_t index) const;

    uint16_t _lastKeymap = 0;
};
