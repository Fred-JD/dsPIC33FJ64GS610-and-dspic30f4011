#include "common.h"
float count = 0;

void TRIS(void)
{
	ADPCFG = 0xffff;
	TRISB = 0b111100011;
	TRISC = 0b0100000000000000;
	TRISD = 0b0110;
	TRISE = 0b000000100000000;
	TRISF = 0b0;

	LED2  = 1; //Active Low
}

void PWM_Setting(void)
{

	PTCON   = 0b1000000000000011;
	PWMCON1 = 0b0000111100000101;
	PDC1 = 	0; 			//Left
	PDC3 = 	0;			//Right
	PTPER = 19999;   			//10M Hz
	OVDCON = 0b0001000100010001;
}

void Timer1(void)
{
	// Configure Timer 1
	T1CON = 0;            // Clear the Timer 1 configuration
	TMR1 = 0;             // Reset Timer 1 counter
	PR1 = 2303.125 ;          // Set the Timer 1 period (max 65535)
	T1CONbits.TCS = 0;    // Select internal clock (Fosc/4)
	T1CONbits.TCKPS = 3;  // Prescaler (0=1:1, 1=1:8, 2=1:64, 3=1:256)
	_T1IP = 1;            // Set the Timer 1 interrupt priority
	_T1IF = 0;            // Clear the Timer 1 interrupt flag
	_T1IE = 1;            // Enable Timer 1 interrupt
	T1CONbits.TON = 1;    // Turn on Timer 1

	// Lock the main function here in an empty loop and let
	// the Timer 1 interrupt service routine do all the work.
}

void Timer2(void)
{
	T2CON = 0;            // Clear the Timer 1 configuration
	TMR2 = 0;             // Reset Timer 1 counter
	PR2 =2303.125 ;          // Set the Timer 1 period (max 65535)
	T2CONbits.TCS = 0;    // Select internal clock (Fosc/4)
	T2CONbits.TCKPS = 3;  // Prescaler (0=1:1, 1=1:8, 2=1:64, 3=1:256)
	_T2IP = 1;            // Set the Timer 1 interrupt priority
	_T2IF = 0;            // Clear the Timer 1 interrupt flag
	_T2IE = 1;            // Enable Timer 1 interrupt
	T2CONbits.TON = 1;    // Turn on Timer 1
	T2CONbits.T32 = 0;
}

void Timer3(void)
{
	T3CON = 0;            // Clear the Timer 1 configuration
	TMR3 = 0;             // Reset Timer 1 counter
	PR3 =57578.125 ;          // Set the Timer 1 period (max 65535)
	T3CONbits.TCS = 0;    // Select internal clock (Fosc/4)
	T3CONbits.TCKPS = 3;  // Prescaler (0=1:1, 1=1:8, 2=1:64, 3=1:256)
	_T3IP = 1;            // Set the Timer 1 interrupt priority
	_T3IF = 0;            // Clear the Timer 1 interrupt flag
	_T3IE = 1;            // Enable Timer 1 interrupt
	T3CONbits.TON = 0;    // Turn off Timer 1
}

void Timer4(void)                
{
	T4CON = 0;            // Clear the Timer 1 configuration
	TMR4 = 0;             // Reset Timer 1 counter
	PR4 =5896;          // Set the Timer 1 period (max 65535)
	T4CONbits.TCS = 0;    // Select internal clock (Fosc/4)
	T4CONbits.TCKPS = 0;  // Prescaler (0=1:1, 1=1:8, 2=1:64, 3=1:256)
	_T4IP = 1;            // Set the Timer 1 interrupt priority
	_T4IF = 0;            // Clear the Timer 1 interrupt flag
	_T4IE = 1;            // Enable Timer 1 interrupt
	T4CONbits.TON = 0;     // Turn off Timer 1
	T4CONbits.T32 = 0;		//Turn off 32bits
}

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void)
{

	_T1IF = 0;

	if (LED1) LED1 = 0; else LED1 = 1;
}   

void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void)
{

	_T2IF = 0;
	
	if (LED2) LED2 = 0; else LED2 = 1;
}   

void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt(void)
{
	_T3IF = 0;
	count = count + 1;
	D1_T2_Led();
}   

void __attribute__((__interrupt__, __auto_psv__)) _T4Interrupt(void)
{
	_T4IF = 0;
	D1_T3_LM();
	D1_T3_RM();
} 

void D1_T1_L2(int a)
{
	switch(a)
	{
		case 0:
			PR2 = 2303.125; 
			break;
		case 1:
			PR2 = 4606.25;
			break;
		case 2:
			PR2 = 6909.375;
			break;
		case 3:
			T2CONbits.TON = 0;
			LED2 = 1;
	}
}		

void D1_T2(void)
{
	T1CONbits.TON = 0;
	T2CONbits.TON = 0;
	T3CONbits.TON = 1;
	T4CONbits.TON = 1;
}	

void D1_T2_Led(void)
{
	if(count < 4)
	{
		LED1 = 1;
		LED2 = 0;
	}	
	else if(count >= 4 && count < 10)
	{
		LED1 = 0;
		LED2 = 1;
	}
	else if(count >= 10 && count < 18)
	{
		LED1 = 1;
		LED2 = 0;
	}
	else if(count >= 18 && count < 30)
	{
		LED1 = 0;
		LED2 = 1;
	}
	else
	{
		LED1 = 1;
		LED2 = 0;
	}	
}	

void D1_T3_LM(void)
{
	Left_Wheel(1);
	if(count < 4)
	{
		PDC1 = 12500;
	}
	else if(count >= 4 && count <= 16)
	{
		PDC1 = 20000;
	}
	else
	{
		PDC1 = 0;
	}
}	
					
void D1_T3_RM(void)
{
	Right_Wheel(1);
	if(count < 8)
	{
		PDC3 = PDC3 + 1;
	}
	else if(count >= 8 && count < 16)
	{
		PDC3 = PDC3 - 1;
	}	
	else
	{
		PDC3 = 0;
	}	
}	

void Right_Wheel(int A)
{
	if(A == 1) // Move forward
	{
		M3 = 1;
		M4 = 0;
	}
	else if(A == -1) //Move reverse
	{
		M3 = 0;
		M4 = 1;
	}
	else if(A == 0) //Stop
	{
		M3 = 0;
		M4 = 0;
	}
}

void Left_Wheel(int A)
{
	if(A == 1) // Move forward
	{
		M1 = 0;		
		M2 = 1;
	}
	else if(A == -1) //Move reverse
	{
		M1 = 1;
		M2 = 0;
	}
	else if(A == 0) //Stop
	{
		M1 = 0;
		M2 = 0;
	}
}	
