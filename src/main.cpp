#include <Arduino.h>
#include <DHT.h>
#include "dd_releu/dd_releu.h"
#include "dd_motor/dd_motor.h"
#include "dd_temp/dd_temp.h"

// ===== VARIABILE GLOBALE =====
float setPoint = 25.0;    // temperatura dorită (setabilă prin serial)
float hysteresis = 1.0;   // histerezis pentru control On-Off
float currentTemp = 0.0;  // temperatura actuală
String inputCommand = "";

TempData tempData; // structura pentru senzorul DHT

// ===== INITIALIZARE =====
void setup() {
    Serial.begin(9600);
    Temp_Init();
    Relay_Init();
    Motor_Init();

    Serial.println("=== Sistem de control temperatura (On-Off cu histerezis) ===");
    Serial.println("Comenzi disponibile:");
    Serial.println("  SET <valoare>     -> setează temperatura de referință");
    Serial.println("  STATUS            -> afișează starea curentă");
    Serial.println("---------------------------------------------------------");
}

// ===== FUNCȚIE DE CONTROL TEMPERATURĂ =====
void ControlTemperature() {
    if (!tempData.sensor_ok) {
        Serial.println("Eroare la senzorul DHT!");
        Relay_Off();
        Motor_Stop();
        return;
    }

    // Control On-Off cu histerezis
    if (currentTemp < (setPoint - hysteresis)) {
        Relay_On();       // pornește încălzirea
        Motor_Forward(200);
        Serial.println(">> Încălzire PORNITĂ");
    } 
    else if (currentTemp > (setPoint + hysteresis)) {
        Relay_Off();      // oprește încălzirea
        Motor_Stop();
        Serial.println(">> Încălzire OPRITĂ");
    }
}

// ===== BUCLE PRINCIPALĂ =====
void loop() {
    // Citire temperatură
    currentTemp = Temp_Read(&tempData);

    Serial.print("Temperatura curentă: ");
    Serial.print(currentTemp);
    Serial.print(" °C | SetPoint: ");
    Serial.print(setPoint);
    Serial.println(" °C");

    // Apel control
    ControlTemperature();

    // === Citire comenzi seriale ===
    if (Serial.available() > 0) {
        inputCommand = Serial.readStringUntil('\n');
        inputCommand.trim();

        if (inputCommand.startsWith("SET")) {
            int spaceIndex = inputCommand.indexOf(' ');
            if (spaceIndex != -1) {
                float val = inputCommand.substring(spaceIndex + 1).toFloat();
                if (val > 0 && val < 100) {
                    setPoint = val;
                    Serial.print("SetPoint actualizat la ");
                    Serial.print(setPoint);
                    Serial.println(" °C");
                } else {
                    Serial.println("Valoare invalida! Introdu o temperatura intre 0 si 100.");
                }
            } else {
                Serial.println("Format invalid! Foloseste: SET <valoare>");
            }
        } 
        else if (inputCommand.equalsIgnoreCase("STATUS")) {
            Serial.print("Temperatura: ");
            Serial.print(currentTemp);
            Serial.print(" °C | SetPoint: ");
            Serial.print(setPoint);
            Serial.print(" °C | Releu: ");
            Serial.println(Relay_GetState() ? "ON" : "OFF");
        } 
        else {
            Serial.println("Comanda necunoscuta!");
        }
    }

    delay(2000); // citire la fiecare 2 secunde
}
