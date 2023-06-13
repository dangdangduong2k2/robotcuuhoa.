#include "main.h"
#include "clock.h"
#include "gpio.h"
#include "tim.h"
#include "i2c.h"
#include "usart.h"
#include "dc_send.h"
#include "ssd1306.h"
#include "fonts.h"


#define ADD_acc 0x0800FC00
#define ADD_vel 0x0800F800
#define ADD_pos 0x0800F400
#define ADD_tim 0x0800F000
/////////////////////////
void updateMenu(void) ;
void robotShootingFightRun(int32_t Acceleration,
                           int32_t Velocity,
                           int32_t Position);
int Flash_Read(uint32_t add);
void Flash_Write(uint32_t add , int value);
void Flash_Delete(uint32_t add);
void setup(void);
/////////////////////////
int acc=100;
int vel=50;

int8_t pos=10;

uint8_t count_to_deacc=0;
uint8_t count_to_back=0;
uint8_t accept_to_deacc=0;

uint8_t buffer_acc[30];
uint8_t buffer_vel[30];
uint8_t buffer_pos[30];
uint8_t buffer_tim[30];
/////////////////////////
int ticktim2=0;
int ticktim3=0;
int ticktim4=0;
int tickmoter=0;
int timemoter=3000;

uint8_t mode=0;

uint8_t state1=0;
uint8_t state2=0;
uint8_t state3=0;
uint8_t state_run=0;
uint8_t run=0;
uint8_t crun=0;
uint8_t back=0;
uint8_t state_to_deacc=0;
uint8_t state_to_back=0;

uint8_t color_acc=0;
uint8_t color_vel=1;
uint8_t color_pos=1;
uint8_t color_time=1;



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2) 
  {
      if(++ticktim2==20)
      {
          ticktim2=0;
          
          if(HAL_GPIO_ReadPin(BUTTON1_GPIO_Port,BUTTON2_Pin)==0 && state2==0)
          {
              SSD1306_Clear();
              mode++;
              if(mode==4)
              {
                mode=0;
              }
              state2=1;
          }
          if(HAL_GPIO_ReadPin(BUTTON1_GPIO_Port,BUTTON2_Pin)==1)
          {    
              
              state2=0;
          }
          
          if(HAL_GPIO_ReadPin(BUTTON1_GPIO_Port,BUTTON1_Pin)==0 && state1==0)
          {
            SSD1306_Clear();
            if(color_acc==0)
              {
                acc=acc+10;
                if(100<=acc)
                {
                  acc=100;
                }
              }
              if(color_vel==0)
              {
                vel=vel+10;
                if(140<=vel)
                {
                  vel=140;
                }
              }
              if(color_pos==0)
              {
                pos++;
                if(20<=pos)
                {
                  pos=20;
                }
              }
              if(color_time==0)
                {
                  timemoter=timemoter+500;
                  if(10000<=timemoter)
                  {
                    timemoter=10000;
                  }
                }
              state1=1;
          }
          if(HAL_GPIO_ReadPin(BUTTON1_GPIO_Port,BUTTON1_Pin)==1)
          {    
              state1=0;
          }
          
          if(HAL_GPIO_ReadPin(BUTTON3_GPIO_Port,BUTTON3_Pin)==0 && state3==0)
          {
              SSD1306_Clear();
              state3=1;
              if(color_acc==0)
              {
                acc=acc-10;
                if(acc<=0)
                {
                  acc=0;
                }
              }
              if(color_vel==0)
              {
                vel=vel-10;
                if(vel<=0)
                {
                  vel=0;
                }
              }
              if(color_pos==0)
              {
                pos--;
                if(pos<=0)
                {
                  pos=0;
                }
              }
              if(color_time==0)
              {
                timemoter=timemoter-500;
                if(timemoter<=0)
                {
                  timemoter=0;
                }
              }
              
          }
          if(HAL_GPIO_ReadPin(BUTTON3_GPIO_Port,BUTTON3_Pin)==1)
          {    
              state3=0;
          }
          
          if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==0 && state_run==0)
          {
              run++;
              state_run=1;
          }
          if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==1 )
          {    
              state_run=0;
          }
          if(run==1)
          {
              DC_Servo_Driver_UART_MSD_Moving_Start();
              setup();
              robotShootingFightRun(100,120,1000);
              run=0;
              crun=0;
              accept_to_deacc=1;
              count_to_deacc=0;
          }
          if(crun==1)
          {
              robotShootingFightRun(100,88,0);
              accept_to_deacc=0;
            
          }
          if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==1 && crun==0)
          {    
              HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,1);
              tickmoter=1;
              crun=99;
              DC_Servo_Driver_UART_MSD_Moving_Stop();
              
          }
      }
      
  }
  else if (htim->Instance == TIM3) 
  {
      if(tickmoter==1)
      {
        ticktim3++;
      }
      if(ticktim3==timemoter)
      {
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,0);
        
        ticktim3=0;
        tickmoter=0;
        crun=1;
        back=1;
      }    
      if(count_to_deacc==pos && accept_to_deacc==1)
      {
        robotShootingFightRun(acc,vel,1000);
        count_to_deacc=0;
      }
      
  }
  else if (htim->Instance == TIM4) 
  {
    
    if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)==0 && state_to_deacc==0)
    {    
        count_to_deacc++;  
        state_to_deacc=1;
    } 
    if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)==1)
    {    
        state_to_deacc=0;
    } 
    if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)==0 && back==1 && state_to_back==0)
    {
        count_to_back++;
        state_to_back=1;
    }
    if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)==1)
    {    
        state_to_back=0;
    }
    if(count_to_back==18)
    {
        DC_Servo_Driver_UART_MSD_Moving_Stop();
        back=0;
    }
  }
}



