#ifndef DD_LED_H
#define DD_LED_H

#include <Arduino.h>

#define LED1_PIN 2
#define LED2_PIN 3

void Led_Init(void);
void Led1_Set(bool state);
void Led1_Toggle(void);
void Led2_Set(bool state);

#endif
