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
	int flag1 = 0, flag2 = 0;

	while(1)
	{
		Task1();
		if(PB1 == 0 && flag1 == 0)
		{
			flag1 = 1;
			T3CONbits.TON = 1;
		}
		else if(PB1 == 1 && flag1 == 1)
		{
			flag1 = 0;
		}
		if(PB2 == 0 && flag2 == 0)
		{
			flag2 = 1;
			WriteStringUART1(Year);
			WriteStringUART1(Space);
			WriteStringUART1(Month);
			WriteStringUART1(Space);
			WriteStringUART1(Day);
		
			T4CONbits.TON = 1;
		}
		else if(PB2 == 1 && flag2 == 1)
		{
			flag2 = 0;
		}		


	}
	
	return 0;
}



