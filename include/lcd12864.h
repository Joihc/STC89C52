// Header: lcd12864.h
// File Name: lcd12864.h
// Author: hg
// Date:2018年5月24日

#ifndef LCD12864_H_
#define LCD12864_H_

#include "config.h"

#ifndef LCD12864_C_
#endif

#define Busy 0x01 //反序  正序是0x80

void Display_Start();

void Display_String(uint8 line,uint8 *string);
void Display_Pic();

void Wr_Data(uint8 wrdata); //写数据
void Wr_Command(uint8 wrcommand,uint8 busyc); //写指令
void RDbf(void);//读忙状态
void Lcm_Init(void);//液晶初始化
void Set_LCMdata(uint8 wrdata);//倒序

#endif