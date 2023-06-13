#include "dc_send.h"


static void DC_Servo_Driver_UART_MSD_Send_Command(const uint8_t *pData,
                                                  uint16_t Size)
{
    HAL_UART_Transmit(&huart1,
                      pData,
                      Size,
                      1000);
}

//////////////////////////////////////////////////
void DC_Servo_Driver_UART_MSD_Restart_Driver(void)
{
    uint8_t buffer[10];
  
    memcpy(buffer, "N1 O R101\n", 10);
    
    DC_Servo_Driver_UART_MSD_Send_Command(buffer,
                                          10);
}

////////////////////////////////////////////////////////////////
void DC_Servo_Driver_UART_MSD_Moving_Set(uint32_t acceleration,
                                         uint32_t velocity,
                                         int32_t position)
{
    uint8_t buffer[30];
    uint8_t length = 0;
    
    sprintf((char *)buffer,
            "N1 p%u v%u a%u\n",
            position,
            velocity,
            acceleration);
    length = strlen((const char *)buffer);
    
    DC_Servo_Driver_UART_MSD_Send_Command(buffer,
                                          length);
}

void DC_Servo_Driver_UART_MSD_Moving_Stop(void)
{
    uint8_t buffer[7];
  
    memcpy(buffer, "N1 O L\n", 7);
    
    DC_Servo_Driver_UART_MSD_Send_Command(buffer,
                                          7);
}
void DC_Servo_Driver_UART_MSD_Moving_Start(void)
{
    uint8_t buffer[7];
  
    memcpy(buffer, "N1 O U\n", 7);
    
    DC_Servo_Driver_UART_MSD_Send_Command(buffer,
                                          7);
}