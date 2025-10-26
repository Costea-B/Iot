#include "dd_led_color.h"

void Led_ColorInit(void) {
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    Led_Green_Off();
    Led_Red_Off();
}

void Led_Green_On(void) {
    digitalWrite(GREEN_LED_PIN, HIGH);
}

void Led_Green_Off(void) {
    digitalWrite(GREEN_LED_PIN, LOW);
}

void Led_Red_On(void) {
    digitalWrite(RED_LED_PIN, HIGH);
}

void Led_Red_Off(void) {
    digitalWrite(RED_LED_PIN, LOW);
}