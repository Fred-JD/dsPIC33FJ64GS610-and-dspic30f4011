#include "common.h"


//===========================================================================================//
//                        				Variable                                             //
//===========================================================================================//
int count = 0,count_Led = 0; 		//count for every 0.5sec
char ReceivedChar;
const char name[] = "UTM Robocon 2019 \r\n";
const char Year[5] = "YEAR";
const char Month[6] = "MONTH";
const char Day[4] = "DAY";
const char Space[8] = "       ";
char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
int countA = 0;
int day=23, month=12, year=18;

//===========================================================================================//
//                        				Function                                          	 //
//===========================================================================================//
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


//===========================================================================================//
//                        TIMER                                                              //
//===========================================================================================//
void Timer1(void)
{
	// Configure Timer 1
	// Let LED blink in 200ms
	T1CON = 0;            // Clear the Timer 1 configuration
	TMR1 = 0;             // Reset Timer 1 counter
	PR1 = 23437.5 ;          // Set the Timer 1 period (max 65535)
	T1CONbits.TCS = 0;    // Select internal clock (Fosc/4)
	T1CONbits.TCKPS = 3;  // Prescaler (0=1:1, 1=1:8, 2=1:64, 3=1:256)
	_T1IP = 1;            // Set the Timer 1 interrupt priority
	_T1IF = 0;            // Clear the Timer 1 interrupt flag
	_T1IE = 1;            // Enable Timer 1 interrupt
	T1CONbits.TON = 0;    // Turn on Timer 1
}

void Timer2(void)
{
	// Configure Timer 1
	// In this example, I'm setting PR1 and TCKPS for 8Hz
	
	T2CON = 0;            // Clear the Timer 1 configuration
	TMR2 = 0;             // Reset Timer 1 counter
	PR2 =23437.5 ;          // Set the Timer 1 period (max 65535)
	T2CONbits.TCS = 0;    // Select internal clock (Fosc/4)
	T2CONbits.TCKPS = 3;  // Prescaler (0=1:1, 1=1:8, 2=1:64, 3=1:256)
	_T2IP = 1;            // Set the Timer 1 interrupt priority
	_T2IF = 0;            // Clear the Timer 1 interrupt flag
	_T2IE = 1;            // Enable Timer 1 interrupt
	T2CONbits.TON =1;    // Turn on Timer 1
	T2CONbits.T32 = 0;
}

void Timer3(void)
{
	// Configure Timer 3
	// Calculate 0.5 sec per count
	
	T3CON = 0;            // Clear the Timer 1 configuration
	TMR3 = 0;             // Reset Timer 1 counter
	PR3 = 58593.75;          // Set the Timer 1 period (max 65535)
	T3CONbits.TCS = 0;    // Select internal clock (Fosc/4)
	T3CONbits.TCKPS = 3;  // Prescaler (0=1:1, 1=1:8, 2=1:64, 3=1:256)
	_T3IP = 1;            // Set the Timer 1 interrupt priority
	_T3IF = 0;            // Clear the Timer 1 interrupt flag
	_T3IE = 1;            // Enable Timer 1 interrupt
	T3CONbits.TON = 0;    // Turn off Timer 1
}

void Timer4(void)            
{
	// Configure Timer 1
	// In this example, I'm setting PR1 and TCKPS for 8Hz
	
	T4CON = 0;            // Clear the Timer 1 configuration
	TMR4 = 0;             // Reset Timer 1 counter
	PR4 =100;          // Set the Timer 1 period (max 65535)
	T4CONbits.TCS = 0;    // Select internal clock (Fosc/4)
	T4CONbits.TCKPS = 3;  // Prescaler (0=1:1, 1=1:8, 2=1:64, 3=1:256)
	_T4IP = 1;            // Set the Timer 1 interrupt priority
	_T4IF = 0;            // Clear the Timer 1 interrupt flag
	_T4IE = 1;            // Enable Timer 1 interrupt
	T4CONbits.TON = 0;     // Turn off Timer 1
	T4CONbits.T32 = 0;		//Turn off 32bits
}

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void)
{
	_T1IF = 0;
	count_Led++;
	if (LED1) LED1 = 0; else LED1 = 1;
}   

void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void)
{
	_T2IF = 0;
	count_Led++;
	Task2_2();
}   

void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt(void)
{
	_T3IF = 0;
	
	count++;
}   

void __attribute__((__interrupt__, __auto_psv__)) _T4Interrupt(void)
{
	_T4IF = 0;

	Task3();
} 

//===========================================================================================//
//                       				UART                                                 //
//===========================================================================================//

