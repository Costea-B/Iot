#ifndef DD_TEMP_H
#define DD_TEMP_H

#include <Arduino.h>
#include <semphr.h>
#include "dd_filtre/dd_filtre.h"


// Structură pentru datele senzorului
typedef struct {
    float temperature;
    bool sensor_ok;
} TempData;

// Inițializare senzor
void Temp_Init();

// Achiziția temperaturii
void Temp_Read(TempData *data);

// Task FreeRTOS pentru achiziție recurentă
void Temp_Task(void *pvParameters);

extern SemaphoreHandle_t tempSemaphore;
extern SemaphoreHandle_t sonicSemaphore;

#endif
