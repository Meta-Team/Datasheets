#include "main.h"

short temp_pwm = 0;

int main(void)
{     
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    CAN1_Configuration();                               //CAN1初始化
    
    delay_ms(500);                                      //刚开始要有足够的延时，确保驱动器已经初始化完成 
    CAN_RoboModule_DRV_Reset(0,0);                      //对0组所有驱动器进行复位 
    
    delay_ms(500);                                      //发送复位指令后的延时必须要有，等待驱动器再次初始化完成
    
    CAN_RoboModule_DRV_Config(0,1,100,0);               //1号驱动器配置为100ms传回一次电流速度位置数据
    delay_us(200);                                      //此处延时为了不让传回数据时候4个不一起传
    CAN_RoboModule_DRV_Config(0,2,100,0);               //2号驱动器配置为100ms传回一次电流速度位置数据
    delay_us(200);                                      //此处延时为了不让传回数据时候4个不一起传
    CAN_RoboModule_DRV_Config(0,3,100,0);               //3号驱动器配置为100ms传回一次电流速度位置数据
    delay_us(200);                                      //此处延时为了不让传回数据时候4个不一起传
    CAN_RoboModule_DRV_Config(0,4,100,0);               //4号驱动器配置为100ms传回一次电流速度位置数据

    
    CAN_RoboModule_DRV_Mode_Choice(0,0,OpenLoop_Mode);  //0组的所有驱动器 都进入开环模式
    delay_ms(500);                                      //发送模式选择指令后，要等待驱动器进入模式就绪。所以延时也不可以去掉。
    
    temp_pwm = 0;
    while(1) //测试加速
    {
        delay_ms(100);
        
        CAN_RoboModule_DRV_OpenLoop_Mode(0,1,temp_pwm);
        CAN_RoboModule_DRV_OpenLoop_Mode(0,2,temp_pwm);
        CAN_RoboModule_DRV_OpenLoop_Mode(0,3,temp_pwm);
        CAN_RoboModule_DRV_OpenLoop_Mode(0,4,temp_pwm);
        temp_pwm += 200;
        if(temp_pwm > 3000)
        {
            break;
        }
    }
    
    temp_pwm = 3000;
    while(1) //测试减速
    {
        delay_ms(100);
        
        CAN_RoboModule_DRV_OpenLoop_Mode(0,1,temp_pwm);
        CAN_RoboModule_DRV_OpenLoop_Mode(0,2,temp_pwm);
        CAN_RoboModule_DRV_OpenLoop_Mode(0,3,temp_pwm);
        CAN_RoboModule_DRV_OpenLoop_Mode(0,4,temp_pwm);
        temp_pwm -= 100;
        if(temp_pwm < 0)
        {
            break;
        }
    }
    
    temp_pwm = 0;
    
    CAN_RoboModule_DRV_OpenLoop_Mode(0,0,temp_pwm);
    
    while(1);
}


/*
1.电脑需要提前安装好keil MDK。推荐版本V4.74。
2.本程序适用芯片为STM32F405RGT6，使用25MHz晶振，主频168MHz。
3.如果需要测试CAN的通信，用户所用开发板或者主控板，必须含有CAN收发器，本例程使用的CAN引脚为PA11 PA12。
4.主控芯片如有不同，请自行在工程上修改，CAN引脚如有不同，请自行在can.c文件上修改。
*/
