#include "dd_bnt.h"

void Button_Init(void) {
  pinMode(BTN_TOGGLE, INPUT_PULLUP);
  pinMode(BTN_INC, INPUT_PULLUP);
  pinMode(BTN_DEC, INPUT_PULLUP);
}

int Button_Toggle_Read(void) {
  return digitalRead(BTN_TOGGLE) == LOW;
}

int Button_Inc_Read(void) {
  return digitalRead(BTN_INC) == LOW;
}

int Button_Dec_Read(void) {
  return digitalRead(BTN_DEC) == LOW;
}
