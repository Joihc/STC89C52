// Header: usart.h
// File Name: usart.h
// Author: hg
// Date:2018年5月24日

#ifndef USART_H_
#define USART_H_

#include "config.h"

#ifndef USART_C_

#endif

void Usart_Update();
void Send_To_Client_Packet(uint8 str,int32 num);
void Interrupt_Queue(int8 sbuf);
uint8 Take_Queue(uint8 *newchar);
void SendHeartBeadPacket();
void Init_USART();//定时器1用作波特率发生器
//bit CompareCMD_head(char CMD_head[]);//比较指令头部  
bit CompareCMD_tail(uint8 start,uint8 quality,int8 CMD_tail[]);//比较指令尾部(start:从哪里开始比较，quality:比较多少个字符，CMD_tail[]：要比较的字符串)
bit Deal_UART_RecData();//处理串口接收数据包函数（成功处理数据包则返回1，否则返回0）  
void send_PC(int8 ch);

#endif