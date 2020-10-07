#include "delay.h"

void delay_us(unsigned int d)
{	/* for 120 MHz @ Level0 Opt */
	unsigned int i;
	for (i=0; i<10*d; i++);
}

void delay_ms(unsigned int d)
{	/* for 120 MHz @ Level0 Opt */
	unsigned int i;
	i=d;while(i){i--;delay_us(998);}
}
