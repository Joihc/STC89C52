#define MENU2_C_
#include "menu2.h"


#define MENU2_INDEX 14
#define MENU2_NUM 12
uint8  code set_str[MENU2_INDEX][MENU2_NUM] = {
	{"  输入密码  "},
	{"  最大功率  "},
	{"最大供电电压"},
	{"最小供电电压"},
	{"最大线盘电流"},
	{"最大线盘温度"},
	{"通用线盘温度"},
	{"最大锅内温度"},		
	{"最大漏料温度"},	
	{"最大IGBT频率"},
	{"最小IGBT频率"},
	{"最大IGBT温度"},
	{"限制工作时间"},
	{"  修改密码  "},
};

static int16 set_value[MENU2_INDEX]={
	9000,
	888,
	777,
	6666,
	5555,
	444,
	3,
	2222,
	1111,
	000,
	11,
	11,
	0,
	0,
};

int8 menu2_now_page = 0;
int8 menu2_old_page = -1;
bit menu2_display =0;

bit menu2_set =0;//在设置状态
uint8 menu2_set_num = 0;//默认设置0位  左到右 0 1 2 3 4

static int16 menu2_value =0;
static uint8 menu2_str[16]={""};

//显示菜单2界面
void Discrable_Menu2_String(uint8 per)
{
	uint8 mark;
	int16 value = set_value[per];
	for(mark=0;mark<16;mark++)
	{
		menu2_str[mark]=' ';
	}
	for(mark=2;mark<14;mark++)
	{
		menu2_str[mark]=set_str[per][mark-2];
	}
	Display_String(1,menu2_str);//菜单头
	
	for(mark=0;mark<16;mark++)
	{
		menu2_str[mark]=' ';
	}
	if(value <0)
	{
		value = 0;
	}
	//mark = (value<0?1:0);
	value = abs(value);
	
//	menu2_str[0]='<';
//	menu2_str[1]='<';
//	menu2_str[2]='<';
//	menu2_str[3]='<';
//	menu2_str[4]='<';
	menu2_str[5] = value%100000/10000+0x30;	
	menu2_str[6] = value%10000/1000+0x30;
	menu2_str[7] = value%1000/100+0x30;
	menu2_str[8] = value%100/10+0x30;
	menu2_str[9]	='.';	
	menu2_str[10]	=value%10+0x30;
//	menu2_str[11]='>';
//	menu2_str[12]='>';
//	menu2_str[13]='>';
//	menu2_str[14]='>';
//	menu2_str[15]='>';
	Display_String(3,menu2_str);//菜单值
}
void Set_Mark()
{
	uint8 mark;
	for(mark=0;mark<16;mark++)
		{
			menu2_str[mark]=' ';
		}
		if(menu2_set)
		{
			switch(menu2_set_num)
			{
				case 0:
					menu2_str[10]=0x18;
					break;
				case 1:
					menu2_str[8]=0x18;
					break;
				case 2:
					menu2_str[7]=0x18;
					break;
				case 3:
					menu2_str[6]=0x18;
					break;
				case 4:
					menu2_str[5]=0x18;
					break;
			}
		}
		Display_String(2,menu2_str);//菜单值
		for(mark=0;mark<16;mark++)
		{
			menu2_str[mark]=' ';
		}
		if(menu2_set)
		{
			switch(menu2_set_num)
			{
				case 0:
					menu2_str[10]=0x19;
					break;
				case 1:
					menu2_str[8]=0x19;
					break;
				case 2:
					menu2_str[7]=0x19;
					break;
				case 3:
					menu2_str[6]=0x19;
					break;
				case 4:
					menu2_str[5]=0x19;
					break;
			}
		}
		Display_String(4,menu2_str);//菜单值
}

//返回多少页 0 1 2 3
void Display_Menu2_Screen()
{
	while(1)
	{
		if(Down_Key_Down())
		{
			if(menu2_old_page < MENU2_INDEX-1 && !menu2_set)
			{
				menu2_now_page++;
			}
			else if(menu2_set)
			{
				if(set_value[menu2_old_page]-My_Pow(10,menu2_set_num)<0)
				{
					menu2_value =0;
				}
				else
				{
					menu2_value = set_value[menu2_old_page]-My_Pow(10,menu2_set_num);
				}
				//Set_Menu2_Value(menu2_old_page,menu2_value);
				//Send_To_Client_Packet(40+menu2_old_page,menu2_value);
				
			}		
		}
		else if(Up_Key_Down())
		{
			if(menu2_old_page >0 && !menu2_set)
			{
				menu2_now_page--;
			}
			else if(menu2_set)
			{
				if(set_value[menu2_old_page] + My_Pow(10,menu2_set_num) > 32767)
				{
					menu2_value = 32767;
				}
				else
				{
					menu2_value = set_value[menu2_old_page] + My_Pow(10,menu2_set_num);
				}			
			}
		}
		else if(Return_Key_Down())
		{
			break;
		}		
		else if(Ok_Key_Down())
		{
			//send_PC('A');
			menu2_set= ~menu2_set;
			menu2_set_num =4;//切换一次就把设置位归零
			if(!menu2_set)
			{
				//保存
				Send_To_Client_Packet(40+menu2_old_page,menu2_value);
			}
		}
		else if(Left_Key_Down() && menu2_set && menu2_set_num <4)
		{
			menu2_set_num++;
		}
		else if(Right_Key_Down() && menu2_set && menu2_set_num >0)
		{
			menu2_set_num--;
		}
		Set_Mark();
		if(menu2_now_page == menu2_old_page && menu2_display==1)
		{
			continue;
		}
		menu2_display = 1;
		
		menu2_old_page = menu2_now_page;
		
		Wr_Command(0x01,1); //显示清屏

		Discrable_Menu2_String(menu2_old_page);	
	}
	menu2_display = 0;
	menu2_set=0;//退出更改模式到显示模式
}

void Set_Menu2_Value(int8 index,int16 value)
{
	index-=40;
	if(set_value[index] != value)
	{
		set_value[index] = value;
		if(index == menu2_old_page && menu2_display)
		{
			for(index=0;index<16;index++)
			{
				menu2_str[index]=' ';
			}
			if(value <0)
			{
				value = 0;
			}
			value = abs(value);
	
			menu2_str[5] = value%100000/10000+0x30;	
			menu2_str[6] = value%10000/1000+0x30;
			menu2_str[7] = value%1000/100+0x30;
			menu2_str[8] = value%100/10+0x30;
			menu2_str[9]	='.';	
			menu2_str[10]	=value%10+0x30;
			Display_String(3,menu2_str);//菜单值
		}
	}
}
