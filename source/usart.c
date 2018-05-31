#define USART_C_
#include "usart.h"


//------------------����ͨ��Э��-----------------//
/*
    ����A01_00_0000000#
		����B01_00_0000000#
*/
int8 buf_string[16];  							//��������
#define deviceID_1Bit '0'           //���ڴ���ͨ��ʱ�����屾���豸ID�ĵ�1λ
#define deviceID_2Bit '1'           //���ڴ���ͨ��ʱ�����屾���豸ID�ĵ�2λ
#define Service_headflag 'A'        //���ڴ���ͨ��ʱ���������ݰ�ͷ������֤���  ��ʾ��������  ���͸���������
#define Clinet_headflag 'B' 				//���ڴ���ͨ��ʱ���������ݰ�ͷ������֤���	��ʾ�ͻ��˰�	�ͻ���Ҫ�˽��յ�

#define deviceID "01"

//7-13 Ϊ���� ��*100
//����-9999.99~+9999.99
int8 DataPackage[16]={Service_headflag,deviceID_1Bit,deviceID_2Bit,'_','0','1','_','X','X','X','X','X','X','X','#'};
int8 HeartBeat[16]={Service_headflag,deviceID_1Bit,deviceID_2Bit,'_','B','e','a','t','X','X','X','X','X','X','#'};

int16 trans_int;
int8 trans_str[7];
//----------------------------------------------//
//�ͻ��˰����� 10-13  ����ֻ�м�(1)��(0)
/* 
str == 00  ��ǰ����
		== 01  �����ѹ
		== 02  �������
		== 03	 ���̵���
		== 04  ������
		== 05  �����¶�
		== 06  �����¶�
		== 07  IGBTƵ��
		== 08  IGBT�¶�
		== 09  ʹ������
		== 10  ����״̬


str1 == 40 ����




str2 ==80
*/
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
	for(buf=0;buf<15;buf++)
	{
		SBUF = DataPackage[buf];
		while(TI==0);
    TI=0;  
	}
	ES=1; 
}                                               
//���ڽ��պ���
bit ReceiveServicePacket()    
{
    char *RecStr=buf_string;
    char num=0;
    unsigned int count=0;
    loop:    
    *RecStr=SBUF;
    count=0;
    RI=0; 
    if(num<14)  //���ݰ�����Ϊ15���ַ�,������������15���ַ�
    {
        num++;
        RecStr++;    
        while(!RI)
				{
            count++;
            if(count>1000)
						{
							return 0;    //�������ݵȴ��ӳ٣��ȴ�ʱ��̫�ûᵼ��CPU�������ã�̫�̻����"���ݰ����ָ�",Ĭ��count=130
						}
				}
        goto loop;
    }
    return 1;
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
}
//�Ƚ�ָ��ͷ��
bit CompareCMD_head(char CMD_head[])    
{
    unsigned char CharNum;
    for(CharNum=0;CharNum<4;CharNum++)  //ָ���Ϊ10���ַ�
    {
        if(!(buf_string[CharNum+4]==CMD_head[CharNum]))
        {
            return 0;  //ָ��ͷ��ƥ��ʧ��
        }
    }
    return 1;        //ָ��ͷ��ƥ��ɹ�
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
	//���������ɱ�־λ
//	  == 00  ��ǰ����
//		== 01  �����ѹ
//		== 02  �������
//		== 03	 ���̵���
//		== 04  ������
//		== 05  �����¶�
//		== 06  �����¶�
//		== 07  IGBTƵ��
//		== 08  IGBT�¶�
//		== 09  ʹ������
//		== 10  ����״̬
//	int8 i;
//	ES=0; //�رս����ж�
//	for(i=0;i<15;i++)
//	{
//		SBUF = buf_string[i];
//		while(TI==0);
//		TI=0;  
//	}
//	ES=1; 
	send_PC('|');
	if(buf_string[0]==Clinet_headflag&&buf_string[14]=='#')  //�������ݰ�ͷβ�����֤
	{        
		send_PC('|');
		if(CompareCMD_tail(1,2,deviceID))
		{
			send_PC('|');
			strncpy(trans_str,buf_string+7,7);
			trans_int = my_atoi(trans_str);
			if(CompareCMD_tail(4,2,"00"))			//	  == 00  ��ǰ����
			{
				Set_Menu1_Value(0,trans_int);
      }
			else if(CompareCMD_tail(4,2,"01"))//		== 01  �����ѹ
			{
				Set_Menu1_Value(1,trans_int);
			}
			else if(CompareCMD_tail(4,2,"02"))//		== 02  �������
			{
				Set_Menu1_Value(2,trans_int);
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
		}
	}
  return 0;
}

void send_PC(int8 ch)
{
		SBUF = ch;
		while(TI==0);
		TI=0;
}