void UART_Set(void)
{
	// Set up UART
	// Default is 8 data bits, 1 stop bit, no parity bit
	U1BRG = 194;            						// 38400 baud @ 30 MIPS
	U1MODEbits.ALTIO = 0;						// 1 = Communicate using U1ATX and U1ARX pins (alternative).
												// 0 = Communicate using U1TX and U1RX pins.
	U1STAbits.URXISEL = 0;          			// Interrupt flag bit is set for every character received.
	IPC2bits.U1RXIP   = 4;          			// UART1 Receive Interrupt Priority = 4.
	U1STAbits.OERR    = 0;	        			// Clear the Receive Overflow Flag.
	IFS0bits.U1RXIF   = 0;          			// Clear the UART1 Receive Interrupt flag.
	IEC0bits.U1RXIE   = 1;          			// Enable UART1 Receive Interrupt.
	U1MODEbits.UARTEN = 1;						// Enable UART1.
	U1STAbits.UTXEN = 1;           				// Enable UART1 Transmit.
}

void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
{
	if(U1STAbits.OERR == 1) 
	{
		U1STAbits.OERR = 0; // Clear Overrun Error to receive data
	} 
	else if ((U1STAbits.FERR ==0) && (U1STAbits.PERR ==0)) 
	{
		ReceivedChar = U1RXREG; // Read Data if there is no parity or framin
	}
	IFS0bits.U1RXIF = 0; // clear TX interrupt flag
}



//===========================================================================================//
//                       	 		DAY 2 TASK 1                                             //
//===========================================================================================//

void WriteStringUART1(const char * s)
{
    while(*s)
            WriteUART1(*s++);
}

void WriteUART1(unsigned int data)
{
    while (U1STAbits.TRMT==0);
    if(U1MODEbits.PDSEL == 3)
        U1TXREG = data;
    else
        U1TXREG = data & 0xFF;
}

void Task1(void)
{
	if(count == 2)
	{
		WriteStringUART1(name);
		T3CONbits.TON = 0;
		count = 0;
	}	
}


//===========================================================================================//
//                       	 		DAY 2 TASK 2                                             //
//===========================================================================================//	

void Task2(void)
{
	int z = ReceivedChar - 95;
	
	if(count_Led < z)
	{
		T1CONbits.TON = 1;
	}	
	else if(count_Led == z)
	{
		T1CONbits.TON = 0;
		count_Led = 0;
		ReceivedChar = 0;
	}	
}	


void Task2_2(void)
{
	
	//int countA = 0;
	//if(ReceivedChar == 'a')
	
		if(countA < 2*(ReceivedChar - 96))
		{
			if (LED1) LED1 = 0; else LED1 = 1;
			countA++;
		}	
		else
		{
			countA = 0;
			ReceivedChar = 0;
		}	
	
}	

//===========================================================================================//
//                       	 		DAY 2 TASK 3                                             //
//===========================================================================================//	

void Task3()
{
	day++;
	if(day==29 && month==2)
	{
		day = 1;
		month++;
	}
	else if(day==31 && (month==4 || month==6 || month==9 || month==11))
	{
		day = 1;
		month++;
	}
	else if(day==32 && (month==1 || month==3 || month==5 || month==7 || month==8 || month==10))
	{
		day = 1;
		month++;
	}	
	else if(day==32 && month==12)
	{
		day = 1;
		month = 1;
		year++;
	}
	
	WriteStringUART1("\r\n");
	U1TXREG = digits[2];
	U1TXREG = digits[0];
	if(year < 10)
	{
		U1TXREG = digits[0];U1TXREG = digits[year];	
	}	
	else if(year >= 10 && year < 20)
	{
		U1TXREG = digits[1];U1TXREG = digits[year - 10];
	}	
	else if(year >= 20 && year < 30)
	{
		U1TXREG = digits[2];U1TXREG = digits[year - 20];
	}	
	WriteStringUART1(Space);
	if(month < 10)
	{
		U1TXREG = digits[0];U1TXREG = digits[month];	
	}	
	else if(month == 11)
	{
		U1TXREG = digits[1];U1TXREG = digits[1];
	}
	else if(month == 10)
	{
		U1TXREG = digits[1];U1TXREG = digits[0];
	}
	else
	{
		U1TXREG = digits[1];U1TXREG = digits[2];
	}
	WriteStringUART1(Space);
	if(day < 10)
	{
		U1TXREG = digits[0];U1TXREG = digits[day];
	}
	else if(day >= 10 && day < 20)
	{
		U1TXREG = digits[1];U1TXREG = digits[day - 10];
	}
	else if(day >= 20 && day < 30)
	{
		U1TXREG = digits[2];U1TXREG = digits[day - 20];
	}
	else if(day == 30)
	{
		U1TXREG = digits[3];U1TXREG = digits[0];
	}
	else if(day == 31)
	{
		U1TXREG = digits[3];U1TXREG = digits[1];
	}	
}	