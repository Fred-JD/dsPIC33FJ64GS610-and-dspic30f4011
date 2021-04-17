#include <p33fj64gs610.h>

#define PB1    	PORTEbits.RE8		//pin 18
#define PB2		PORTEbits.RE9		//pin 19

#define LED1	LATAbits.LATA6		//pin 91
#define LED2	LATAbits.LATA7		//pin 92
#define LED3 	LATEbits.LATE0 		//pin 93




void Init_TRIS(void);
void Timer1(void);
void Timer2(void);
void Task_1(void);
void Init_UART(void);
void Init_PWM(void);
void WriteUART1(unsigned int data);
void WriteStringUART1(const char * s);
void RecievedDatat(void);
void WriteUART1(unsigned int data);
void WriteStringUART1(const char * s);
void Task_3(void);







