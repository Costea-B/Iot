#include "dd_releu.h"

static bool relayState = false; // true = releu activ

void Relay_Init(void) {
    pinMode(RELAY_CONTROL_PIN, OUTPUT);
    digitalWrite(RELAY_CONTROL_PIN, LOW); // releu oprit la start
    relayState = false;
}

void Relay_On(void) {
    digitalWrite(RELAY_CONTROL_PIN, HIGH); // alimentăm releul
    relayState = true;
}

void Relay_Off(void) {
    digitalWrite(RELAY_CONTROL_PIN, LOW); // oprim releul
    relayState = false;
}

void Relay_Toggle(void) {
    if(relayState) Relay_Off();
    else Relay_On();
}

bool Relay_GetState(void) {
    return relayState;
}
