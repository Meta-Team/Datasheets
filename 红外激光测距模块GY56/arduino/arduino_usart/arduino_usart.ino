/////////////////////
/*
GY56----MINI
VCC----VCC
GND----GND
1:GY56_TX---10,send A5 65 01 0B to GY-56
2:GY56_RX---11
*/
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX
//////////////////
typedef  struct
{
	uint16_t distance;
	uint8_t   mode;
	uint8_t   temp;
}sensor;
sensor GY56;
unsigned char re_Buf[11],counter=0;
unsigned char sign=0;
void setup() {
   Serial.begin(115200);
   mySerial.begin(9600);
   mySerial.listen();  
   delay(500);    
   mySerial.write(0XA5); 
   mySerial.write(0X65);    //初始化,连续输出模式
   mySerial.write(0X01);    //初始化,连续输出模式 
   mySerial.write(0X0b);    //初始化,连续输出模式 
}

void loop() {
  unsigned char i=0,sum=0;
  while (mySerial.available()) {   
    re_Buf[counter]=(unsigned char)mySerial.read();
    if(counter==0&&re_Buf[0]!=0x5A) return;      // 检查帧头         
    counter++;       
    if(counter==9)                //接收到数据
    {    
       counter=0;                 //重新赋值，准备下一帧数据的接收 
       sign=1;
    }      
  }
  if(sign)
  {   
     sign=0;
     for(i=0;i<8;i++)
      sum+=re_Buf[i]; 
     if(sum==re_Buf[i] )        //检查帧头，帧尾
     {  	       
          GY56.distance=re_Buf[4]<<8|re_Buf[5];
          GY56.mode=re_Buf[6];
          GY56.temp=re_Buf[7]; 
          Serial.print("distance: ");
          Serial.print(  GY56.distance);
          Serial.print(" cm,mode: ");
          Serial.print(  GY56.mode);   
          Serial.print(" temp ");
          Serial.println(  GY56.temp);       
   }
  } 

}

