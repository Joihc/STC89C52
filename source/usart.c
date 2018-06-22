#define USART_C_
#include "usart.h"


//------------------����ͨ��Э��-----------------//
/*
    ����B01_00_0000000#
		����A01_00_0000000#
*/
uint8 buf_string[16];  							//��������
#define deviceID_1Bit '0'           //���ڴ���ͨ��ʱ�����屾���豸ID�ĵ�1λ
#define deviceID_2Bit '1'           //���ڴ���ͨ��ʱ�����屾���豸ID�ĵ�2λ
#define Service_headflag 'A'        //���ڴ���ͨ��ʱ���������ݰ�ͷ������֤���  ��ʾ��������  ���͸���������
#define Clinet_headflag 'B' 				//���ڴ���ͨ��ʱ���������ݰ�ͷ������֤���	��ʾ�ͻ��˰�	�ͻ���Ҫ�˽��յ�

#define deviceID "01"

//7-13 Ϊ���� ��*100
//����-9999.99~+9999.99
uint8 DataPackage[16]={Service_headflag,deviceID_1Bit,deviceID_2Bit,'_','0','1','_','X','X','X','X','X','X','X','#'};
uint8 HeartBeat[16]={Service_headflag,deviceID_1Bit,deviceID_2Bit,'_','X','X','_','0','0','0','0','0','0','0','#'};
uint8 InitPackage[16]={Service_headflag,deviceID_1Bit,deviceID_2Bit,'_','X','1','_','0','0','0','0','0','0','0','#'};
//����˳�򣺿���->����InitPackage->������������->��ˢ�µ�ǰ����
//							->ÿ��100��ѭ������HeartBeat->100��ѭ��������գ�������ղ�����˵��ͨ�Ź���
//							->�޸ĵ�ǰ�˵�2������->�����޸ĺ������->���ղ�ˢ�µ�ǰ����
int16 trans_int;
uint8 trans_str[7];

struct FifoQueue usart_queue;//����ͨ�Ųֿ�

uint16 WaitHeartTime 	= 0;

void Usart_Update()
{
	if(Take_Queue(buf_string))
	{
		Deal_UART_RecData();
	}
	
	
	if(WaitHeartTime<10000)
	{
		WaitHeartTime++;
		Set_Menu1_Value(11,0);//û��ͨ�Ź���
	}
	else
	{
		Set_Menu1_Value(11,1);//ͨ�Ź���
	}
}
void Send_To_Client_Packet(int8 index,int32 num)
{
	uint32 buf = abs(num);
	DataPackage[4]=0x30+index%100/10;
	DataPackage[5]= 0x30+index%10;
	
	DataPackage[7] = (num<0?'-':'+');
	DataPackage[8] = 0x30+buf%1000000/100000;
	DataPackage[9] = 0x30+buf%100000/10000;
	DataPackage[10]= 0x30+buf%10000/1000;
	DataPackage[11]= 0x30+buf%1000/100;
	DataPackage[12]= 0x30+buf%100/10;
	DataPackage[13]= 0x30+buf%10;
		
	ES=0; //�رս����ж�
	SBUF ='#';
	while(TI==0);
  TI=0;
	for(buf=0;buf<15;buf++)
	{
		
		SBUF = DataPackage[buf];
		while(TI==0);
    TI=0;	  
	}
	ES=1; 
}  

void SendHeartBeadPacket()
{
	uint8 i;
	ES=0; //�رս����ж�
	for(i=0;i<15;i++)
	{
		SBUF = HeartBeat[i];
		while(TI==0);
    TI=0;  
	}
	ES=1; 
}
//��ʱ��1���������ʷ�����
void Init_USART()  
{
    SCON=0x50;  //���ڷ�ʽ1��ʹ�ܽ���
    TMOD|=0x20;  //��ʱ��1������ʽ2��8λ�Զ���װ��ֵ��
    TMOD&=~0x10;
    TH1=0xf3;   // ��������ʼֵ���ã�ע�Ⲩ������4800��  12M����
    TL1=0xf3;  
    PCON|=0x80;  //SMOD=1
    TR1=1;
    TI=0;
    RI=0;
    //PS=1;   //��ߴ����ж����ȼ�
    ES=1;  //���������ж�ʹ��
	
		QueueInit(&usart_queue);
}
//װ������
void Interrupt_Queue(uint8 sbuf)
{
	QueueIn(&usart_queue,sbuf);
}
//ȡ��B�ֿ�ͷ��15������
uint8 Take_Queue(uint8 *newchar)
{
	uint8 i =0;
	uint8 temp;
	QueueCheckOut(&usart_queue,Clinet_headflag);
	if(usart_queue.count < 15)
	{
		return 0;//����15������
	}
	while(i<15 && QueueOut(&usart_queue,&temp) != QueueEmpty)
	{
		if(temp == Clinet_headflag || i != 0)
		{
			newchar[i] = temp;
			i++;
		}
	}
	if(i==15)//ȡ������������
	{
		return 1;
	}
	return 0;
}
//�Ƚ�ָ��β��(start:�����￪ʼ�Ƚϣ�quality:�Ƚ϶��ٸ��ַ���CMD_tail[]��Ҫ�Ƚϵ��ַ���)
bit CompareCMD_tail(unsigned char start,unsigned char quality,char CMD_tail[]) 
{
    unsigned char CharNum;
    for(CharNum=0;CharNum<quality;CharNum++)
    {
        if(!(buf_string[start+CharNum]==CMD_tail[CharNum]))
        {
            return 0; 
        }
    }
    return 1;
}

