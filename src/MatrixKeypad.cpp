// MatrixKeypad.cpp
#include "MatrixKeypad.h"
#include "OpenKNX.h"
#include "Feedback.h"

MatrixKeypad::MatrixKeypad(uint8_t numRows,
                           uint8_t numCols,
                           const uint16_t rowPins[],
                           const uint16_t colPins[],
                           const char keyMap[], // 1D array
                           MatrixKeypadCallback callback,
                           uint16_t debounceTime_ms)
    : m_numRows(numRows),
      m_numCols(numCols),
      m_rowPins(rowPins, rowPins + numRows),
      m_colPins(colPins, colPins + numCols),
      m_keyMap(keyMap, keyMap + (static_cast<size_t>(numRows) * numCols)),
      m_callback(callback),
      m_debounceCounter(0),
      m_keyIsPressed(false),
      m_lastPressedKey('\0'),
      m_debounceThreshold(debounceTime_ms / 5)
{
    // Initialize all row pins as outputs and set them high (inactive)
    for (uint8_t i = 0; i < m_numRows; ++i)
    {
        openknx.gpio.pinMode(m_rowPins[i], OUTPUT);
        openknx.gpio.digitalWrite(m_rowPins[i], HIGH); // Keep rows inactive (high)
    }

    // Initialize all column pins as inputs with pull-up resistors
    for (uint8_t i = 0; i < m_numCols; ++i)
    {
        openknx.gpio.pinMode(m_colPins[i], INPUT_PULLUP);
    }
}

MatrixKeypad::~MatrixKeypad()
{
    // No cleanup needed for std::vector members
}

void MatrixKeypad::loop()
{
    char currentKey = '\0'; // No key detected this scan cycle

    // Loop through each row
    for (uint8_t r = 0; r < m_numRows; ++r)
    {
        // Drive the current row low to activate it
        openknx.gpio.digitalWrite(m_rowPins[r], LOW);

        // Check all columns for a key press
        for (uint8_t c = 0; c < m_numCols; ++c)
        {
            // If a column pin is low, a key in this row/column is pressed
            if (openknx.gpio.digitalRead(m_colPins[c]) == LOW)
            {
                // Access keyMap using 1D indexing: row * numCols + col
                currentKey = m_keyMap[static_cast<size_t>(r) * m_numCols + c];
                break; // Exit the column loop
            }
        }

        // Set the current row back to high (inactive) before moving to the next row
        openknx.gpio.digitalWrite(m_rowPins[r], HIGH);

        // If a key has been found, exit the row loop
        if (currentKey != '\0')
        {
            break;
        }
    }

    // All rows have been scanned. Now handle debouncing.
    handleDebounce(currentKey);

    // Ensure all row pins are returned to a safe state (HIGH)
    // after the scan to prevent ghosting or unwanted current draw.
    for (uint8_t r = 0; r < m_numRows; ++r)
    {
        openknx.gpio.digitalWrite(m_rowPins[r], HIGH);
    }
}

void MatrixKeypad::handleDebounce(char currentKey)
{
    if (currentKey != '\0')
    { // A key is physically pressed
        // logDebug("MatrixKeypad","Key '%c' pressed", currentKey);
        if (!m_keyIsPressed)
        { // No key was previously considered pressed (debounced)
            m_debounceCounter++;
            // logDebug("MatrixKeypad","Debounce '%c' Cnt %d of %d", currentKey, m_debounceCounter, m_debounceThreshold);
            if (m_debounceCounter >= m_debounceThreshold)
            {
                // Key has been stable for long enough, register it as pressed
                m_keyIsPressed = true;
                m_lastPressedKey = currentKey;
                m_debounceCounter = 0; // Reset counter for next press/release cycle
                if (m_callback)
                {
                    m_callback(currentKey); // Trigger the callback!
                    openknxFeedback.setBuzzer(true);
                    //logDebug("MatrixKeypad", "Key '%c' pressed (debounced)", currentKey);
                }
            }
        }
        else if (m_keyIsPressed && currentKey == m_lastPressedKey)
        {
            // Key is still pressed and it's the same key, maintain state
            m_debounceCounter = 0; // Reset counter to prevent accidental release detection
        }
        else if (m_keyIsPressed && currentKey != m_lastPressedKey)
        {
            // A different key is now pressed while another was active.
            // This could be a "slide" or ghosting. Reset debounce for the new key.
            m_keyIsPressed = false;        // Treat as a new press sequence
            m_debounceCounter = 1;         // Start debouncing the new key
            m_lastPressedKey = currentKey; // Tentatively store the new key
        }
    }
    else
    { // No key is physically pressed
        if (m_keyIsPressed)
        {
            // A key was considered pressed, but now nothing is.
            // Start debouncing for release (though we don't care about release,
            // this helps reset the state for the next *new* press).
            m_debounceCounter++;
            if (m_debounceCounter >= m_debounceThreshold)
            {
                m_keyIsPressed = false;
                m_lastPressedKey = '\0';
                m_debounceCounter = 0;
            }
        }
        else
        {
            // Nothing pressed, nothing was pressed. Reset counter.
            m_debounceCounter = 0;
            m_lastPressedKey = '\0'; // Ensure it's clear
        }
    }
}