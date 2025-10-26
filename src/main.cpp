#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "dd_bnt/dd_bnt.h"
#include "dd_led/dd_led.h"
#include "task/tasks.h"
#include "dd_temp/dd_temp.h"
#include "dd_ultasonic/dd_ultrasonic.h"
#include "dd_stdio/dd_stdio.h"
#include <semphr.h>

SemaphoreHandle_t tempSemaphore;
SemaphoreHandle_t sonicSemaphore;

void setup() {
    Serial.begin(9600);
    init_stdio();
    printf("System started...");

    tempSemaphore  = xSemaphoreCreateBinary();
    sonicSemaphore = xSemaphoreCreateBinary();

    // Pornim primul task
    xSemaphoreGive(tempSemaphore);

    Temp_Init();
    Ultrasonic_Init(2, 3);

    xTaskCreate(Temp_Task, "TempTask", 2048, NULL, 1, NULL);
    xTaskCreate(Ultrasonic_Task, "UltraTask", 2048, NULL, 1, NULL);

    printf("System ready...");
    vTaskStartScheduler();
}

void loop() {
    // nu folosim loop – FreeRTOS rulează task-urile
}
