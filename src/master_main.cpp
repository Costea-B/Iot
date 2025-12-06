// =====================================================
// MCU MASTER - Arduino Mega 2560
// Lab 7.1: Comunicație I²C - Interogare și afișare
// =====================================================

#include <Arduino.h>
#include <Wire.h>

// Adresa I2C slave
#define I2C_SLAVE_ADDRESS 0x08

// Structură pachet de date (identică cu slave)
typedef struct {
    uint8_t head;       // Header: 0xAA
    uint8_t length;     // Lungime payload
    uint16_t distance;  // Distanța măsurată (cm)
    uint8_t checksum;   // Suma de control
} DataPacket;

// Variabile globale
DataPacket receivedData;
unsigned long lastRequest = 0;
const unsigned long requestInterval = 1000; // Interogare la fiecare 1000ms

void setup() {
    Serial.begin(9600);
    Serial.println("MASTER: Initializare...");
    
    // Inițializare I2C ca master
    Wire.begin();
    
    Serial.println("MASTER: I2C initializat ca master");
    Serial.println("MASTER: Asteapta date de la slave (0x08)...");
    Serial.println("========================================");
}

void loop() {
    unsigned long currentMillis = millis();
    
    // Interogare periodică a slave-ului
    if (currentMillis - lastRequest >= requestInterval) {
        lastRequest = currentMillis;
        
        // Cerere date de la slave
        Wire.requestFrom(I2C_SLAVE_ADDRESS, sizeof(DataPacket));
        
        // Verificare dacă au fost primite date
        if (Wire.available() >= sizeof(DataPacket)) {
            // Citire date
            Wire.readBytes((uint8_t*)&receivedData, sizeof(DataPacket));
            
            // Validare header
            if (receivedData.head == 0xAA) {
                // Calculare checksum pentru validare
                uint8_t calculatedChecksum = receivedData.head ^ receivedData.length ^
                                             (receivedData.distance >> 8) ^ (receivedData.distance & 0xFF);
                
                // Verificare checksum
                if (calculatedChecksum == receivedData.checksum) {
                    // Date valide - afișare
                    Serial.println("========================================");
                    Serial.print("MASTER: Pachet I2C valid primit\n");
                    Serial.print("  Header:   0x");
                    Serial.println(receivedData.head, HEX);
                    Serial.print("  Length:   ");
                    Serial.println(receivedData.length);
                    Serial.print("  Distance: ");
                    Serial.print(receivedData.distance);
                    Serial.println(" cm");
                    Serial.print("  Checksum: 0x");
                    Serial.print(receivedData.checksum, HEX);
                    Serial.println(" [OK]");
                    
                    // Comportament adițional: Alertă proximitate
                    if (receivedData.distance < 10 && receivedData.distance > 0) {
                        Serial.println("  *** ALERTĂ: Obiect prea aproape! ***");
                    } else if (receivedData.distance == 9999) {
                        Serial.println("  *** ATENȚIE: Eroare citire senzor ***");
                    }
                    
                    Serial.println("========================================");
                } else {
                    // Checksum invalid
                    Serial.println("MASTER: EROARE - Checksum invalid!");
                    Serial.print("  Așteptat: 0x");
                    Serial.println(calculatedChecksum, HEX);
                    Serial.print("  Primit:   0x");
                    Serial.println(receivedData.checksum, HEX);
                }
            } else {
                // Header invalid
                Serial.println("MASTER: EROARE - Header invalid!");
                Serial.print("  Așteptat: 0xAA, Primit: 0x");
                Serial.println(receivedData.head, HEX);
            }
        } else {
            // Nu s-au primit date
            Serial.println("MASTER: EROARE - Nu s-au primit date de la slave");
            Serial.print("  Bytes disponibili: ");
            Serial.println(Wire.available());
        }
    }
}
