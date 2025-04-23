#include "JY61P.h"


// static JY61PData JY61P_data;
static JY61P_Instance_s JY61P_instance;

JY61PData* JY61P_Init(uint8_t uart_baud)
{
    USART_Init_Config_s conf;
    conf.module_callback = WITSerialDecode;
    conf.usart_handle = &huart3; //修改为对应串口
    conf.recv_buff_size = 33; //与传感器上位机有关，wit协议固定一次传回11字节，若开启gyro、mag、euler、acc则为44字节
    JY61P_instance.usart_instance = USARTRegister(&conf);

    WitInit(WIT_PROTOCOL_NORMAL, 0x50);
    WitSerialWriteRegister(USARTSend2Sensor); // 注册传感器串口发送函数
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
    for (int i = 0; i < 44; i++)
  {
    WitSerialDataIn(JY61P_instance.usart_instance->recv_buff[i]);
  }
}

void JY61P_DataUpdate(uint32_t uiReg, uint32_t uiRegNum)
{
    switch(uiReg)
    {
        case AX:
            for(int i = 0; i < uiRegNum; i++)
            {
                JY61P_instance.JY61P_data.fAcc[i] = sReg[AX+i] / 32768.0f * 16.0f;
            }
            break;
        case GX:
            for(int i = 0; i < uiRegNum; i++)
            {
                JY61P_instance.JY61P_data.fGyro[i] = sReg[GX+i] / 32768.0f * 2000.0f;
            }
            break;
        case HX:
            for(int i = 0; i < uiRegNum; i++)
            {
                JY61P_instance.JY61P_data.fMag[i] = sReg[HX+i] * 0.098f;
            }
            break;
        case Roll:
            for(int i = 0; i < uiRegNum; i++)
            {
                JY61P_instance.JY61P_data.fEuler[i] = sReg[Roll+i] / 32768.0f * 180.0f;
            }
            JY61P_instance.JY61P_data.fEuler[2] = JY61P_instance.JY61P_data.fEuler[2] > 0 ? 360 - JY61P_instance.JY61P_data.fEuler[2] : -JY61P_instance.JY61P_data.fEuler[2];    
            break;
        case TEMP:
            JY61P_instance.JY61P_data.fTemp = sReg[TEMP] / 100.0f;
            break;
        default:
            break;
    }
}
