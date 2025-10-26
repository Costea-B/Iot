#include "dd_led.h"

void Led_Init(void) {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
}

void Led1_Set(bool state) {
  digitalWrite(LED1_PIN, state ? HIGH : LOW);
}

void Led1_Toggle(void) {
  digitalWrite(LED1_PIN, !digitalRead(LED1_PIN));
}

void Led2_Set(bool state) {
  digitalWrite(LED2_PIN, state ? HIGH : LOW);
}
