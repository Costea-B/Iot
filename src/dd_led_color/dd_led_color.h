#ifndef DD_LED_COLOR_H
#define DD_LED_COLOR_H

#include <Arduino.h>

#define GREEN_LED_PIN 14
#define RED_LED_PIN   15

#ifdef __cplusplus
extern "C" {
#endif

void Led_ColorInit(void);
void Led_Green_On(void);
void Led_Green_Off(void);
void Led_Red_On(void);
void Led_Red_Off(void);

#ifdef __cplusplus
}
#endif

#endif