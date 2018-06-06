// Header: usart.h
// File Name: usart.h
// Author: hg
// Date:2018��5��24��

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
void Init_USART();//��ʱ��1���������ʷ�����
//bit CompareCMD_head(char CMD_head[]);//�Ƚ�ָ��ͷ��  
bit CompareCMD_tail(uint8 start,uint8 quality,int8 CMD_tail[]);//�Ƚ�ָ��β��(start:�����￪ʼ�Ƚϣ�quality:�Ƚ϶��ٸ��ַ���CMD_tail[]��Ҫ�Ƚϵ��ַ���)
bit Deal_UART_RecData();//�����ڽ������ݰ��������ɹ��������ݰ��򷵻�1�����򷵻�0��  
void send_PC(int8 ch);

#endif