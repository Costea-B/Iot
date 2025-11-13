#ifndef DD_RELAY_H
#define DD_RELAY_H

#include <Arduino.h>
#include <stdbool.h>

#define RELAY_CONTROL_PIN  6      // pinul Arduino conectat la + releu

void Relay_Init(void);
void Relay_On(void);
void Relay_Off(void);
void Relay_Toggle(void);
bool Relay_GetState(void);

#endif
