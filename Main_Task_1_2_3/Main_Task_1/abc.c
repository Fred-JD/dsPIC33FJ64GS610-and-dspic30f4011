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
	