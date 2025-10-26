#ifndef DD_FILTER_SALTPEPPER_H
#define DD_FILTER_SALTPEPPER_H

#include <Arduino.h>

// Aplica filtrul "sare și piper" pe un buffer de date
// data: buffer de valori
// length: dimensiunea bufferului
// windowSize: dimensiunea ferestrei (număr impar, ex. 5)
float SaltPepperFilter(float* data, size_t length);

#endif
