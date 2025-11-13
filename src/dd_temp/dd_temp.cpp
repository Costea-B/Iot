#include <Arduino.h>
#include <SimpleDHT.h>
#include "dd_temp.h"

// === Configurare pin și senzor ===
#define DHTPIN 5   // pin digital
SimpleDHT11 dht(DHTPIN);
static TempData currentData;

// === Inițializare senzor ===
void Temp_Init() {
    delay(1000); // timp pentru stabilizare
    currentData.sensor_ok = true;
    Serial.println("Senzor DHT11 initializat (SimpleDHT)");
}

// === Citire temperatură și umiditate ===
float Temp_Read(TempData *data) {
    byte temperature = 0;
    byte humidity = 0;

    int err = dht.read(&temperature, &humidity, NULL);
    if (err != SimpleDHTErrSuccess) {
        data->sensor_ok = false;
        data->temperature = 0;
        data->humidity = 0;
        Serial.println("Eroare la citirea senzorului DHT!");
        return NAN;
    } else {
        data->sensor_ok = true;
        data->temperature = (float)temperature;
        data->humidity = (float)humidity;

        Serial.print("Temperatura: ");
        Serial.print(data->temperature);
        Serial.print(" °C | Umiditate: ");
        Serial.print(data->humidity);
        Serial.println(" %");
        return data->temperature;
    }
}
