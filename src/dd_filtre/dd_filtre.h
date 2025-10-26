#ifndef FILTRE_H
#define FILTRE_H

#include <Arduino.h>
#include <stddef.h>

#define SAMPLES_NUM 5  // numărul de valori luate în calcul

// Structură pentru mediere ponderată
typedef struct {
    float values[SAMPLES_NUM];  // buffer cu ultimele valori
    float alpha[SAMPLES_NUM];   // ponderi pentru fiecare valoare
    int index;                  // indice curent
} WeightedAvgData;

// Funcții publice
float weightedMovingAverageFilter(WeightedAvgData &data, float newValue);
void printBuffer(WeightedAvgData &data);

#endif
