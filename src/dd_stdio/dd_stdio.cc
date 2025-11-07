#include "dd_stdio.h"
#include <Arduino.h>
#include <stdio.h>

static int serial_putchar(char c, FILE* stream) {
    if (c == '\n') Serial.write('\r');
    Serial.write(c);
    return 0;
}

void init_stdio(void) {
    static FILE uartout = {0};
    fdev_setup_stream(&uartout, serial_putchar, NULL, _FDEV_SETUP_WRITE);
    stdout = &uartout;
}
