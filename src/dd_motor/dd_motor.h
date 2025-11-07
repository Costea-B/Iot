#ifndef DD_MOTOR_H
#define DD_MOTOR_H

#include <Arduino.h>

#define MOTOR_IN1     8   // intrarea 1 spre L293D
#define MOTOR_IN2     9   // intrarea 2 spre L293D
#define MOTOR_ENABLE  10  // pin PWM pentru viteza motorului (pin EN1 de pe L293D)

void Motor_Init(void);
void Motor_Forward(uint8_t speed);
void Motor_Reverse(uint8_t speed);
void Motor_Stop(void);

#endif