void robotShootingFightRun(int32_t Acceleration,
                           int32_t Velocity,
                           int32_t Position)
{
    DC_Servo_Driver_UART_MSD_Moving_Set(Acceleration,
                                        Velocity,
                                        Position);
}




int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  
  MX_I2C1_Init();
  
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_Base_Start_IT(&htim4);
  DC_Servo_Driver_UART_MSD_Restart_Driver();
  DC_Servo_Driver_UART_MSD_Moving_Stop();
  acc = Flash_Read(ADD_acc);
  

  
  char snum[5]; 
  SSD1306_Init (); 
  
  SSD1306_GotoXY (25,20); 
  SSD1306_Puts ("WELCOME!", &Font_11x18, 1); 
  SSD1306_UpdateScreen(); 
  HAL_Delay(1000);
  
  
  SSD1306_Clear();
  SSD1306_GotoXY (12,20); 
  SSD1306_Puts ("SPK AURORA!", &Font_11x18, 1); 
  SSD1306_UpdateScreen(); 
  HAL_Delay(1000);
  SSD1306_Clear();
  acc = Flash_Read(ADD_acc);
  vel = Flash_Read(ADD_vel);
  pos = Flash_Read(ADD_pos);
  timemoter = Flash_Read(ADD_tim);
  while (1)
  {
    
    SSD1306_GotoXY (5,0);
    SSD1306_Puts ("SETTING!", &Font_7x10, 1); 
    SSD1306_DrawLine(0,10,130,10,1);
    updateMenu();
    SSD1306_GotoXY (5,12);
    sprintf((char *)buffer_acc,
          "ACCELARATION:%u",
          acc);
    SSD1306_Puts (buffer_acc, &Font_7x10, color_acc); 
    
    SSD1306_GotoXY (5,24);
    sprintf((char *)buffer_vel,
          "VELOCITY:%u",
          vel);
    SSD1306_Puts (buffer_vel, &Font_7x10, color_vel); 
    
    SSD1306_GotoXY (5,36);
    sprintf((char *)buffer_pos,
          "POSITION:%u",
          pos);
    SSD1306_Puts (buffer_pos, &Font_7x10, color_pos); 
    
    SSD1306_GotoXY (5,48);
    sprintf((char *)buffer_tim,
          "TIME OFF:%u",
          timemoter);
    SSD1306_Puts (buffer_tim, &Font_7x10, color_time); 
    
    SSD1306_UpdateScreen(); 
  }  
}

void updateMenu(void) 
{
    switch (mode) 
    {
    case 0:
      color_acc=0;
      color_vel=1;
      color_pos=1;
      color_time=1;
      break;
    case 1:
      color_acc=1;
      color_vel=0;
      color_pos=1;
      color_time=1;

      break;
    case 2:
      color_acc=1;
      color_vel=1;
      color_pos=0;
      color_time=1;
      break; 
    case 3:
      color_acc=1;
      color_vel=1;
      color_pos=1;
      color_time=0;
      break; 
    }
}
void Flash_Delete(uint32_t add)
{
  HAL_FLASH_Unlock();
  FLASH_EraseInitTypeDef err;
  err.Banks=1;
  err.NbPages=1;
  err.PageAddress=add;
  err.TypeErase=FLASH_TYPEERASE_PAGES;
  uint32_t page;
  HAL_FLASHEx_Erase(&err,&page);
  
}
void Flash_Write(uint32_t add , int value)
{
  HAL_FLASH_Unlock();
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,add,value);
  HAL_FLASH_Lock();
}
int Flash_Read(uint32_t add)
{
  return *(__IO uint16_t *)(add) ;
}
void setup(void)
{
  Flash_Delete(ADD_acc);
  Flash_Write(ADD_acc,acc);
  acc = Flash_Read(ADD_acc);
  
  Flash_Delete(ADD_vel);
  Flash_Write(ADD_vel,vel);
  vel = Flash_Read(ADD_vel);
  
  Flash_Delete(ADD_pos);
  Flash_Write(ADD_pos,pos);
  pos = Flash_Read(ADD_pos);
  
  Flash_Delete(ADD_tim);
  Flash_Write(ADD_tim,timemoter);
  timemoter = Flash_Read(ADD_tim);
}



