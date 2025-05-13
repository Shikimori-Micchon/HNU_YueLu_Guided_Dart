#include "dart.h"

#include "JY61P.h"  // 传感器头文件
#include "servo_motor.h" // 舵机头文件
#include"openmv.h" // openmv头文件
#include"Ducted_fan_Motor.h" // 电机头文件
#include "PNG.h" // PNG头文件
#include "bsp_dwt.h" // DWT头文件

JY61PData *pJY61PData = NULL; // 传感器数据结构体指针
OPENMV_Instance_s *pOPENMVData = NULL; // openmv数据结构体指针
float fly_time = 0; // 飞行时间
float stop_time = 0; // 停止时间

void DartInit()
{
    pJY61PData = INS_Init(); // 初始化传感器
    pOPENMVData = OPENMVInit(&huart1); // 初始化openmv
    Ducted_Fan_Motor_Init(); // 初始化电机
    Servo_Motor_Init(); // 初始化舵机
    PNG_Init(); // 初始化PNG
}   

void DartTask()
{
    if(IMU_ax>1000){
        fly_time = DWT_GetTimeline_ms();
        while(stop_time = DWT_GetTimeline_ms() - fly_time < 4000)
        {
            Ducted_Fan_Motor_Task();
            if(flag == 1)
            {
            PNG_TASK(); // PNG任务
            }
        }
        Ducted_Fan_Motor_Stop(); // 停止电机
        Servo_Motor_Task(0, 0, 0, 0); // 停止舵机
    }
}