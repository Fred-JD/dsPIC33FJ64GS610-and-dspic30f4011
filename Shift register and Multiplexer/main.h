#include "common.h"

int count_us = 0;
int count_ms = 0;
int count_Led = 0;
int count_us_c = 0;
int count_us_s = 0;
int memory = 0;
int flag=0;
char ReceivedChar;

void TRIS_init(void){
	ADPCFG  = 0xffff;
	ADPCFG2 = 0xffff;
    TRISE = 0xffff;
    TRISF = 0;
    TRISG = 0;  
    TRISA = 0;
    TRISD = 0xffff;
}

/*
 *This function will enable the Clock.
 */
void clock(void){
    CLK_595 = 1;
    while(count_us_c < 50);
    count_us_c = 0;
    CLK_595 = 0;
    while(count_us_c < 50);
    count_us_c = 0;
}

/*
 *This function will strobe and enable the output trigger.
 */
void strobe(void){
    STROBE_595 = 1;
  	while(count_us_s < 50);
  	count_us_s = 0;
    STROBE_595 = 0;
} 

void data_submit(unsigned int data){
	int i;
    for (i=0 ; i<8 ; i++){
        DATA_595 = (data >> i) & 0x01;
        clock();
    }
    strobe(); // Data finally submitted 
} 

void Timer1(void)
{
	T1CONbits.TON = 0;// Disable Timer
	T1CONbits.TCS = 0;// Select internal instruction cycle clock 
	T1CONbits.TGATE = 0;// Disable Gated Timer mode
	T1CONbits.TCKPS = 0;// Select 1:1 Prescaler
	TMR1 = 0; // Clear timer registerPR1 = 9; // Load the period value

	PR1 = 368.5; //100us
	
	
	IPC0bits.T1IP = 1;// Set Timer1 Interrupt Priority Level
	IFS0bits.T1IF = 0;// Clear Timer1 Interrupt Flag
	IEC0bits.T1IE = 1;// Enable Timer1 interrupt
	
	T1CONbits.TON = 1;
}

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
	count_us++;
	count_ms++;
	count_Led++;
	count_us_c++;
	count_us_s++;	
	
	IFS0bits.T1IF = 0; // Clear Timer1 Interrupt Flag
}	

void LED1_Blink(void)
{
	L1 = 1;
	while(count_ms < 5000);
	count_ms = 0;
	L1 = 0;
}

void LED2_Blink(void)
{
	L2 = 1;
	while(count_ms < 5000);
	count_ms = 0;
	L2 = 0;
}		

void LED3_Blink(void)
{
	L3 = 1;
	while(count_ms < 5000);
	count_ms = 0;
	L3 = 0;
}	

void LED4_Blink(void)
{
	L4 = 1;
	while(count_ms < 5000);
	count_ms = 0;
	L4 = 0;
}	

void clear_led(void)
{
	data_submit(0b00000000);

	count_ms = 0;
}

// Shift Register Task
void Task(void)
{	
	data_submit(0b00000000);
	while(count_ms < 5000);
	count_ms = 0;
	
	LED1_Blink();
	
	data_submit(0b01000000);
	while(count_ms < 5000);
	count_ms = 0;

	data_submit(0b10000000);
	while(count_ms < 5000);
	count_ms = 0;
	
	clear_led();
	
	LED2_Blink();
	
	data_submit(0b00010000);
	while(count_ms < 5000);
	count_ms = 0;
	
	data_submit(0b00100000);
	while(count_ms < 5000);
	count_ms = 0;
	
	clear_led();
	LED3_Blink();
	
	data_submit(0b00000100);
	while(count_ms < 5000);
	count_ms = 0;
	
	data_submit(0b00001000);
	while(count_ms < 5000);
	count_ms = 0;
	
	clear_led();
	LED4_Blink();
	
	data_submit(0b00000001);
	while(count_ms < 5000);
	count_ms = 0;
	
	data_submit(0b00000010);
	while(count_ms < 5000);
	count_ms = 0;
	
	//Clear 
	data_submit(0b00000000);
	while(count_ms < 5000);
	count_ms = 0;
	
	data_submit(0b00000010);
	while(count_ms < 5000);
	count_ms = 0;
	
	data_submit(0b00000001);
	while(count_ms < 5000);
	count_ms = 0;
	
	clear_led();
	LED4_Blink();
	
	data_submit(0b00001000);
	while(count_ms < 5000);
	count_ms = 0;
					
	data_submit(0b00000100);
	while(count_ms < 5000);
	count_ms = 0;
	
	clear_led();
	LED3_Blink();
	
	data_submit(0b00100000);
	while(count_ms < 5000);
	count_ms = 0;
	
	data_submit(0b00010000);
	while(count_ms < 5000);
	count_ms = 0;	
	
	clear_led();
	LED2_Blink();	
	
	data_submit(0b10000000);
	while(count_ms < 5000);
	count_ms = 0;	
	
	data_submit(0b01000000);
	while(count_ms < 5000);
	count_ms = 0;	
	
	clear_led();
	LED1_Blink();													
}

