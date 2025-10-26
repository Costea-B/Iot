#include "dd_lcd.h"
#include <LiquidCrystal.h>

// Modifică pinii după necesitate!
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void LCD_Init(void) {
    lcd.begin(16, 2);
    lcd.clear();
}

void LCD_WriteChar(char c) {
    lcd.write(c);
}

void LCD_Clear(void) {
    lcd.clear();
}