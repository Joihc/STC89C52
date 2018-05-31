// Header: config.h
// File Name: config.h
// Author: hg
// Date:2018年5月24日

#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdlib.h>
#include <reg52.h>
#include <string.h>

//****************************类型缩写******************************
typedef signed short int4;
typedef signed char int8;//8位有符号整型
typedef signed int int16;//16位有符号整型
typedef signed long int32;//32位有符号整型
typedef unsigned short uint4;//16
typedef unsigned char uint8;//8位无符号256
typedef unsigned int uint16;//16位无符号 1-35535
typedef unsigned long uint32;//32位无符号 1- 
//******************************************************************

#define Set_Bit(byte,bit) (byte|=(1<<bit))//对byte的bit位置取1
#define Clr_Bit(byte,bit) (byte&=~(1<<bit))//对byte的bit位置取0
#define Com_Bit(byte,bit) (byte^=(1<<bit))//对byte的bit位置取反
#define Test_Bit(byte,bit) (byte&(1<<bit))//对byte的bit结果
//******************************************************************
//******************************端口定义****************************
//LCD12864
#define LCMdata P0

sbit RS=P3^4;
sbit RW=P3^3;
sbit E=P3^2;
sbit PSB=P2^7;
sbit REST=P2^6;

//LED和BEEP
sbit LED=P2^0;
sbit BEEP = P1^0;

//按键
sbit UP_BTN		= P1^5;
sbit DOWN_BTN = P1^2;
sbit LEFT_BTN = P1^3;
sbit RIGHT_BTN=	P1^1;
sbit OK_BTN		= P1^7;
sbit RETURN_BTN = P1^4;

//******************************************************************

#define FOSC 12000000L

#define T1MS (65536-FOSC/12/1000)   //1ms timer calculation method in 12T mode

#include "lcd12864.h"
#include "usart.h"
#include "time.h"
#include "led.h"
#include "time0.h"
#include "time1.h"
#include "button.h"
#include "menu1.h"
#include "menu2.h"
#include "menu3.h"



#endif