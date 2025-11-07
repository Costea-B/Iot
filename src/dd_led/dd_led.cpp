#include "dd_led.h"

static bool ledState = false;

void Led_Init(void) {
    pinMode(RELAY_PIN, OUTPUT);
    Led_Set(false); // LED oprit la start
}

void Led_Set(bool state) {
    digitalWrite(RELAY_PIN, state ? HIGH : LOW); // HIGH = LED aprins
    ledState = state;
}

void Led_Toggle(void) {
    Led_Set(!ledState);
}

bool Led_GetState(void) {
    return ledState;
}
