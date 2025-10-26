#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <DHT.h>
#include "dd_filter_saltpepper/dd_filter_saltpepper.h"
#include "dd_ultrasonic.h"

//extern SemaphoreHandle_t tempSemaphore;
//extern SemaphoreHandle_t sonicSemaphore;

static int trigPin;
static int echoPin;


WeightedAvgData ultrasonicData = { {0}, {0.1, 0.15, 0.2, 0.25, 0.3}, SAMPLES_NUM-1 };

static UltrasonicData currentData;

void Ultrasonic_Init(int tPin, int ePin) {
    trigPin = tPin;
    echoPin = ePin;
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    currentData.sensor_ok = true;
    currentData.distance = 0.0;
}

void Ultrasonic_Read(UltrasonicData* data) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    data->distance = duration * 0.034 / 2; // cm
    data->sensor_ok = (duration > 0);
}
void Ultrasonic_Task(void *pvParameters) {
    const size_t SAMPLE_COUNT = 5;
    float samples[SAMPLE_COUNT];

    while(true) {
        if(xSemaphoreTake(sonicSemaphore, portMAX_DELAY) == pdTRUE) {
            // Citim ultrasonic
            Ultrasonic_Read(&currentData);

            // Mutăm buffer-ul cu ultimele valori
            for (size_t i = 0; i < SAMPLE_COUNT - 1; i++) {
                samples[i] = samples[i + 1];
            }
            samples[SAMPLE_COUNT - 1] = currentData.distance;

            // Filtrul median (Salt & Pepper)
            float filteredMedian = SaltPepperFilter(samples, SAMPLE_COUNT);

            // Filtrul ponderat
            float filteredWeighted = weightedMovingAverageFilter(ultrasonicData, currentData.distance);

            // Afișare rezultate
            Serial.print("Distance raw: "); Serial.println(currentData.distance, 2);
            Serial.print("Distance filtrată median: "); Serial.println(filteredMedian, 2);
            Serial.print("Distance filtrată ponderată: "); Serial.println(filteredWeighted, 2);
            Serial.print("Status: "); Serial.println(currentData.sensor_ok ? "OK" : "ERROR");

            // Dăm drumul la Temp_Task
            xSemaphoreGive(tempSemaphore);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
