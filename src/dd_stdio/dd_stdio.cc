#include "dd_stdio.h"

static FILE SerialOut;

static int serial_putchar(char c, FILE* f) {
  if (c == '\n') Serial.write('\r');  // conversie corectă pentru terminal
  Serial.write(c);
  return 0;
}

void init_stdio() {
  fdev_setup_stream(&SerialOut, serial_putchar, NULL, _FDEV_SETUP_WRITE);
  stdout = &SerialOut; // redirecționează printf() către Serial
}
