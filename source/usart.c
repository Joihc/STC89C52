#define USART_C_
#include "usart.h"


//------------------串口通信协议-----------------//
/*
    接收B01_00_0000000#
		发送A01_00_0000000#
*/
uint8 buf_string[16];  							//接收数据
#define deviceID_1Bit '0'           //用于串口通信时，定义本地设备ID的第1位
#define deviceID_2Bit '1'           //用于串口通信时，定义本地设备ID的第2位
#define Service_headflag 'A'        //用于串口通信时，定义数据包头部的验证标记  表示服务器包  发送给服务器的
#define Clinet_headflag 'B' 				//用于串口通信时，定义数据包头部的验证标记	表示客户端包	客户需要端接收的

#define deviceID "01"

//7-13 为数据 均*100
//数据-9999.99~+9999.99
uint8 DataPackage[16]={Service_headflag,deviceID_1Bit,deviceID_2Bit,'_','0','1','_','X','X','X','X','X','X','X','#'};
uint8 HeartBeat[16]={Service_headflag,deviceID_1Bit,deviceID_2Bit,'_','X','X','_','0','0','0','0','0','0','0','#'};
uint8 InitPackage[16]={Service_headflag,deviceID_1Bit,deviceID_2Bit,'_','X','1','_','0','0','0','0','0','0','0','#'};
//开启顺序：开启->发送InitPackage->接收所有数据->并刷新当前界面
//							->每隔100次循环发送HeartBeat->100次循环里面接收，如果接收不到则说明通信故障
//							->修改当前菜单2的数据->发送修改后的数据->接收并刷新当前界面
int16 trans_int;
uint8 trans_str[7];

struct FifoQueue usart_queue;//串口通信仓库

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
		Set_Menu1_Value(11,0);//没有通信故障
	}
	else
	{
		Set_Menu1_Value(11,1);//通信故障
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
		
	ES=0; //关闭接收中断
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
	ES=0; //关闭接收中断
	for(i=0;i<15;i++)
	{
		SBUF = HeartBeat[i];
		while(TI==0);
    TI=0;  
	}
	ES=1; 
}
//定时器1用作波特率发生器
void Init_USART()  
{
    SCON=0x50;  //串口方式1，使能接收
    TMOD|=0x20;  //定时器1工作方式2（8位自动重装初值）
    TMOD&=~0x10;
    TH1=0xf3;   // 计数器初始值设置，注意波特率是4800的  12M晶振
    TL1=0xf3;  
    PCON|=0x80;  //SMOD=1
    TR1=1;
    TI=0;
    RI=0;
    //PS=1;   //提高串口中断优先级
    ES=1;  //开启串口中断使能
	
		QueueInit(&usart_queue);
}
//装入数据
void Interrupt_Queue(uint8 sbuf)
{
	QueueIn(&usart_queue,sbuf);
}
//取出B字开头的15个数据
uint8 Take_Queue(uint8 *newchar)
{
	uint8 i =0;
	uint8 temp;
	QueueCheckOut(&usart_queue,Clinet_headflag);
	if(usart_queue.count < 15)
	{
		return 0;//不足15个数据
	}
	while(i<15 && QueueOut(&usart_queue,&temp) != QueueEmpty)
	{
		if(temp == Clinet_headflag || i != 0)
		{
			newchar[i] = temp;
			i++;
		}
	}
	if(i==15)//取了完整的数据
	{
		return 1;
	}
	return 0;
}
//比较指令尾部(start:从哪里开始比较，quality:比较多少个字符，CMD_tail[]：要比较的字符串)
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

bit Deal_UART_RecData()   //处理串口接收数据包函数（成功处理数据包则返回1，否则返回0）
{	
	if(buf_string[0]==Clinet_headflag&&buf_string[14]=='#')  //进行数据包头尾标记验证
	{        
		if(CompareCMD_tail(1,2,deviceID))
		{
			strncpy(trans_str,buf_string+7,7);
			trans_int = my_atoi(trans_str);
			if(CompareCMD_tail(4,2,"00"))			//	  == 00  当前功率
			{
				Set_Menu1_Value(0,trans_int);
				//send_PC('a');
      }
			else if(CompareCMD_tail(4,2,"01"))//		== 01  供电电压
			{
				Set_Menu1_Value(1,trans_int);
				//send_PC('b');
			}
			else if(CompareCMD_tail(4,2,"02"))//		== 02  供电电流
			{
				Set_Menu1_Value(2,trans_int);
				//send_PC('c');
			}
			else if(CompareCMD_tail(4,2,"03"))//		== 03	 线盘电流
			{
				Set_Menu1_Value(3,trans_int);
			}
			else if(CompareCMD_tail(4,2,"04"))//		== 04  最大电流
			{
				Set_Menu1_Value(4,trans_int);
			}
			else if(CompareCMD_tail(4,2,"05"))//		== 05  线盘温度
			{
				Set_Menu1_Value(5,trans_int);
			}
			else if(CompareCMD_tail(4,2,"06"))//		== 06  锅内温度
			{
				Set_Menu1_Value(6,trans_int);
			}
			else if(CompareCMD_tail(4,2,"07"))//		== 07  IGBT频率
			{
				Set_Menu1_Value(7,trans_int);
			}
			else if(CompareCMD_tail(4,2,"08"))//		== 08  IGBT温度
			{
				Set_Menu1_Value(8,trans_int);
			}
			else if(CompareCMD_tail(4,2,"09"))//		== 09  使用容量
			{
				Set_Menu1_Value(9,trans_int);
			}
			else if(CompareCMD_tail(4,2,"10"))//		== 10  机器状态
			{
				Set_Menu1_Value(10,trans_int);
			}
			else if(CompareCMD_tail(4,2,"11"))//		==11   通信状态
			{
			
			}
			else if(CompareCMD_tail(4,2,"50"))//		== 50   输入密码
			{
				Set_Menu2_Value(0,trans_int);														
			}
			else if(CompareCMD_tail(4,2,"51"))//		== 51		最大功率
			{
				Set_Menu2_Value(1,trans_int);																
			}  
			else if(CompareCMD_tail(4,2,"52"))//		== 52		最大线盘电流
			{
				Set_Menu2_Value(2,trans_int);	
			}
			else if(CompareCMD_tail(4,2,"53"))//		== 53		最大线盘温度
			{
				Set_Menu2_Value(3,trans_int);	
			}
			else if(CompareCMD_tail(4,2,"54"))//		== 54		最大锅内温度
			{
				Set_Menu2_Value(4,trans_int);	
			}
			else if(CompareCMD_tail(4,2,"55"))//		== 55		最大漏料温度
			{
				Set_Menu2_Value(5,trans_int);	
			}
			else if(CompareCMD_tail(4,2,"56"))//		== 56		最大IGBT频率
			{
				Set_Menu2_Value(6,trans_int);	
			}
			else if(CompareCMD_tail(4,2,"57"))//		== 57		最小IGBT频率
			{
				Set_Menu2_Value(7,trans_int);	
			}				
			else if(CompareCMD_tail(4,2,"58"))//		== 58		最大IGBT温度	
			{
				Set_Menu2_Value(8,trans_int);	
			}
			else if(CompareCMD_tail(4,2,"59"))//		== 59		限制工作时间
			{
				Set_Menu2_Value(9,trans_int);	
			}
			else if(CompareCMD_tail(4,2,"60"))//		== 60		恢复出厂设置
			{
				Set_Menu2_Value(10,trans_int);	
			}
			else if(CompareCMD_tail(4,2,"XX"))//接收到 心跳反馈包
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



