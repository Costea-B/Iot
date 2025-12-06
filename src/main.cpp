#define TRIG_PIN 8
#define ECHO_PIN 9
#include <Arduino.h>

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // Curățăm semnalul TRIG
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Trimitem un impuls de 10us
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Citim durata în care semnalul revine pe ECHO
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Calculăm distanța în cm
  long distance = duration * 0.034 / 2;

  Serial.print("Distanta: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500);
}
