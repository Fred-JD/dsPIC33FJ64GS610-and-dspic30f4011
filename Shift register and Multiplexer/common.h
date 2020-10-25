#include <p33fj64gs610.h>

#define PB1    	PORTEbits.RE8		//pin 18
#define PB2		PORTEbits.RE9		//pin 19

#define LED1	LATAbits.LATA6		//pin 91
#define LED2	LATAbits.LATA7		//pin 92
#define LED3 	LATEbits.LATE0 		//pin 93


//My Side
#define L1		LATDbits.LATD7
#define L2		LATEbits.LATE1
#define L3		LATGbits.LATG14
#define L4		LATGbits.LATG12

/*
#define L1		LATEbits.LATE3
#define L2		LATEbits.LATE5
#define L3		LATEbits.LATE7
#define L4		LATGbits.LATG15
*/


#define DATA_595 	LATEbits.LATE4
#define STROBE_595 	LATEbits.LATE2
#define CLK_595 	LATGbits.LATG13


#define S0 		LATFbits.LATF12
#define S1 		LATFbits.LATF13
#define S2		LATAbits.LATA1

#define y 		PORTDbits.RD14


/*
#define DATA_595 	PORTEbits.RE4
#define STROBE_595 	PORTEbits.RE2
#define CLK_595 	PORTGbits.RG13
*/

void TRIS_init(void);
void clock(void);
void strobe(void);
void data_submit(unsigned int data);
void Timer1(void);
void delay_500us(void);
void delay_200ms(void);
void Task(void);
void RecievedData(void);
void Init_UART(void);
void WriteUART1(unsigned int data);
void WriteStringUART1(const char * s);
void WriteUART1dec2string(unsigned int data);
void Mux(void);


//		data_submit(0b00000000);
//		data_submit(0b01000000); //L2
//		data_submit(0b10000000); //L3
//
//		data_submit(0b00100000); //L6
//		data_submit(0b00010000); //L5
//		
//		data_submit(0b00001000); //L9
//		data_submit(0b00000100);  //L8
//		
//		data_submit(0b00000010); //L12
//		data_submit(0b00000001); //L11