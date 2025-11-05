#include <utility>
#include "KeypadBase.h"

KeypadBase::KeypadBase(){}

std::string KeypadBase::logPrefix()
{
    return "Keypad";
}

void KeypadBase::registerCallback(KeyPressedCallback callback)
{
    _callback = std::move(callback);
}

void KeypadBase::runTestMode() {}
