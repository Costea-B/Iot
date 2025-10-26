#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "dd_bnt/dd_bnt.h"
#include "dd_led/dd_led.h"

// Variabile globale
volatile bool led1_state = false;
volatile bool led2_state = false;
volatile int blinkInterval = 500;   // timp ON/OFF pentru LED2 (ms)
volatile bool btn_event = false;

// Task 1: LED1 controlat de buton
void Task_ButtonLed(void *pvParameters) {
  for (;;) {
    if (Button_Toggle_Read()) {
      led1_state = !led1_state;
      Led1_Set(led1_state);
      btn_event = true;
      vTaskDelay(200 / portTICK_PERIOD_MS); // debounce
    }
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

// Task 2: LED2 intermitent când LED1 e stins
void Task_BlinkLed(void *pvParameters) {
  for (;;) {
    if (!led1_state) {
      led2_state = !led2_state;
      Led2_Set(led2_state);
      vTaskDelay(blinkInterval / portTICK_PERIOD_MS);
    } else {
      Led2_Set(false);
      led2_state = false;
      vTaskDelay(200 / portTICK_PERIOD_MS);
    }
  }
}

// Task 3: Incrementare/decrementare interval LED2
void Task_Counter(void *pvParameters) {
  for (;;) {
    if (Button_Inc_Read()) {
      blinkInterval += 100;  // crește intervalul
      if (blinkInterval > 2000) blinkInterval = 2000; // limită max
      btn_event = true;
      vTaskDelay(200 / portTICK_PERIOD_MS);
    }
    if (Button_Dec_Read()) {
      blinkInterval -= 100;  // micșorează intervalul
      if (blinkInterval < 100) blinkInterval = 100; // limită min
      btn_event = true;
      vTaskDelay(200 / portTICK_PERIOD_MS);
    }
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

// Task 4: Raportare stări în terminal
void Task_Report(void *pvParameters) {
  // variabile pentru stări anterioare
  bool prev_led1_state = led1_state;
  bool prev_led2_state = led2_state;
  int prev_blinkInterval = blinkInterval;

  for (;;) {
    // verificăm dacă s-au schimbat LED-urile sau intervalul
    if (led1_state != prev_led1_state ||
        led2_state != prev_led2_state ||
        blinkInterval != prev_blinkInterval ||
        btn_event) 
    {
      Serial.print("LED1: ");
      Serial.print(led1_state ? "ON" : "OFF");
      Serial.print(" | LED2: ");
      Serial.print(led2_state ? "ON" : "OFF");
      Serial.print(" | Blink Interval: ");
      Serial.print(blinkInterval);
      Serial.println(" ms");

      if (btn_event) {
        Serial.println(">> Eveniment: Buton apăsat!");
        btn_event = false;
      }

      // actualizăm stările anterioare
      prev_led1_state = led1_state;
      prev_led2_state = led2_state;
      prev_blinkInterval = blinkInterval;
    }

    vTaskDelay(0.1 / portTICK_PERIOD_MS); // verificăm des
  }
}


// Variabile pentru heartbeat
#define TIMER_PERIOD_MS 5000  // 5 secunde
volatile bool heartbeatFlag = false;
volatile unsigned long lastHeartbeat = 0;

void Task_Heartbeat(void *pvParameters) {
  for (;;) {
    unsigned long now = millis();
    if (now - lastHeartbeat >= TIMER_PERIOD_MS) {
      heartbeatFlag = true;
      lastHeartbeat = now;

      // Afișăm mesaj serial sau aprindem LED dedicat
      Serial.println(">> SYSTEM HEARTBEAT <<");
      // Dacă vrei să folosești un LED:
      // Led3_Toggle(); // presupunem că ai definit Led3
    }
    vTaskDelay(100 / portTICK_PERIOD_MS); // verificăm des
  }
}

