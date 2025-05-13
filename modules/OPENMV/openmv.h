#ifndef OPENMV_H
#define OPENMV_H

#include "main.h"
#include "bsp_usart.h"
#include "bsp_dwt.h"

#define OPENMV_DATA_SIZE 5
typedef struct
{
uint16_t data[5];
uint16_t checksum;
} OPENMVData;


typedef struct
{
    uint16_t target_x;
    uint16_t target_y;
} OPENMV_TargetData;

typedef struct
{
    USARTInstance *usart_instance;
    OPENMVData OPENMV_data;
    OPENMV_TargetData TargetData;
} OPENMV_Instance_s;

OPENMV_Instance_s *OPENMVInit(UART_HandleTypeDef *hc05_usart_handle);

extern uint16_t target_x;
extern uint16_t target_y;
extern uint16_t flag;

#endif 
