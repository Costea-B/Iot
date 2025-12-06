// =====================================================
// MCU SLAVE - Arduino Mega 2560
// Lab 7.1: Comunicație I²C cu senzor ultrasonic HC-SR04
// =====================================================

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <Wire.h>

// Definire pini pentru HC-SR04
#define TRIG_PIN 8
#define ECHO_PIN 9

// Adresa I2C slave
#define I2C_SLAVE_ADDRESS 0x08

// Structură pachet de date
typedef struct {
    uint8_t head;       // Header: 0xAA
    uint8_t length;     // Lungime payload
    uint16_t distance;  // Distanța măsurată (cm)
    uint8_t checksum;   // Suma de control
} DataPacket;

// Buffer partajat pentru răspuns I²C
volatile DataPacket dataBuffer;
volatile bool dataReady = false;

// Prototipuri taskuri FreeRTOS
void TaskReadUltrasonic(void *pvParameters);
void TaskUpdateBuffer(void *pvParameters);

// Prototip callback I²C
void requestEvent();

// Variabilă globală pentru distanța citită
volatile uint16_t lastDistance = 0;

void setup() {
    Serial.begin(9600);
    Serial.println("SLAVE: Initializare...");
    
    // Configurare pini HC-SR04
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    
    // Inițializare buffer
    dataBuffer.head = 0xAA;
    dataBuffer.length = sizeof(uint16_t);
    dataBuffer.distance = 0;
    dataBuffer.checksum = 0;
    
    // Configurare I2C ca slave
    Wire.begin(I2C_SLAVE_ADDRESS);
    Wire.onRequest(requestEvent);
    
    Serial.println("SLAVE: I2C initializat la adresa 0x08");
    
    // Creare taskuri FreeRTOS
    xTaskCreate(
        TaskReadUltrasonic,
        "ReadUltrasonic",
        128,
        NULL,
        2,  // Prioritate mai mare pentru achiziție
        NULL
    );
    
    xTaskCreate(
        TaskUpdateBuffer,
        "UpdateBuffer",
        128,
        NULL,
        1,  // Prioritate mai mică pentru actualizare
        NULL
    );
    
    Serial.println("SLAVE: Taskuri FreeRTOS create");
}

void loop() {
    // Loop-ul este controlat de FreeRTOS
}

// ===== TASK 1: Citire periodică senzor ultrasonic =====
void TaskReadUltrasonic(void *pvParameters) {
    (void) pvParameters;
    
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(500); // Citire la fiecare 500ms
    
    for (;;) {
        // Trigger puls
        digitalWrite(TRIG_PIN, LOW);
        delayMicroseconds(2);
        digitalWrite(TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG_PIN, LOW);
        
        // Citire durată ecou
        long duration = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout 30ms
        
        // Calcul distanță (cm)
        uint16_t distance = 0;
        if (duration > 0) {
            distance = duration * 0.034 / 2;
        } else {
            distance = 9999; // Valoare eroare
        }
        
        // Actualizare distanță
        lastDistance = distance;
        
        Serial.print("SLAVE: Distanta citita: ");
        Serial.print(distance);
        Serial.println(" cm");
        
        // Așteaptă până la următoarea perioadă
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

// ===== TASK 2: Actualizare buffer pentru I²C =====
void TaskUpdateBuffer(void *pvParameters) {
    (void) pvParameters;
    
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(100); // Actualizare la fiecare 100ms
    
    for (;;) {
        // Actualizare buffer cu datele cele mai recente
        dataBuffer.distance = lastDistance;
        
        // Calculare checksum (XOR simplu)
        dataBuffer.checksum = dataBuffer.head ^ dataBuffer.length ^ 
                              (dataBuffer.distance >> 8) ^ (dataBuffer.distance & 0xFF);
        
        dataReady = true;
        
        // Așteaptă până la următoarea perioadă
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

// ===== Callback I²C: Răspuns la cerere de la master =====
void requestEvent() {
    if (dataReady) {
        // Trimitere pachet complet
        Wire.write((uint8_t*)&dataBuffer, sizeof(DataPacket));
        
        Serial.print("SLAVE: Trimis pachet I2C - Distanta: ");
        Serial.print(dataBuffer.distance);
        Serial.print(" cm, Checksum: 0x");
        Serial.println(dataBuffer.checksum, HEX);
    } else {
        // Trimite pachet gol dacă nu sunt date valide
        DataPacket emptyPacket = {0xAA, sizeof(uint16_t), 0, 0};
        Wire.write((uint8_t*)&emptyPacket, sizeof(DataPacket));
    }
}
