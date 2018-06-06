// Header: lcd12864.h
// File Name: lcd12864.h
// Author: hg
// Date:2018��5��24��

#ifndef LCD12864_H_
#define LCD12864_H_

#include "config.h"

#ifndef LCD12864_C_
#endif

#define Busy 0x01 //����  ������0x80

void Display_Start();

void Display_String(uint8 line,uint8 *string);
void Display_Pic();

void Wr_Data(uint8 wrdata); //д����
void Wr_Command(uint8 wrcommand,uint8 busyc); //дָ��
void RDbf(void);//��æ״̬
void Lcm_Init(void);//Һ����ʼ��
void Set_LCMdata(uint8 wrdata);//����

#endif