bit Deal_UART_RecData()   //�����ڽ������ݰ��������ɹ��������ݰ��򷵻�1�����򷵻�0��
{	
	if(buf_string[0]==Clinet_headflag&&buf_string[14]=='#')  //�������ݰ�ͷβ�����֤
	{        
		if(CompareCMD_tail(1,2,deviceID))
		{
			strncpy(trans_str,buf_string+7,7);
			trans_int = my_atoi(trans_str);
			if(CompareCMD_tail(4,2,"00"))			//	  == 00  ��ǰ����
			{
				Set_Menu1_Value(0,trans_int);
				//send_PC('a');
      }
			else if(CompareCMD_tail(4,2,"01"))//		== 01  �����ѹ
			{
				Set_Menu1_Value(1,trans_int);
				//send_PC('b');
			}
			else if(CompareCMD_tail(4,2,"02"))//		== 02  �������
			{
				Set_Menu1_Value(2,trans_int);
				//send_PC('c');
			}
			else if(CompareCMD_tail(4,2,"03"))//		== 03	 ���̵���
			{
				Set_Menu1_Value(3,trans_int);
			}
			else if(CompareCMD_tail(4,2,"04"))//		== 04  ������
			{
				Set_Menu1_Value(4,trans_int);
			}
			else if(CompareCMD_tail(4,2,"05"))//		== 05  �����¶�
			{
				Set_Menu1_Value(5,trans_int);
			}
			else if(CompareCMD_tail(4,2,"06"))//		== 06  �����¶�
			{
				Set_Menu1_Value(6,trans_int);
			}
			else if(CompareCMD_tail(4,2,"07"))//		== 07  IGBTƵ��
			{
				Set_Menu1_Value(7,trans_int);
			}
			else if(CompareCMD_tail(4,2,"08"))//		== 08  IGBT�¶�
			{
				Set_Menu1_Value(8,trans_int);
			}
			else if(CompareCMD_tail(4,2,"09"))//		== 09  ʹ������
			{
				Set_Menu1_Value(9,trans_int);
			}
			else if(CompareCMD_tail(4,2,"10"))//		== 10  ����״̬
			{
				Set_Menu1_Value(10,trans_int);
			}
			else if(CompareCMD_tail(4,2,"11"))//		==11   ͨ��״̬
			{
			
			}
			else if(CompareCMD_tail(4,2,"50"))//		== 50   ��������
			{
				Set_Menu2_Value(0,trans_int);														
			}
			else if(CompareCMD_tail(4,2,"51"))//		== 51		�����
			{
				Set_Menu2_Value(1,trans_int);																
			}  
			else if(CompareCMD_tail(4,2,"52"))//		== 52		������̵���
			{
				Set_Menu2_Value(2,trans_int);	
			}
			else if(CompareCMD_tail(4,2,"53"))//		== 53		��������¶�
			{
				Set_Menu2_Value(3,trans_int);	
			}
			else if(CompareCMD_tail(4,2,"54"))//		== 54		�������¶�
			{
				Set_Menu2_Value(4,trans_int);	
			}
			else if(CompareCMD_tail(4,2,"55"))//		== 55		���©���¶�
			{
				Set_Menu2_Value(5,trans_int);	
			}
			else if(CompareCMD_tail(4,2,"56"))//		== 56		���IGBTƵ��
			{
				Set_Menu2_Value(6,trans_int);	
			}
			else if(CompareCMD_tail(4,2,"57"))//		== 57		��СIGBTƵ��
			{
				Set_Menu2_Value(7,trans_int);	
			}				
			else if(CompareCMD_tail(4,2,"58"))//		== 58		���IGBT�¶�	
			{
				Set_Menu2_Value(8,trans_int);	
			}
			else if(CompareCMD_tail(4,2,"59"))//		== 59		���ƹ���ʱ��
			{
				Set_Menu2_Value(9,trans_int);	
			}
			else if(CompareCMD_tail(4,2,"60"))//		== 60		�ָ���������
			{
				Set_Menu2_Value(10,trans_int);	
			}
			else if(CompareCMD_tail(4,2,"XX"))//���յ� ����������
			{
				WaitHeartTime = 0;
			}
		}
	}
  return 0;
}


//void send_PC(int8 ch)
//{
//		SBUF = ch;
//		while(TI==0);
//		TI=0;
//}



