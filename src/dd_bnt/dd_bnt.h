#ifndef DD_BUTTON_H
#define DD_BUTTON_H

#include <Arduino.h>

#define BTN_TOGGLE 4   // Buton pentru LED1
#define BTN_INC    5   // Buton pentru incrementare
#define BTN_DEC    6   // Buton pentru decrementare

void Button_Init(void);
int Button_Toggle_Read(void);
int Button_Inc_Read(void);
int Button_Dec_Read(void);

#endif
