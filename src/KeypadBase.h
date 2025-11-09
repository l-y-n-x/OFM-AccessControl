#pragma once

#include <functional>
#include "Arduino.h"
#include "OpenKNX.h"
// #include "hardware.h"
// #include "knxprod.h"

// ETS parameter values
#define VAL_Keypad_Backlight_On 0
#define VAL_Keypad_Backlight_Keypress 1
#define VAL_Keypad_Backlight_Ko 2
#define VAL_Keypad_Backlight_Off 3

#define VAL_Keypad_BacklightIntensity_High 0
#define VAL_Keypad_BacklightIntensity_Middle 1
#define VAL_Keypad_BacklightIntensity_Low 2
#define VAL_Keypad_BacklightIntensity_Ko 3


class KeypadBase
{
  public:
    using KeyPressedCallback = std::function<void(char)>;
     
    enum FeedbackType : uint8_t
    {
        Off, // turn led off
        Kepress, // called on keypad key press
        ActionOk, // action channel success
        ActionNotFound, // action channel not found
        CodeUnknown, // keypad code unknown
        CodeDeleted, // keypad code deleted by user
        PauseExceeded, // keypad code entry pause time exceeded
        Ok, // general success
        Failed, // general failure
        ButtonPress, // keypress on special button (i.e. Bell)
        WaitForCode // authorization action was called 
    };

    enum LedEffect : uint8_t
    {
        NoEffect = 0,
        Blink
    };


    KeypadBase();
    virtual std::string logPrefix();
    virtual void init(bool testMode = false) = 0;
    virtual void loop(bool testMode = false);
    virtual void registerCallback(KeyPressedCallback callback);
    virtual void setFeedback(FeedbackType feedbackType);
    virtual void setBacklight(bool on);
    virtual bool getBacklight();
    virtual void setInfoLed(uint32_t ledColor) = 0;
    virtual void setBackgroundLed(uint8_t brightness) = 0;
    virtual void runTestMode();

  protected:
    KeyPressedCallback _callback;
    bool _initialized = false;

  private:
    uint32_t _ledTimer = 0;
    uint16_t _ledDuration = 0;
    LedEffect _ledEffect = LedEffect::NoEffect;
    bool _ledBlinkState = false;
    uint32_t _ledOnColor = 0;
    uint32_t _ledOffColor = 0;
    uint32_t _ledPulse = 0;
    uint32_t _ledPause = 0;
    bool _backlight = false;
    uint32_t _backlightTimer = 0;

};
