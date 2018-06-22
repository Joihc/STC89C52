#define MENU1_C_
#include "menu1.h"

#define MENU1_INDEX 12
#define MENU1_NUM 9
uint8  code display_str[MENU1_INDEX][MENU1_NUM] = {
	{"当前功率:"},
	{"供电电压:"},
	{"供电电流:"},
	{"线盘电流:"},
	{"最大电流:"},
	{"线盘温度:"},
	{"锅内温度:"},									 
	{"IGBT频率:"},
	{"IGBT温度:"},
	{"使用容量:"},
	{"机器状态:"},
	{"通信故障:"},
};
//最大-9999~+ 32767
static int16 display_value[MENU1_INDEX]={
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
};
int8 menu1_now_page = 0;
int8 menu1_old_page = -1;
bit menu1_display =0;

static uint8 menu1_str[16]={""};
static uint32 heart_beat_men1 =0;

bit menu1_value_update =0;

//显示菜单1界面
void Discrable_Menu1_String(uint8 *string,int16 value,uint8 per)
{
	uint8 mark;
	for(mark=0;mark<9;mark++)
	{
		menu1_str[mark]=string[mark];
	}
	if(value <-9999)
	{
		value = -9999;
	}
	mark = (value<0?1:0);
	value = abs(value);
	
	
	menu1_str[9] = value%100000/10000+0x30;
	if(menu1_str[9]=='0')
	{
		menu1_str[9] = ' ';
		if(mark && (value>999))
		{
			menu1_str[9] = '-';
			mark =0;		
		}
	}
	
	menu1_str[10] = value%10000/1000+0x30;
	if(menu1_str[10]=='0' && menu1_str[9]==' ')
	{
		menu1_str[10] = ' ';
		if(mark && (value>99))
		{
			menu1_str[10] = '-';
			mark =0;
		}
	}
	menu1_str[11] = value%1000/100+0x30;
	if(menu1_str[11]=='0'&& menu1_str[10]==' ' && menu1_str[10]==' ')
	{
		menu1_str[11] = ' ';
		if(mark)
		{
			menu1_str[11] = '-';
			mark =0;			
		}
	}
	menu1_str[12] = value%100/10+0x30;
	menu1_str[13]	='.';	
	menu1_str[14]	=value%10+0x30;
	switch(per)
	{
		case 0:
			menu1_str[15] = 'K';
			break;
		case 1:
			menu1_str[15] = 'V';
			break;
		case 2:
			menu1_str[15] = 'A';
			break;
		case 3:
			menu1_str[15] = 'A';
			break;
		case 4:
			menu1_str[15] = 'A';
			break;
		case 5:
			menu1_str[15] = 'C';
			break;
		case 6:
			menu1_str[15] = 'C';
			break;
		case 7:
			menu1_str[15] = 'K';
			break;
		case 8:
			menu1_str[15] = 'C';
			break;
		case 9:
			menu1_str[15] = '%';
			break;
		default:
			menu1_str[15] = ' ';
			break;
	}
}
	
//返回多少页 0 1 2 3
void Display_Menu1_Screen()
{
	while(1)
	{
		if(menu1_old_page < (MENU1_INDEX-1)/4 && Down_Key_Down())
		{
			menu1_now_page++;
		}
		else if(menu1_old_page >0 && Up_Key_Down())
		{
			menu1_now_page--;
		}
		else  if(Return_Key_Down())
		{	
			break;
		}
		
		Usart_Update();
		
		if(menu1_now_page == menu1_old_page &&  menu1_display == 1)
		{
			if(menu1_value_update)
			{
				menu1_value_update = 0;
				Update_Menu1_Value();
			}
			continue;
		}
		menu1_display=1;
		menu1_old_page = menu1_now_page;
		
		Wr_Command(0x01,1); //显示清屏
		if(menu1_old_page*4<MENU1_INDEX)
		{
			Discrable_Menu1_String(display_str[menu1_old_page*4],display_value[menu1_old_page*4],menu1_old_page*4);
			Display_String(1,menu1_str);
		}
		if(menu1_old_page*4+1<MENU1_INDEX)
		{
			Discrable_Menu1_String(display_str[menu1_old_page*4+1],display_value[menu1_old_page*4+1],menu1_old_page*4+1);
			Display_String(2,menu1_str);
		}
		if(menu1_old_page*4+2<MENU1_INDEX)
		{
			Discrable_Menu1_String(display_str[menu1_old_page*4+2],display_value[menu1_old_page*4+2],menu1_old_page*4+2);
			Display_String(3,menu1_str);
		}
		if(menu1_old_page*4+3<MENU1_INDEX)
		{
			Discrable_Menu1_String(display_str[menu1_old_page*4+3],display_value[menu1_old_page*4+3],menu1_old_page*4+3);
			Display_String(4,menu1_str);
		}
	}
	menu1_display = 0;
}
void Update_Menu1_Value()
{
	uint8 mark;
	uint8 i=0;
	int16 value;
	for(i=menu1_old_page*4;i<MENU1_INDEX && i<(menu1_old_page*4+4);i++)
	{
			mark = (display_value[i]<0?1:0);
			switch(i%4)
			{
				case 0:
					Wr_Command(0x84,1);
					break;
				case 1:
					Wr_Command(0x94,1);
					break;
				case 2:
					Wr_Command(0x8c,1);
					break;
				case 3:
					Wr_Command(0x9c,1);
					break;
			}
			menu1_str[8]=':';
			
			value = abs(display_value[i]);
	
	
			menu1_str[9] = value%100000/10000+0x30;
			if(menu1_str[9]=='0')
			{
				menu1_str[9] = ' ';
				if(mark && (value>999))
				{
					menu1_str[9] = '-';
					mark =0;		
				}
			}
	
			menu1_str[10] = value%10000/1000+0x30;
			if(menu1_str[10]=='0'&& menu1_str[9]==' ')
			{
				menu1_str[10] = ' ';
				if(mark && (value>99))
				{
					menu1_str[10] = '-';
					mark =0;
				}
			}
			menu1_str[11] = value%1000/100+0x30;
			if(menu1_str[11]=='0'&& menu1_str[10]==' '&& menu1_str[9]==' ')
			{
				menu1_str[11] = ' ';
				if(mark)
				{
					menu1_str[11] = '-';
					mark =0;			
				}
			}
			menu1_str[12] 	= value%100/10+0x30;
			menu1_str[13]	='.';	
			menu1_str[14]	=value%10+0x30;
			for(mark=8;mark<15;mark++)
			{
				Wr_Data(menu1_str[mark]);
			}
	}
}

void Set_Menu1_Value(int8 index,int16 value)
{
	if(value <-9999)
	{
		value = -9999;
	}
	if(display_value[index] != value)
	{
		display_value[index] = value;
		if(index/4 == menu1_old_page && menu1_display)
		{
			menu1_value_update = 1;
		}
	}
}