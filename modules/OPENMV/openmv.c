#include"openmv.h"

#define OPENMV_BUFFER_SIZE OPENMV_DATA_SIZE+2

#define OPENMV_START_FLAG 0xAA
#define OPENMV_END_FLAG 0x55

static OPENMV_Instance_s openmv_instance;
uint16_t target_x, target_y;

static void OPENMVRxCallback()
{
    uint8_t *rxbuff;
    rxbuff = openmv_instance.usart_instance->recv_buff;

    // 帧头帧尾判断
    if(rxbuff[0] == OPENMV_START_FLAG && rxbuff[OPENMV_BUFFER_SIZE - 1] == OPENMV_END_FLAG)
    {
        for(int i = 0; i < OPENMV_DATA_SIZE; i++)
            openmv_instance.OPENMV_data.data[i] = rxbuff[i+1];
            openmv_instance.OPENMV_data.checksum = rxbuff[OPENMV_DATA_SIZE];
    }
    
    // //校验和
    // if(openmv_instance.OPENMV_data.checksum!= (rxbuff[1] + rxbuff[2] + rxbuff[3] + rxbuff[4])& 0xFF)
    // {
    //     return;
    // }

    //解析
    openmv_instance.TargetData.target_x = (rxbuff[1] << 8) | rxbuff[2];
    openmv_instance.TargetData.target_y = (rxbuff[3] << 8) | rxbuff[4];
    target_x = openmv_instance.TargetData.target_x;
    target_y = openmv_instance.TargetData.target_y;

    return;
}

OPENMV_Instance_s *OPENMVInit(UART_HandleTypeDef *OPENMV_usart_handle)
{
    USART_Init_Config_s conf;
    conf.module_callback = OPENMVRxCallback;
    conf.usart_handle = OPENMV_usart_handle;
    conf.recv_buff_size = OPENMV_BUFFER_SIZE;
    openmv_instance.usart_instance = USARTRegister(&conf);

    return (OPENMV_Instance_s*)&openmv_instance;
}