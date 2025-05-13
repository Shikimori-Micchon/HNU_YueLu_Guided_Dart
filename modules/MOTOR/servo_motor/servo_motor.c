#include "servo_motor.h"
#include <stdint.h>

void Servo_Motor_Init(void)
{
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2); 
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
    Servo_Motor_SetAngle(0, 0, 0, 0); // 初始化舵机角度
    DWT_Delay_ms(500);
    Servo_Motor_SetAngle(30, 30, 30, 30); // 初始化舵机角度
    DWT_Delay_ms(500);
    Servo_Motor_SetAngle(-30, -30, -30, -30); // 初始化舵机角度
    DWT_Delay_ms(500);
    Servo_Motor_SetAngle(0, 0, 0, 0); // 初始化舵机角度
}
void angle_to_pwm(int16_t angle,  uint16_t*pwm)
{
    if (angle < -60 || angle > 60) {
        return; // Invalid angle
    }
    *pwm = (uint16_t)(angle * 10 + 1500); // Convert angle to PWM value
}

void Servo_Motor_Task(int16_t angle1, int16_t angle2, int16_t angle3, int16_t angle4)
{
    uint16_t pwm1, pwm2, pwm3, pwm4;
    angle_to_pwm(angle1, &pwm1);
    angle_to_pwm(angle2, &pwm2);
    angle_to_pwm(angle3, &pwm3);
    angle_to_pwm(angle4, &pwm4);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, pwm1);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, pwm2);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, pwm3);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, pwm4);
}