#include "KeypadMatrix3x4.h"
#include "OpenKNX.h"
#include <utility>

KeypadMatrix3x4::KeypadMatrix3x4()
    : KeypadBase()
{
    _keypad = new MatrixKeypad(
        4,
        3,
        (const uint16_t[]){OPENKNX_ACC_KEYPAD_ROW_PINS},
        (const uint16_t[]){OPENKNX_ACC_KEYPAD_COL_PINS},
        (const char[]){'1', '2', '3', '4', '5', '6', '7', '8', '9', '*', '0', '#'},
        [this](char key) { if (_callback) _callback(key); },
        50);
}

void KeypadMatrix3x4::init(bool testMode)
{
    _lastKeymap = 0;
    _initialized = true;
}

void KeypadMatrix3x4::loop(bool testMode)
{
    if (!_initialized)
        return;

    // first call base (necessary for correct effect evaluation)
    KeypadBase::loop(testMode);
    _keypad->loop();
}

void KeypadMatrix3x4::setInfoLed(uint32_t ledColor)
{
    return;
}

void KeypadMatrix3x4::setBackgroundLed(uint8_t brightness)
{
    return;
}