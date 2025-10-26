#include <Arduino.h>
#include "dd_filtre.h"
// Filtrul mediere ponderată
float weightedMovingAverageFilter(WeightedAvgData &data, float newValue) {
  // Mutăm bufferul la stânga
  for (int i = 0; i < SAMPLES_NUM - 1; i++) {
    data.values[i] = data.values[i + 1];
  }
  // Adăugăm valoarea nouă la final
  data.values[SAMPLES_NUM - 1] = newValue;

  // Calculăm media ponderată
  float weightedAverage = 0;
  float weightSum = 0;
  for (int i = 0; i < SAMPLES_NUM; i++) {
    weightedAverage += data.values[i] * data.alpha[i];
    weightSum += data.alpha[i];
  }

  return weightedAverage / weightSum;
}

// Funcție de debug: afișează bufferul curent
void printBuffer(WeightedAvgData &data) {
    Serial.print("[ ");
    for (int i = 0; i < SAMPLES_NUM; i++) {
        Serial.print(data.values[i], 2);
        Serial.print(" ");
    }
    Serial.println("]");
}
