#include <utility>
#include "KeypadForGira.h"

std::string KeypadForGira::logPrefix()
{
    return "Keypad";
}

void KeypadForGira::init(bool testMode)
{
    // if (ParamACC_NfcScanner == 0)
    //     return;

#ifdef KEYPAD_PCA9633_ADDR
    _ledController.begin(KEYPAD_PCA9633_ADDR, &OPENKNX_GPIO_WIRE);
    _ledController.setLdrStateAll(LDR_STATE_IND);
    _ledController.setRGBW(0, 0, 0, 128);
    _ledInitialized = true;
    logInfoP("Initialized PCA9633.");
#endif

    if (_keypad.begin("8116"))
        logInfoP("Initialized BS8116.");
    else
        logInfoP("Failed to initialize BS8116.");

    _lastKeymap = 0;
    _initialized = true;
}

void KeypadForGira::loop(bool testMode)
{
    if (!_initialized)
        return;

    // if (!testMode && ParamACC_NfcScanner == 0)
    //     return;
    
    uint16_t keymap = _keypad.readKeys();

    if (testMode)
    {
        char map[16];
        for (uint8_t i = 0; i < 16; i++)
            map[i] = (keymap & (1 << i)) ? '1' : '0';
        //map[16] = '\0';

        Serial.print("Key status:");
        Serial.print(map);

        uint8_t key = _keypad.getKey_active();
        Serial.print("  Pressed key: ");
        Serial.println(key);

        // if (_keypad.getKey_passive(12))
        //     Serial.println("Key 12 is pressed!");

        // if (_keypad.getKey_edge(1, 2))
        //     Serial.println("Key 2 rising edge detected!");
        // else if (_keypad.getKey_edge(2, 2))
        //     Serial.println("Key 2 falling edge detected!");

        delay(1000);
    }

    uint16_t newlyPressed = keymap & ~_lastKeymap;
    _lastKeymap = keymap;

    if (!_callback || newlyPressed == 0)
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

void KeypadForGira::registerCallback(KeyPressedCallback callback)
{
    _callback = std::move(callback);
}

#ifdef KEYPAD_PCA9633_ADDR
void KeypadForGira::runLedTestSequence()
{
    if (!_ledInitialized)
        return;
    
    logDebugP("RGB with background, all max");
    _ledController.setRGBW(255, 255, 255, 0);
    delay(1000);

    logDebugP("Red only");
    _ledController.setRGBW(255, 0, 0, 255);
    delay(1000);

    logDebugP("Green only");
    _ledController.setRGBW(0, 255, 0, 255);
    delay(1000);

    logDebugP("Blue only");
    _ledController.setRGBW(0, 0, 255, 255);
    delay(1000);

    logDebugP("Background only max");
    _ledController.setRGBW(0, 0, 0, 0);
    delay(1000);

    logDebugP("Background only 50%");
    _ledController.setRGBW(0, 0, 0, 128);
    delay(1000);

    logDebugP("Background only 25%");
    _ledController.setRGBW(0, 0, 0, 196);
    delay(1000);
}
#endif

char KeypadForGira::mapKey(uint8_t index) const
{
    static constexpr char lookup[] = {'F', '3', 'B', '6', '5', '4', '7', '8', '9', 'K', '*', '0', '#', 'C', '2', '1'};
    if (index < sizeof(lookup))
        return lookup[index];
    return '?';
}
