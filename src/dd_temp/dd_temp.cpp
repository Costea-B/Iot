#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <DHT.h>
#include "dd_filter_saltpepper/dd_filter_saltpepper.h"
#include "dd_temp.h"

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
static TempData currentData;

// Declarăm extern semafoarele definite în main.cpp
//SemaphoreHandle_t tempSemaphore;
//SemaphoreHandle_t sonicSemaphore;

// Buffer pentru filtrul ponderat
WeightedAvgData tempData = { {0}, {0.1, 0.15, 0.2, 0.25, 0.3}, SAMPLES_NUM-1 };


// Inițializare senzor
void Temp_Init() {
    dht.begin();
    currentData.sensor_ok = true;
}

// Citirea temperaturii
void Temp_Read(TempData *data) {
    float temp = dht.readTemperature();

    if (isnan(temp)) {
        data->sensor_ok = false;
        data->temperature = 0;
        Serial.println("Eroare la citirea senzorului DHT!");
    } else {
        data->sensor_ok = true;
        data->temperature = temp;
        Serial.print("Temperature: "); Serial.print(temp);
    }
}

// Task FreeRTOS
void Temp_Task(void *pvParameters) {
    Serial.println("Temp task started");
    const size_t SAMPLE_COUNT = 5;   // colectăm 5 valori pentru filtru
    float samples[SAMPLE_COUNT];

    while (true) {
        // Așteaptă semaforul
        if (xSemaphoreTake(tempSemaphore, portMAX_DELAY) == pdTRUE) {

            // Citim 5 valori
            for (size_t i = 0; i < SAMPLE_COUNT; i++) {
                Temp_Read(&currentData);
                samples[i] = currentData.temperature;
                vTaskDelay(pdMS_TO_TICKS(100));
            }

            // Aplicăm filtrul median
            float filteredTemp = SaltPepperFilter(samples, SAMPLE_COUNT);
             // Aplicăm filtrul ponderat
            float filteredTemp2 = weightedMovingAverageFilter(tempData, currentData.temperature);

            Serial.print("Temperatură filtrată cu sare si piper: ");
            Serial.print(filteredTemp, 2);
            Serial.println(" °C");
            Serial.print("Temperatură filtrată mediana: ");
            Serial.print(filteredTemp2, 2);
            Serial.println(" °C");

            // Dăm drumul la Ultrasonic_Task
            xSemaphoreGive(sonicSemaphore);
        }

        vTaskDelay(pdMS_TO_TICKS(10)); // scurt delay ca să nu blocheze complet scheduler-ul
    }
}
