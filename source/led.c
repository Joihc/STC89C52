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
    int flag = 1; //判断符号

    while (*str == ' '||*str == '0')  //跳过字符串前面的空格
    {
        str++;
    }

    if (*str == '-')  //第一个字符若是‘-’，说明可能是负数
    {
        flag = 0;
        str++;
    }
    else if (*str == '+') //第一个字符若是‘+’，说明可能是正数
    {
        flag = 1;
        str++;
    }//第一个字符若不是‘+’‘-’也不是数字字符，直接返回0
		
		while (*str == ' '||*str == '0')  //跳过字符串前面的空格
    {
        str++;
    }
		
    if (*str > '9' || *str < '0') 
    {
        return 0;    
    }

    //当遇到非数字字符或遇到‘/0’时，结束转化
    while (*str != '#' && *str != '\0' && *str <= '9' && *str >= '0')
    {
        value = value * 10 + *str - 0x30; //将数字字符转为对应的整形数
        str++;
    }

    if (flag == 0) //负数的情况
    {
        value = -value;
    }

    return value;
}
int32 My_Pow(int base, int exponent) 
{
    int32 ans = 1, last_pow = base;
    while (exponent > 0) {
        // 如果最低位为1
        if ((exponent & 1) != 0)
            ans = ans * last_pow ;
        exponent = exponent >> 1;
        last_pow = last_pow * last_pow;
    }
    return ans;
}