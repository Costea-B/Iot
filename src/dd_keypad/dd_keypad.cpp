#include "dd_keypad.h"
#include <Keypad.h>

// Modifică pinii după necesitate!
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'A','B','C','1'},
  {'3','8','5','2'},
  {'2','9','6','3'},
  {'1','4','B','A'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {13, 10, A1, A0};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void Keypad_Init(void) {
    // Nimic special, doar pentru compatibilitate
}

char Keypad_GetKey(void) {
    return keypad.getKey();
}