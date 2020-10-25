#include "main.h"


int main()
{
	TRIS_init();
	Timer1();
	Init_UART();
	S0 = 0;
	S1 = 0;
	S2 = 0;
	//int u = 0;
	int flag1=0;
	int flag2 =0;
	while(1){
		//Task();
		Mux();
		if(PB1 == 0 && flag1 == 0)
		{
			flag1 = 1;
			S0 = 0;
			S1 = 0;
			S2 = 1;
		}
		else if(PB1 == 1 && flag1 == 1)
		{
			flag1 = 0;
		}
		if(PB2 == 0 && flag2 == 0)
		{
			flag2 = 1;
			S0 = 0;
			S1 = 1;
			S2 = 0;

		}
		else if(PB2 == 1 && flag2 == 1)
		{
			flag2 = 0;
		}
	}
}	