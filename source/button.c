#define BUTTON_C_
#include "button.h"

static bit up_down;
static bit down_down;
static bit left_down;
static bit right_down;
static bit ok_down;
static bit return_down;

static uint8 up_num;
static uint8 down_num;
static uint8 left_num;
static uint8 right_num;
static uint8 ok_num;
static uint8 return_num;


bit Up_Key_Down()
{
	while(!UP_BTN && !up_down)
	{
		up_num++;
		if(up_num >CHECK_BUTTON_TIME)
		{
			up_num =0;
			up_down =1;
			Beep_Change();
			return 1;
		}
	}
	up_num =0;
	while(UP_BTN && up_down)
	{
		up_num++;
		if(up_num >CHECK_BUTTON_TIME)
		{
			up_num =0;
			up_down =0;
			break;
		}
	}
	up_num =0;
	return 0;
}
bit Down_Key_Down()
{
	while(!DOWN_BTN && !down_down)
	{
			down_num++;
			if(down_num >CHECK_BUTTON_TIME)
			{
				down_num =0;
				down_down =1;
				Beep_Change();
				return 1;
			}
	}
	down_num =0;
	while(DOWN_BTN && down_down)
	{
		down_num++;
		if(down_num >CHECK_BUTTON_TIME)
		{
			down_num =0;
			down_down =0;
			break;
		}
	}
	down_num =0;
	return 0;
}
bit Left_Key_Down()
{
	while(!LEFT_BTN && !left_down)
	{
			left_num++;
			if(left_num >CHECK_BUTTON_TIME)
			{
				left_num =0;
				left_down =1;
				Beep_Change();
				return 1;
			}
	}
	left_num =0;
	while(LEFT_BTN && left_down)
	{
		left_num++;
		if(left_num >CHECK_BUTTON_TIME)
		{
			left_num =0;
			left_down =0;
			break;
		}
	}
	left_num =0;
	return 0;
}
bit Right_Key_Down()
{
	while(!RIGHT_BTN && !right_down)
	{
			right_num++;
			if(right_num >CHECK_BUTTON_TIME)
			{
				right_num =0;
				right_down =1;
				Beep_Change();
				return 1;
			}
	}
	right_num=0;
	while(RIGHT_BTN && right_down)
	{
		right_num++;
		if(right_num >CHECK_BUTTON_TIME)
		{
			right_num =0;
			right_down =0;
			break;
		}
	}
	right_num =0;
	return 0;
}
bit Ok_Key_Down()
{
	while(!OK_BTN && !ok_down)
	{
			ok_num++;
			if(ok_num >CHECK_BUTTON_TIME)
			{
				ok_num =0;
				ok_down =1;
				Beep_Change();
				return 1;
			}
	}
	ok_num =0;
	while(OK_BTN && ok_down)
	{
		ok_num++;
		if(ok_num >CHECK_BUTTON_TIME)
		{
			ok_num =0;
			ok_down =0;
			break;
		}
	}
	ok_num =0;
	return 0;
}
bit Return_Key_Down()
{
	while(!RETURN_BTN && !return_down)
	{
			return_num++;
			if(return_num >CHECK_BUTTON_TIME)
			{
				return_num =0;
				return_down =1;
				Beep_Change();
				return 1;
			}
	}
	return_num =0;
	while(RETURN_BTN && return_down)
	{
		return_num++;
		if(return_num >CHECK_BUTTON_TIME)
		{
			return_num =0;
			return_down =0;
			break;
		}
	}
	return_num =0;
	return 0;
}