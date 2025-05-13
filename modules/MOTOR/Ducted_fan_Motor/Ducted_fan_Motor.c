#include"Ducted_fan_Motor.h"

void Ducted_Fan_Motor_Init()
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    DWT_Delay_ms(3000);
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, 1000);
    DWT_Delay_ms(1000);
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, 1900);
    DWT_Delay_ms(1000);
    // __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, 1700);
    // DWT_Delay_ms(1000);
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, 1200);
    // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
}

void Ducted_Fan_Motor_Task()
{

    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, 1900);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
}  
