#ifndef JY61P_H
#define JY61P_H

#include "main.h"
#include "bsp_usart.h"
#include "bsp_dwt.h"
#include "wit_c_sdk.h"
typedef struct
{
float fEuler[3], fAcc[3], fGyro[3];
} JY61PData;

typedef struct
{
    USARTInstance *usart_instance;
    JY61PData JY61P_data;
} JY61P_Instance_s;

/**
 * @brief JY61P传感器初始化函数，初始化阶段会校准加速度计、陀螺仪、磁力计
 * @note 串口波特率默认为921600，使用USART1作为传输串口
 * @param uart_baud 串口波特率，参考REG.h中的宏定义BAUD部分, 默认为921600
 * @return void
 */
JY61PData* JY61P_Init(uint8_t uart_baud);

/**
 * @brief JY61P传感器数据读取函数，读取传感器Acc、Gyro、Mag、Temp数据
 * @param void 
 * @return void
 */
void JY61P_ReadData(void);

/**
 * @brief JY61P传感器数据处理函数，处理传感器数据，数据被存储在JY61PData结构体中
 * @param void
 * @return void
 */
void WITSerialDecode(void); // JY61P传感器回传数据解码函�????????

/**
 * @brief JY61P传感器数据更新函数，更新传感器数据，数据被存储在JY61PData结构体中
 * @note 在调用JY61P_ReadData()函数后，该函数会自动更新传感器数据，无需手动调用
 * @param uiReg 传感器数据寄存器地址，表示被更新的寄存器地址
 * @param uiRegNum 传感器数据寄存器数量
 * @return void
 */
void JY61P_DataUpdate(uint32_t uiReg, uint32_t uiRegNum);

JY61PData *INS_Init(void); // 初始化传感器函数，返回传感器数据结构体指针

extern double gyro_x; // 陀螺仪x轴数据
extern double gyro_y; // 陀螺仪y轴数据
extern double dart_speed; 
extern double IMU_ax; // 加速度计x轴数据
extern double IMU_ay; // 加速度计y轴数据    
extern double IMU_az; // 加速度计z轴数据
#endif // ! JY61P_H
