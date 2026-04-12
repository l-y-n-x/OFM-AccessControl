#pragma once

#include <cstdint>
#include <functional> // For std::function
#include <vector>     // For std::vector

using MatrixKeypadCallback = std::function<void(char)>;

class MatrixKeypad
{
  public:
    // Constructor:
    // numRows: Number of rows in the keypad
    // numCols: Number of columns in the keypad
    // rowPins: Array of GPIO pins connected to the rows (output)
    // colPins: Array of GPIO pins connected to the columns (input)
    // keyMap: 1D array mapping physical keys to characters
    // callback: Function to call when a key is pressed
    // debounceTime_ms: Time in milliseconds for debouncing (e.g., 50ms)
    MatrixKeypad(uint8_t numRows,
                 uint8_t numCols,
                 const uint16_t rowPins[],
                 const uint16_t colPins[],
                 const char keyMap[], // 1D array, size numRows * numCols
                 MatrixKeypadCallback callback,
                 uint16_t debounceTime_ms);

    // Destructor (std::vector handles memory cleanup)
    ~MatrixKeypad();

    void loop();

  private:
    uint8_t m_numRows;
    uint8_t m_numCols;

    // Pin configurations - dynamic and uint16_t
    std::vector<uint16_t> m_rowPins;
    std::vector<uint16_t> m_colPins;

    std::vector<char> m_keyMap;

    // Callback function
    MatrixKeypadCallback m_callback;

    // Debouncing variables
    uint16_t m_debounceCounter;
    const uint16_t m_debounceThreshold; // Number of loops for debounce
    bool m_keyIsPressed;                // True if a key is currently considered pressed (after debounce)
    char m_lastPressedKey;              // Stores the last key that was registered as pressed

    // Private helper for debouncing logic
    void handleDebounce(char currentKey);
};
