#define TIME0_C_
#include "time0.h"

void Init_TIME0()
{
	TMOD = 0x01;                    //set timer0 as mode1 (16-bit)
  TL0 = T1MS;                     //initial timer0 low byte
  TH0 = T1MS >> 8;                //initial timer0 high byte
  TR0 = 1;                        //timer0 start running
  ET0 = 1;                        //enable timer0 interrupt
}
void Interrupt_Init0()
{
	TL0 = T1MS;                     //initial timer0 low byte
  TH0 = T1MS >> 8;                //initial timer0 high byte
}