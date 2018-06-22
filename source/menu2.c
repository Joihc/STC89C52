#define MENU2_C_
#include "menu2.h"


#define MENU2_INDEX 11
#define MENU2_NUM 12
uint8  code set_str[MENU2_INDEX][MENU2_NUM] = {
	{"  ��������  "},
	{"  �����  "},
	{"������̵���"},
	{"��������¶�"},
	{"�������¶�"},		
	{"���©���¶�"},	
	{"���IGBTƵ��"},
	{"��СIGBTƵ��"},
	{"���IGBT�¶�"},
	{"���ƹ���ʱ��"},
	{"�ָ���������"},
};

static int16 set_value[MENU2_INDEX]={
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

static int16 set_view_value =0;

bit menu2_value_update = 0;

int8 menu2_now_page = 0;
int8 menu2_old_page = -1;
bit menu2_display =0;

bit menu2_set =0;//������״̬
uint8 menu2_set_num = 0;//Ĭ������0λ  ���� 0 1 2 3 4

static uint8 menu2_str[16]={""};

static uint32 heart_beat_men2 =0;

//��ʾ�˵�2����
void Discrable_Menu2_String(uint8 per)
{
	uint8 mark;
	int16 value = set_value[per];
	
	set_view_value = set_value[per];//������ʾ������
	
	for(mark=0;mark<16;mark++)
	{
		menu2_str[mark]=' ';
	}
	for(mark=2;mark<14;mark++)
	{
		menu2_str[mark]=set_str[per][mark-2];
	}
	Display_String(1,menu2_str);//�˵�ͷ
	
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
	Display_String(3,menu2_str);//�˵�ֵ
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
		Display_String(2,menu2_str);//�˵�ֵ
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
		Display_String(4,menu2_str);//�˵�ֵ
}

//���ض���ҳ 0 1 2 3
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
				if(set_view_value-My_Pow(10,menu2_set_num)<0)
				{
					set_view_value =0;
				}
				else
				{
					set_view_value -= My_Pow(10,menu2_set_num);
				}
				menu2_value_update = 1;
				
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
				if(set_view_value + My_Pow(10,menu2_set_num) > 32767)
				{
					set_view_value = 32767;
				}
				else
				{
					set_view_value += My_Pow(10,menu2_set_num);
				}
				menu2_value_update = 1;
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
			menu2_set_num =4;//�л�һ�ξͰ�����λ����
			if(!menu2_set)
			{
				//����
				Send_To_Client_Packet(50+menu2_old_page,set_view_value);
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
		
		Usart_Update();	
		if(!menu2_set && set_view_value !=set_value[menu2_old_page] )
		{
			set_view_value = set_value[menu2_old_page];
			menu2_value_update = 1;
		}			
		if(menu2_now_page == menu2_old_page && menu2_display==1)
		{
			//ֻ��������
			if(menu2_value_update)
			{
				menu2_value_update =0;
				Update_Menu2_Value();
			}
//			if(!menu2_set && set_value[menu2_old_page]!=set_view_value)
//			{
//				set_view_value = set_value[menu2_old_page];
//				Update_Menu2_Value();
//			}
			continue;
		}
		menu2_display = 1;
		
		menu2_old_page = menu2_now_page;
		
		Wr_Command(0x01,1); //��ʾ����

		Discrable_Menu2_String(menu2_old_page);	


	}
	menu2_display = 0;
	menu2_set=0;//�˳�����ģʽ����ʾģʽ
}

void Update_Menu2_Value()
{
	uint8 i =0;
	for(i=0;i<16;i++)
	{
		menu2_str[i]=' ';
	}	
	menu2_str[5] = set_view_value%100000/10000+0x30;	
	menu2_str[6] = set_view_value%10000/1000+0x30;
	menu2_str[7] = set_view_value%1000/100+0x30;
	menu2_str[8] = set_view_value%100/10+0x30;
	menu2_str[9]	='.';	
	menu2_str[10]	=set_view_value%10+0x30;
	Display_String(3,menu2_str);//�˵�ֵ
}
void Set_Menu2_Value(int8 index,int16 value)
{
	set_value[index] = value;
}
