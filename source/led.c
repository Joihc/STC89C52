#define LED_C_
#include "led.h"

//void Led_On()
//{
//	LED =1;
//}
//void Led_Off()
//{
//	LED=0;
//}
void Led_Change()
{
	LED=~LED;
}
void Beep_Change()
{
	uint8 i=1000;
	while(i--)
	{
		BEEP=~BEEP;
		delayus(100);	
	}
}

int16 my_atoi (char *str)
{
	  int value = 0;
    int flag = 1; //�жϷ���

    while (*str == ' '||*str == '0')  //�����ַ���ǰ��Ŀո�
    {
        str++;
    }

    if (*str == '-')  //��һ���ַ����ǡ�-����˵�������Ǹ���
    {
        flag = 0;
        str++;
    }
    else if (*str == '+') //��һ���ַ����ǡ�+����˵������������
    {
        flag = 1;
        str++;
    }//��һ���ַ������ǡ�+����-��Ҳ���������ַ���ֱ�ӷ���0
		
		while (*str == ' '||*str == '0')  //�����ַ���ǰ��Ŀո�
    {
        str++;
    }
		
    if (*str > '9' || *str < '0') 
    {
        return 0;    
    }

    //�������������ַ���������/0��ʱ������ת��
    while (*str != '#' && *str != '\0' && *str <= '9' && *str >= '0')
    {
        value = value * 10 + *str - 0x30; //�������ַ�תΪ��Ӧ��������
        str++;
    }

    if (flag == 0) //���������
    {
        value = -value;
    }

    return value;
}
int32 My_Pow(int base, int exponent) 
{
    int32 ans = 1, last_pow = base;
    while (exponent > 0) {
        // ������λΪ1
        if ((exponent & 1) != 0)
            ans = ans * last_pow ;
        exponent = exponent >> 1;
        last_pow = last_pow * last_pow;
    }
    return ans;
}