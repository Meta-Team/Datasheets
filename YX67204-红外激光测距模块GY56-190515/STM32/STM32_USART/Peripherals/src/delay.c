#include "delay.h"
static u8  Times_us=0;//1us需要计的次数
static u16 Times_ms=0;//1ms需要计的次数
//SYSCLK=72M,SYSCLK/8=9M
void delay_init(u8 SYSCLK)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
	Times_us=SYSCLK/8;	//Times_us=9次	    
	Times_ms=(u16)Times_us*1000;//Times_ms=9000次
}
//SYSCLK=72M,SYSCLK/8=9M,即计一次数为9分之一 微秒，1us需要计数9次
//所以SysTick->LOAD=nus*SYSCLK/8=nus*9=Count
//Count<2^24，Max_Count=16777215
//最大nus=Max_Count/9=16777215/9=1864135us
//即最大nus为1864135
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=(nus*Times_us); //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL=0x01 ;      //开始倒数 	 	
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	 
}
//SYSCLK=72M,SYSCLK/8=9M,1微秒需计数9次，则1毫秒需计数9000次
//所以SysTick->LOAD=nms*9000=Count
//Count<2^24，Max_Count=16777215
//最大nms=Max_Count/9000=16777215/9000=1864.135ms，
//即最大nms=1864
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(nms*Times_ms);//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           
	SysTick->CTRL=0x01 ;   
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));  
	SysTick->CTRL=0x00;       
	SysTick->VAL =0X00;       	    
}




