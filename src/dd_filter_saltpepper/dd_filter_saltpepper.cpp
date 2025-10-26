#include "dd_filter_saltpepper.h"

// Funcția Bubble Sort pentru a sorta valorile
static void bubbleSort(float* arr, size_t n) {
    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                float temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

float SaltPepperFilter(float* data, size_t length) {
    // copie buffer
    float temp[length];
    for(size_t i=0; i<length; i++) temp[i] = data[i];

    // Sortăm cu Bubble Sort
    bubbleSort(temp, length);

    // valoarea mediană
    float median = temp[length/2];
    return median;
}
