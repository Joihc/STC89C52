// Source: main.h 
// File Name: main.h
// Author: hg
// Date:2018��5��24��

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
	
	EA=1;//���ж�
	
	while(1)
	{
		Display_Menu1_Screen();//����ʾ����
		Display_Menu2_Screen();//��ʾ���ý���
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
        //���ݰ�������ȷ��ִ�����´���
        Deal_UART_RecData(); 						
    }
    else
    {
        //���ݰ����ȴ�����ִ�����´��� 
				//SetLcdData(Title,1);	
    }
		RI=0;  //���ղ�����һ�����ݺ�ѽ����жϱ�־���һ�£��ܾ���Ӧ���жϽ���æ��ʱ����������  
}
