#include "main.h"

char ReceivedChar;
int count_L0=0,count_L1=0,count_L2=0, count=0;
/*
const char Text1 = "Submit your task on time.\r\n";
const char Text2 = "Put your shoe on rak.\r\n";
const char Text3 = "Sharif is handsome!!!?\r\n";
*/
void Init_TRIS(void)
{
	ADPCFG  = 0xffff;
	ADPCFG2 = 0xffff;
	TRISA = 0x0000;
	TRISE = 0b111111111111110;
}	

void Timer1(void)
{
	T1CONbits.TON = 0;// Disable Timer
	T1CONbits.TCS = 0;// Select internal instruction cycle clock 
	T1CONbits.TGATE = 0;// Disable Gated Timer mode
	T1CONbits.TCKPS = 3;// Select 1:256 Prescaler
	TMR1 = 0; // Clear timer registerPR1 = 9; // Load the period value
	PR1 = 288; //20ms
	
	
	IPC0bits.T1IP = 1;// Set Timer1 Interrupt Priority Level
	IFS0bits.T1IF = 0;// Clear Timer1 Interrupt Flag
	IEC0bits.T1IE = 1;// Enable Timer1 interrupt
	
	T1CONbits.TON = 1;
}	

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
	/* Interrupt Service Routine code goes here */
	if(LED1) LED1=0; else LED1=1;
	Task_3();
	
	IFS0bits.T1IF = 0; // Clear Timer1 Interrupt Flag
}

void Timer2(void)
{
	T2CONbits.TON = 0;// Disable Timer
	T2CONbits.TCS = 0;// Select internal instruction cycle clock 
	T2CONbits.TGATE = 0;// Disable Gated Timer mode
	T2CONbits.TCKPS = 3;// Select 1:256 Prescaler
	TMR2 = 0; // Clear timer registerPR1 = 9; // Load the period value
	PR2 = 9212.5;
	
	_T2IP = 2;// Set Timer1 Interrupt Priority Level
	_T2IF = 0;// Clear Timer1 Interrupt Flag
	_T2IE = 1;// Enable Timer1 interrupt
	
	T2CONbits.TON = 1;
}

void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void)
{
	/* Interrupt Service Routine code goes here */
	Task_1();
	
	_T2IF = 0; // Clear Timer1 Interrupt Flag
}

void Push_Button(void)
{
	int flag1,flag2;
	if(PB1 == 0 && flag1 == 0)
	{
		flag1 = 1;
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

void Task_1(void)
{
	count_L0++;
	if(count_L0 == 2)
	{
		count_L0 = 0;
		count_L1++;
		if (LED2) LED2 = 0; else LED2 = 1;
		if(count_L1 == 1)
		{
			count_L1 = 0;
			count_L2++;
			PR2 = 18425/((2*count_L2)/2);
			if(PR2 < 144)
			{
				PR2 = 144;
			}	
		}
	}
}

void Init_UART(void)
{
	
	U1BRG = 23; // BAUD Rate Setting for 9600
	U1MODEbits.UARTEN = 1; // Enable UART	
	U1STAbits.UTXEN = 1; // Enable UART TX

}		



void Init_PWM(void)
{
	PTCON = 0b1000000000000011;
	IOCON1bits.PENL = 1;
	PTPER = 100;
	PDC1 = 0;
}	



void WriteUART1(unsigned int data)
{
    while (U1STAbits.TRMT==0);
    if(U1MODEbits.PDSEL == 3)
        U1TXREG = data;
    else
        U1TXREG = data & 0xFF;
}

void WriteStringUART1(const char * s)
{
    while(*s)
            WriteUART1(*s++);
}

void RecievedDatat(void)
{
	if(U1STAbits.OERR == 1) 
	{
		U1STAbits.OERR = 0; // Clear Overrun Error to receive data
	} 
	else if ((U1STAbits.FERR ==0) && (U1STAbits.PERR ==0)) 
	{
		ReceivedChar = U1RXREG; // Read Data if there is no parity or framin
	}
}	

void Task_3(void)
{
	//PWM increse from 0 - 100 then decrese from 100 - 0
	count++;
	if(count < 100)
	{
		PDC1 = PDC1 + 1;		
	}	
	else if(count >= 100 && count < 200)
	{
		PDC1 = PDC1 - 1;
	}	
	else if(count == 200)
	{
		count = 0;
	}	
}	
int main()
{
	int flag1 = 0, flag2 = 0;
	Init_TRIS();
	Init_UART();
	Init_PWM();
	Timer1();
	Timer2();
	count_L0=0,count_L1=0,count_L2=0;
	
	while(1)
	{
		LED3 = 1;

		RecievedDatat();

		if(ReceivedChar == 'A')
		{
			LED3 = 0;
			WriteStringUART1("Put your shoe on the rack\r\n");
		}	
		else if(ReceivedChar == 'B')
		{
			LED3 = 0;
			WriteStringUART1("Sharif is handsome!!!?\r\n");
		}
		if(PB1 == 0 && flag1 == 0)
		{
			flag1 = 1;
			WriteStringUART1("Submit your task on time.\r\n");
		}
		else if(PB1 == 1 && flag1 == 1)
		{
			flag1 = 0;
		}		
		if(PB2 == 0 && flag2 == 0)
		{
			flag2 = 1;
			//if(LED2) LED2=0; else LED2=1;
			WriteStringUART1("Submit your task on time.\r\n");
		}
		else if(PB2 == 1 && flag2 == 1)
		{
			flag2 = 0;
		}
	}	
}

