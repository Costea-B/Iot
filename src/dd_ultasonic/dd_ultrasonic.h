#ifndef DD_ULTRASONIC_H
#define DD_ULTRASONIC_H

#include <Arduino.h>
#include <semphr.h> 
#include "dd_filtre/dd_filtre.h"

typedef struct {
    float distance;
    bool sensor_ok;
} UltrasonicData;

void Ultrasonic_Init(int trigPin, int echoPin);
void Ultrasonic_Read(UltrasonicData *data);
void Ultrasonic_Task(void *pvParameters);

extern SemaphoreHandle_t tempSemaphore;
extern SemaphoreHandle_t sonicSemaphore;

extern WeightedAvgData ultrasonicData;

#endif
