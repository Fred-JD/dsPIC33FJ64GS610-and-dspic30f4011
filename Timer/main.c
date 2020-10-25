#include "main.h"

int main()
{
	TRIS();
	PWM_Setting();
	Timer1();
	Timer2();
	Timer3();
	Timer4();
	int flag1 = 0, flag2 = 0;
	int LED2_Mode = 0;

	while(1)
	{
		D1_T1_L2(LED2_Mode);
		if(PB1 == 0 && flag1 == 0)
		{
			flag1 = 1;
			LED2_Mode++;
		}
		else if(PB1 == 1 && flag1 == 1)
		{
			flag1 = 0;
		}
		if(PB2 == 0 && flag2 == 0)
		{
			flag2 = 1;
			D1_T2();
		}
		else if(PB2 == 1 && flag2 == 1)
		{
			flag2 = 0;
		}			
	}
	
	return 0;
}
