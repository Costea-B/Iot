#ifndef DD_LED_H
#define DD_LED_H

#include <Arduino.h>
#include <stdbool.h>

#define RELAY_PIN  13   // pinul care controlează releul/LED-ul

void Led_Init(void);
void Led_Set(bool state);
void Led_Toggle(void);
bool Led_GetState(void);

#endif
