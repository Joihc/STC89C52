#define USART_C_
#include "usart.h"


//------------------串口通信协议-----------------//
/*
    接收A01_00_0000000#
		发送B01_00_0000000#
*/
int8 buf_string[16];  							//发送数据
#define deviceID_1Bit '0'           //用于串口通信时，定义本地设备ID的第1位
#define deviceID_2Bit '1'           //用于串口通信时，定义本地设备ID的第2位
#define Service_headflag 'A'        //用于串口通信时，定义数据包头部的验证标记  表示服务器包  发送给服务器的
#define Clinet_headflag 'B' 				//用于串口通信时，定义数据包头部的验证标记	表示客户端包	客户需要端接收的

#define deviceID "01"

//7-13 为数据 均*100
//数据-9999.99~+9999.99
int8 DataPackage[16]={Service_headflag,deviceID_1Bit,deviceID_2Bit,'_','0','1','_','X','X','X','X','X','X','X','#'};
int8 HeartBeat[16]={Service_headflag,deviceID_1Bit,deviceID_2Bit,'_','B','e','a','t','X','X','X','X','X','X','#'};

int16 trans_int;
int8 trans_str[7];
//----------------------------------------------//
//客户端包描述 10-13  参数只有加(1)减(0)
/* 
str == 00  当前功率
		== 01  供电电压
		== 02  供电电流
		== 03	 线盘电流
		== 04  最大电流
		== 05  线盘温度
		== 06  锅内温度
		== 07  IGBT频率
		== 08  IGBT温度
		== 09  使用容量
		== 10  机器状态


str1 == 40 密码




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
		
	ES=0; //关闭接收中断
	for(buf=0;buf<15;buf++)
	{
		SBUF = DataPackage[buf];
		while(TI==0);
    TI=0;  
	}
	ES=1; 
}                                               
//串口接收函数
bit ReceiveServicePacket()    
{
    char *RecStr=buf_string;
    char num=0;
    unsigned int count=0;
    loop:    
    *RecStr=SBUF;
    count=0;
    RI=0; 
    if(num<14)  //数据包长度为15个字符,尝试连续接收15个字符
    {
        num++;
        RecStr++;    
        while(!RI)
				{
            count++;
            if(count>1000)
						{
							return 0;    //接收数据等待延迟，等待时间太久会导致CPU运算闲置，太短会出现"数据包被分割",默认count=130
						}
				}
        goto loop;
    }
    return 1;
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
}
//比较指令头部
bit CompareCMD_head(char CMD_head[])    
{
    unsigned char CharNum;
    for(CharNum=0;CharNum<4;CharNum++)  //指令长度为10个字符
    {
        if(!(buf_string[CharNum+4]==CMD_head[CharNum]))
        {
            return 0;  //指令头部匹配失败
        }
    }
    return 1;        //指令头部匹配成功
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
	//清除发送完成标志位
//	  == 00  当前功率
//		== 01  供电电压
//		== 02  供电电流
//		== 03	 线盘电流
//		== 04  最大电流
//		== 05  线盘温度
//		== 06  锅内温度
//		== 07  IGBT频率
//		== 08  IGBT温度
//		== 09  使用容量
//		== 10  机器状态
//	int8 i;
//	ES=0; //关闭接收中断
//	for(i=0;i<15;i++)
//	{
//		SBUF = buf_string[i];
//		while(TI==0);
//		TI=0;  
//	}
//	ES=1; 
	send_PC('|');
	if(buf_string[0]==Clinet_headflag&&buf_string[14]=='#')  //进行数据包头尾标记验证
	{        
		send_PC('|');
		if(CompareCMD_tail(1,2,deviceID))
		{
			send_PC('|');
			strncpy(trans_str,buf_string+7,7);
			trans_int = my_atoi(trans_str);
			if(CompareCMD_tail(4,2,"00"))			//	  == 00  当前功率
			{
				Set_Menu1_Value(0,trans_int);
      }
			else if(CompareCMD_tail(4,2,"01"))//		== 01  供电电压
			{
				Set_Menu1_Value(1,trans_int);
			}
			else if(CompareCMD_tail(4,2,"02"))//		== 02  供电电流
			{
				Set_Menu1_Value(2,trans_int);
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



