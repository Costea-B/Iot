#ifndef DD_KEYPAD_H
#define DD_KEYPAD_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

void Keypad_Init(void);
char Keypad_GetKey(void);

#ifdef __cplusplus
}
#endif

#endif