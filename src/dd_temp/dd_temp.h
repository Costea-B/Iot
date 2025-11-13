#ifndef DD_TEMP_H
#define DD_TEMP_H

#include <Arduino.h>
#include "dd_filter_saltpepper/dd_filter_saltpepper.h"

// Structură pentru datele senzorului
typedef struct {
    float temperature;
    bool sensor_ok;
    float humidity;
} TempData;

// Inițializare senzor
void Temp_Init();

// Citirea temperaturii brute
float Temp_Read(TempData *data);

#endif
