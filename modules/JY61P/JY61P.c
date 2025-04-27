#include "JY61P.h"


// static JY61PData JY61P_data;
static JY61P_Instance_s JY61P_instance;

static JY61PData *JY61P_data; // 传感器数据结构体实例

JY61PData *INS_Init(void)
{
    DWT_Init(168);
    JY61P_data = JY61P_Init(WIT_BAUD_9600);
    // WitRestartSensor();
    DWT_Delay(2);
    return &JY61P_data;
}

JY61PData* JY61P_Init(uint8_t uart_baud)
{
    
    USART_Init_Config_s conf;
    conf.module_callback = WITSerialDecode;
    conf.usart_handle = &huart3; 
    conf.recv_buff_size = 33; 
    JY61P_instance.usart_instance = USARTRegister(&conf);

    WitInit(WIT_PROTOCOL_NORMAL, 0x50);
    WitSerialWriteRegister(USART_JY61P_SEND); // 注册传感器串口发送函数
    WitDelayMsRegister(DWT_Delay_ms);// 注册传感器延时函数
    WitRegisterCallBack(JY61P_DataUpdate);// 注册传感器数据更新函数
    WitSetUartBaud(uart_baud);
    WitStartAccCali();
    DWT_Delay_ms(100);
    return &JY61P_instance.JY61P_data;
}

void JY61P_ReadData()
{
    WitReadReg(AX, 3);
    DWT_Delay(0.1);
    WitReadReg(GX, 3);
    DWT_Delay(0.1);
    WitReadReg(HX, 3);
    DWT_Delay(0.1);
    WitReadReg(Roll, 3);
}

void WITSerialDecode()
{
    for (int i = 0; i < 33; i++)
  {
    // HAL_UART_Transmit_IT(&huart1, &JY61P_instance.usart_instance->recv_buff,33);
    WitSerialDataIn(JY61P_instance.usart_instance->recv_buff[i]);
  }
}

void JY61P_DataUpdate(uint32_t uiReg, uint32_t uiRegNum)
{
    switch(uiReg)
    {
        case AX:
            for(int i = 0; i < 3; i++)
            {
                JY61P_instance.JY61P_data.fAcc[i] = sReg[AX+i] / 32768.0f * 16.0f;
            }
            break;
        case GX:
            for(int i = 0; i < 3; i++)
            {
                JY61P_instance.JY61P_data.fGyro[i] = sReg[GX+i] / 32768.0f * 2000.0f;
            }
            break;
        case Roll: 
            for(int i = 0; i < 3; i++)
            {
                JY61P_instance.JY61P_data.fEuler[i] = sReg[Roll+i] / 32768.0f * 180.0f;
            }
            // JY61P_instance.JY61P_data.fEuler[2] = JY61P_instance.JY61P_data.fEuler[2] > 0 ? 360 - JY61P_instance.JY61P_data.fEuler[2] : -JY61P_instance.JY61P_data.fEuler[2];    
            break;
        default:
            break;
    }
}
