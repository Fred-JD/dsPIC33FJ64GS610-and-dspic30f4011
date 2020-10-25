#include <p30f4011.h>

//input
#define PB1     PORTBbits.RB0  //Pin 2
#define PB2     PORTBbits.RB1  //Pin 3

//Sensor 

#define Sens1   PORTDbits.RD1  //pin 18
#define Sens2   PORTDbits.RD2  //pin 22 
#define Sens3   PORTEbits.RE8  //pin 17
#define Sens4   PORTCbits.RC14 //pin 16

//motor 
#define M1 		LATEbits.LATE1  //pin 37 //Motor near switch
#define M2		LATEbits.LATE2  //pin 36 //Motor near switch
#define M3		LATEbits.LATE5  //pin 33
#define M4		LATFbits.LATF6  //pin 24
//#define M5	
//#define M6

//Controller
#define c1 		PORTBbits.RB5  //pin 7
#define c2		PORTBbits.RB6  //pin 8
#define c3 		PORTBbits.RB7  //pin 9
#define c4		PORTBbits.RB8  //pin 10

//Output
#define LED1  LATBbits.LATB4 //Pin 6
#define LED2  LATDbits.LATD0 //Pin 23


void TRIS(void);
void PWM_Setting(void);
void Timer1(void);
void Timer2(void);
void Timer3(void);
void Timer4(void);
void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void);
void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void);
void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt(void);
void __attribute__((__interrupt__, __auto_psv__)) _T4Interrupt(void);
void D1_T1_L2(int a);
void D1_T2(void);
void D1_T2_Led(void);
void D1_T3_LM(void);
void D1_T3_RM(void);
void Left_Wheel(int A);
void Right_Wheel(int A);