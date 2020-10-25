#include "common.h"


//===========================================================================================//
//                        				Variable                                             //
//===========================================================================================//
int count = 0,count_Led = 0; 		//count for every 0.5sec
int mot_dir = 0;
char buf[10];
int data0, data1, data2, data3;
unsigned int ms_counter;
char ReceivedChar;

//===========================================================================================//
//                        				Function                                          	 //
//===========================================================================================//
void TRIS(void)
{
	ADPCFG = 0x1F;
	//ADPCFG = 0xffff;
	
	TRISB = 0b111100011;
	TRISC = 0b0100000000000000;
	TRISD = 0b0110;
	TRISE = 0b000000100000000;
	TRISF = 0b0;

	LED2  = 1; //Active Low
}
//===========================================================================================//
//                                     Motor                                                 //
//===========================================================================================//
void PWM_Setting(void)
{

	PTCON   = 0b1000000000000011;
	PWMCON1 = 0b0000111100000101;
	PDC1 = 	0; 			//Left
	PDC3 = 	0;			//Right
	PTPER = 19999;   			//10M Hz
	OVDCON = 0b0001000100010001;
}

void Right_Wheel(int A)
{
	if(A == 1) // Move forward
	{
		M3 = 1;
		M4 = 0;
	}
	else if(A == 0) //Move reverse
	{
		M3 = 0;
		M4 = 1;
	}
	else if(A == 3) //Stop
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
	else if(A == 0) //Move reverse
	{
		M1 = 1;
		M2 = 0;
	}
	else if(A == 3) //Stop
	{
		M1 = 0;
		M2 = 0;
	}
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
	// Write UART for analog sensor value
	
	T2CON = 0;            // Clear the Timer 1 configuration
	TMR2 = 0;             // Reset Timer 1 counter
	PR2 =23437.5 ;          // Set the Timer 1 period (max 65535)
	T2CONbits.TCS = 0;    // Select internal clock (Fosc/4)
	T2CONbits.TCKPS = 3;  // Prescaler (0=1:1, 1=1:8, 2=1:64, 3=1:256)
	_T2IP = 1;            // Set the Timer 1 interrupt priority
	_T2IF = 0;            // Clear the Timer 1 interrupt flag
	_T2IE = 1;            // Enable Timer 1 interrupt
	T2CONbits.TON =0;    // Turn on Timer 1
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
	T3CONbits.TON = 1;    // Turn off Timer 3
}

void Timer4(void)            
{
	
	T4CON = 0;            // Clear the Timer 1 configuration
	TMR4 = 0;             // Reset Timer 1 counter
	PR4 =58593.75;          // Set the Timer 1 period (max 65535)
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
	WriteStringUART1("               ");
	WriteUART1dec2string(data0);
	WriteStringUART1("               ");
	WriteUART1dec2string(data1);
	WriteStringUART1("               ");
	WriteUART1dec2string(data2);
	WriteStringUART1("               ");
	WriteUART1dec2string(data3);
	WriteStringUART1("\r\n");
}   

void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt(void)
{
	_T3IF = 0;
	
	Day3_Task2();	
}   

void __attribute__((__interrupt__, __auto_psv__)) _T4Interrupt(void)
{
	_T4IF = 0;


} 

//===========================================================================================//
//                       				UART                                                 //
//===========================================================================================//

void UART_Set(void)
{
	// Set up UART
	// Default is 8 data bits, 1 stop bit, no parity bit
	U1BRG = 48;            						// 38400 baud @ 30 MIPS
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


void WriteUART1(unsigned int data)
{
    while (U1STAbits.TRMT==0);
    if(U1MODEbits.PDSEL == 3)
        U1TXREG = data;
    else
        U1TXREG = data & 0xFF;
}

void WriteUART1dec2string(unsigned int data)
{
    unsigned char temp;
    temp=data/1000;
    WriteUART1(temp+'0');
    data=data-temp*1000;
    temp=data/100;
    WriteUART1(temp+'0');
    data=data-temp*100;
    temp=data/10;
    WriteUART1(temp+'0');
    data=data-temp*10;
    WriteUART1(data+'0');
}

void WriteStringUART1(const char * s)
{
    while(*s)
            WriteUART1(*s++);
}

//===========================================================================================//
//                       	 	           	ADC                                             //
//==========================================================================================//

void InitADC(void)
{
	ADCON1bits.ADSIDL=0;
    ADCON1bits.FORM=0;
    ADCON1bits.SSRC=7;
    ADCON1bits.SAMP=1;
    
    ADCON2bits.VCFG=7;
    ADCON2bits.CSCNA=1;
    ADCON2bits.SMPI=3;  // open for 3 input 
    ADCON2bits.BUFM=0;
    ADCON2bits.ALTS=0;
    
    ADCON3bits.SAMC=31;
    ADCON3bits.ADRC=1;
    ADCON3bits.ADCS=31;
    
    ADCHSbits.CH0NB=0;
    ADCHSbits.CH0NA=0;
    ADCHSbits.CH0SA=0;
    ADCHSbits.CH0SB=0;
    
    ADCSSL=0b0000000111100000;
    
    ADCON1bits.ASAM=1;
    IFS0bits.ADIF=1;
    IEC0bits.ADIE=1;

}

void __attribute__((__interrupt__)) _ADCInterrupt(void) {
    data0 = ADCBUF0;
    data1 = ADCBUF1; 
    data2 = ADCBUF2; 
    data3 = ADCBUF3; 
   	
   	Day3_Task1();
    IFS0bits.ADIF = 0;  //After conversion ADIF is set to 1 and must be cleared
}

//===========================================================================================//
//                       	 		DAY 3 TASK 1                                             //
//===========================================================================================//
void Day3_Task1(void)
{
	Left_Wheel(mot_dir);
	Right_Wheel(mot_dir);
	PDC1 = (data0 + data1)*10;
	PDC3 = (data2 + data3)*10;
}	

void Day3_Task2(void)
{
	if(data2 > 700)
	{
		count++;
	}	
	else
	{
		count = 0;
	}	

}	

void Day3_Task3(void)
{
	if(count == 6)
	{
		count = 0;
		if (mot_dir) 
		{
			mot_dir = 0; 
		}	
		else 
		{
			mot_dir = 1;
		}
	}
}	
