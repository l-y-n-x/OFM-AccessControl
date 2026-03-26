#include <utility>
#include "KeypadMatrix3x4.h"

KeypadMatrix3x4::KeypadMatrix3x4() {};

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


    
    uint16_t keymap = _keypad.readKeys();

    {
        char map[16];
        for (uint8_t i = 0; i < 16; i++)
            map[i] = (keymap & (1 << i)) ? '1' : '0';

        Serial.print("Key status:");
        Serial.print(map);

        uint8_t key = _keypad.getKey_active();
        Serial.print("  Pressed key: ");
        Serial.println(key);

        delay(1000);
    }

    uint16_t newlyPressed = keymap & ~_lastKeymap;

    // send also a key up event
    if (_callback && keymap == 0 && _lastKeymap != 0 && !(_lastKeymap & (_lastKeymap - 1)))
        _callback('\0');

    _lastKeymap = keymap;

    if (!_callback || newlyPressed == 0)
        return;
    // skip, if more than 1 bit is set (more than one key is pressed)
    if (newlyPressed & (newlyPressed-1))
        return;
        
    for (uint8_t index = 0; index < 16; ++index)
    {
        if (newlyPressed & (1 << index))
        {
            char resolved = mapKey(index);
            if (resolved != '\0')
                _callback(resolved);
        }
    }
}


void KeypadMatrix3x4::setInfoLed(uint32_t ledColor)
{
    return;
}

void KeypadMatrix3x4::setBackgroundLed(uint8_t brightness)
{
    return;
}


char KeypadMatrix3x4::mapKey(uint8_t index) const
{
    static constexpr char lookup[] = {'F', '3', 'B', '6', '5', '4', '7', '8', '9', 'K', '*', '0', '#', 'C', '2', '1'};
    if (index < sizeof(lookup))
        return lookup[index];
    return '\0';
}
