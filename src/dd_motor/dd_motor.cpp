#include "dd_motor.h"

void Motor_Init(void) {
    pinMode(MOTOR_IN1, OUTPUT);
    pinMode(MOTOR_IN2, OUTPUT);
    pinMode(MOTOR_ENABLE, OUTPUT);
    analogWrite(MOTOR_ENABLE, 255); // motor activ full speed
    Motor_Stop();
}

void Motor_Forward(uint8_t speed) {
    digitalWrite(MOTOR_IN1, HIGH);
    digitalWrite(MOTOR_IN2, LOW);
    analogWrite(MOTOR_ENABLE, speed);
}

void Motor_Reverse(uint8_t speed) {
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, HIGH);
    analogWrite(MOTOR_ENABLE, speed);
}

void Motor_Stop(void) {
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, LOW);
    analogWrite(MOTOR_ENABLE, 0);
}