void Init_UART(void)
{
	
	U1BRG = 23; // BAUD Rate Setting for 9600
	U1MODEbits.UARTEN = 1; // Enable UART	
	U1STAbits.UTXEN = 1; // Enable UART TX

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

void RecievedData(void)
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

// Multiplexer
void Mux(void)
{
	if(!S0 && !S1 && !S2)
	{
	//26
		if(y == 0 && flag == 0)
		{
			flag = 1;
			memory = memory + 26;
			WriteUART1dec2string(memory);
			WriteStringUART1("\r\n");
		}	
		else if(y == 1 && flag == 1)
		{
			flag = 0;
		}	
	}	
	else if(!S0 && !S1 && S2)
	{
	//27
		if(y == 0 && flag == 0)
		{
			flag = 1;
			memory = memory + 27;
			WriteUART1dec2string(memory);
			WriteStringUART1("\r\n");
		}	
		else if(y == 1 && flag == 1)
		{
			flag = 0;
		}
	}
	else if(!S0 && S1 && !S2) //010
	{
	//28
		if(y == 0 && flag == 0)
		{
			flag = 1;
			memory = memory + 28;
			WriteUART1dec2string(memory);
			WriteStringUART1("\r\n");
		}	
		else if(y == 1 && flag == 1)
		{
			flag = 0;
		}
	}
	else if(!S0 && S1 && S2) //011
	{
	//29
		if(y == 0 && flag == 0)
		{
			flag = 1;
			memory = memory + 29;
			WriteUART1dec2string(memory);
			WriteStringUART1("\r\n");
		}	
		else if(y == 1 && flag == 1)
		{
			flag = 0;
		}
	}
	else if(S0 && !S1 && !S2) //100
	{
	//30
		if(y == 0 && flag == 0)
		{
			flag = 1;
			memory = memory + 30;
			WriteUART1dec2string(memory);
			WriteStringUART1("\r\n");
		}	
		else if(y == 1 && flag == 1)
		{
			flag = 0;
		}
	}
	else if(S0 && !S1 && S2)//101
	{
	//31
		if(y == 0 && flag == 0)
		{
			flag = 1;
			memory = memory + 31;
			WriteUART1dec2string(memory);
			WriteStringUART1("\r\n");
		}	
		else if(y == 1 && flag == 1)
		{
			flag = 0;
		}
	}
	else if(S0 && !S1 && S2)//110
	{
	//32
		if(y == 0 && flag == 0)
		{
			flag = 1;
			memory = memory + 32;
			WriteUART1dec2string(memory);
			WriteStringUART1("\r\n");
		}	
		else if(y == 1 && flag == 1)
		{
			flag = 0;
		}
	}
	else if(S0 && !S1 && S2)//111
	{
	//33
		if(y == 0 && flag == 0)
		{
			flag = 1;
			memory = memory + 33;
			WriteUART1dec2string(memory);
			WriteStringUART1("\r\n");
		}	
		else if(y == 1 && flag == 1)
		{
			flag = 0;
		}
	}
}	

void change_pin(int n)
{
	switch (n)
	{
		case 0 :
			S0 = 0;
			S1 = 0;
			S2 = 0;
			break;
			
		case 1 :
			S0 = 0;
			S1 = 0;
			S2 = 1;
			break;
	
		case 2 :
			S0 = 0;
			S1 = 1;
			S2 = 0;
			break;
			
		case 3:
			S0 = 0;
			S1 = 1;
			S2 = 1;
			break;
			
		case 4 :
			S0 = 1;
			S1 = 0;
			S2 = 0;
			break;
			
		case 5 :
			S0 = 1;
			S1 = 0;
			S2 = 1;
			break;
			
		case 6 :
			S0 = 1;
			S1 = 1;
			S2 = 0;
			break;
			
		case 7 :
			S0 = 1;
			S1 = 1;
			S2 = 1;
			break;
			
		default:
			S0 = 1;
			S1 = 1;
			S2 = 1;
	}	
}	
