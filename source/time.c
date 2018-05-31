#define TIME_C_
#include "time.h"

void delayms(uint8 ms)
{
	uint16 TempCyc=ms*1000;
	while(--TempCyc);
}
void delayus(uint8 u)
{
	while(--u);
}


