#include "main.h"
#include "common.h"

int count_us = 0;
int count_ms = 0;
int count_Led = 0;
int count_us_c = 0;
int count_us_s = 0;

void TRIS_init(void){
	ADPCFG  = 0xffff;
	ADPCFG2 = 0xffff;
    TRISE = 0;
    TRISG = 0;  
    TRISA = 0;
}

/*
 *This function will enable the Clock.
 */
void clock(void){
    CLK_595 = 1;
    while(count_us_c < 5);
    count_us_c = 0;
    CLK_595 = 0;
    while(count_us_c < 5);
    count_us_c = 0;
}

/*
 *This function will strobe and enable the output trigger.
 */
void strobe(void){
    STROBE_595 = 1;
  	while(count_us_s < 5);
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
	/* Interrupt Service Routine code goes here */
	//if(LED1) LED1=0; else LED1=1;
	
	count_us++;
	count_ms++;
	count_Led++;
	count_us_c++;
	count_us_s++;
	/*
	int i;
	for(i = 0; i < 2000; i++)
	{
		if(LED2) LED2=0; else LED2=1;
	}*/
	if (count_Led == 400){
		count_Led = 0;
		if(LED2) LED2=0; else LED2=1;
		if(L1) L1=0; else L1=1;
	}	
	
	IFS0bits.T1IF = 0; // Clear Timer1 Interrupt Flag
}

void delay_500us(void)
{
	while(count_us < 5){
	}
	count_us = 0;
}

void delay_200ms(void)
{
	while(count_us < 2000){
	}
	count_ms = 0;	
}		

void Task(void)
{
	/*
	data_submit(0b00000000);
	data_submit(0b10000000);
	data_submit(0b01000000);
	data_submit(0b00100000);
	data_submit(0b00010000);
	data_submit(0b00001000);
	data_submit(0b00000100);
	data_submit(0b00000010);
	data_submit(0b00000001);
	data_submit(0xFF);  */   
	
	switch(count_ms)
	{
		case 2000:
			data_submit(0b00000000);
			break;
		case 4000:
			data_submit(0b10000000);
			break;
		case 6000:
			data_submit(0b01000000);
			break;			
		case 8000:
			data_submit(0b00100000);
			break;
		case 10000:
			data_submit(0b00010000);
			break;
		case 12000:
			data_submit(0b00001000);
			break;
		case 14000:
			data_submit(0b00000100);
			break;
		case 16000:
			data_submit(0b00000010);
			break;
		case 18000:
			data_submit(0b00000001);
			break;
		case 20000:
			data_submit(0xFF);	
			count_ms = 0;
			break;
	}																						
}

int main()
{
	TRIS_init();
	Timer1();
	while(1){
		if(count_ms == 2000)
		{
			count_ms = 0;
			if(LED1) LED1 = 0;
			else LED1 = 1;
		}	
		Task();
	
		L1 = 1;
		L2 = 1;
		L3 = 0;
		L4 = 0;
	}
}	