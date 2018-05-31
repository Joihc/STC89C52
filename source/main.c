// Source: main.h 
// File Name: main.h
// Author: hg
// Date:2018年5月24日

#define MAIN_C_

#include "main.h"

/* define variables */
uint16 count;






void main()
{
	Lcm_Init();
	
	
	Init_TIME0();
	Init_TIME1();
	
	Init_USART();
	
	Display_Start();
	
	EA=1;//总中断
	
	while(1)
	{
		Display_Menu1_Screen();//主显示界面
		Display_Menu2_Screen();//显示设置界面
	}
}

void Timer0() interrupt 1
{
	Interrupt_Init0();
	if (count-- == 0)               //1ms * 1000 -> 1s
  {
    count = 1000;               //reset counter
		Led_Change();
  }
}
void Usart() interrupt 4
{
		if(ReceiveServicePacket())    
    {
        //数据包长度正确则执行以下代码
        Deal_UART_RecData(); 						
    }
    else
    {
        //数据包长度错误则执行以下代码 
				//SetLcdData(Title,1);	
    }
		RI=0;  //接收并处理一次数据后把接收中断标志清除一下，拒绝响应在中断接收忙的时候发来的请求  
}
