#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

#include "main.h"
#include "tim.h"

void Servo_Motor_Init(void);
void Servo_Motor_Task(int16_t angle1, int16_t angle2, int16_t angle3, int16_t angle4);

#endif 
