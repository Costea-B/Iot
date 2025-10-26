#ifndef DD_LCD_H
#define DD_LCD_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

void LCD_Init(void);
void LCD_WriteChar(char c);
void LCD_Clear(void);

#ifdef __cplusplus
}
#endif

#endif