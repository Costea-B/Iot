#include <Arduino.h>
#include "dd_releu/dd_releu.h"
#include "dd_motor/dd_motor.h"
#include "dd_stdio/dd_stdio.h"

String inputCommand = "";

void setup() {
    Serial.begin(9600);
    init_stdio();

    Relay_Init();
    Motor_Init();

    printf("=== Control Releu + Motor L293D ===\n");
    printf("Comenzi disponibile:\n");
    printf("  RELAY_ON / RELAY_OFF / RELAY_TOGGLE\n");
    printf("  MOTOR_FWD <0-255> / MOTOR_REV <0-255> / MOTOR_OFF\n");
    printf("-------------------------------------\n");
}

void loop() {
    if (Serial.available() > 0) {
        inputCommand = Serial.readStringUntil('\n');
        inputCommand.trim();

        // ===== RELEU =====
        if (inputCommand.equalsIgnoreCase("RELAY_ON")) {
            Relay_On();
            printf("Releu activat (LED aprins)\n");
        }
        else if (inputCommand.equalsIgnoreCase("RELAY_OFF")) {
            Relay_Off();
            printf("Releu dezactivat (LED stins)\n");
        }
        else if (inputCommand.equalsIgnoreCase("RELAY_TOGGLE")) {
            Relay_Toggle();
            printf("Releu schimbat. Stare curentă: %d\n", Relay_GetState());
        }

        // ===== MOTOR =====
        else if (inputCommand.startsWith("MOTOR_FWD")) {
            int speed = 255;
            int spaceIndex = inputCommand.indexOf(' ');
            if (spaceIndex != -1) {
                speed = inputCommand.substring(spaceIndex + 1).toInt();
                speed = constrain(speed, 0, 255);
            }
            Motor_Forward(speed);
            printf("Motor înainte la viteză %d\n", speed);
        }
        else if (inputCommand.startsWith("MOTOR_REV")) {
            int speed = 255;
            int spaceIndex = inputCommand.indexOf(' ');
            if (spaceIndex != -1) {
                speed = inputCommand.substring(spaceIndex + 1).toInt();
                speed = constrain(speed, 0, 255);
            }
            Motor_Reverse(speed);
            printf("Motor invers la viteză %d\n", speed);
        }
        else if (inputCommand.equalsIgnoreCase("MOTOR_OFF")) {
            Motor_Stop();
            printf("Motor oprit\n");
        }
        else {
            printf("Comandă necunoscută: %s\n", inputCommand.c_str());
        }
    }
}
