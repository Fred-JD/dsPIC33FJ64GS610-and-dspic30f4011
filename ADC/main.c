#include "main.h"


int main()
{
	TRIS();
	PWM_Setting();
	Timer1();
	Timer2();
	Timer3();
	Timer4();
	UART_Set();
	InitADC();
	
	
	ADCON1bits.ADON = 1;
	int flag1 = 0, flag2 = 0;
	
	while(1)
	{
		Day3_Task3();
		if(PB1 == 0 && flag1 == 0)
		{
			flag1 = 1;
			T2CONbits.TON = 1; 
		}
		else if(PB1 == 1 && flag1 == 1)
		{
			flag1 = 0;
		}
		if(PB2 == 0 && flag2 == 0)
		{
			flag2 = 1;
		}
		else if(PB2 == 1 && flag2 == 1)
		{
			flag2 = 0;
		}		
	}
	return 0;
}




