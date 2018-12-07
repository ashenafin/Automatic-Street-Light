/*(C) Umang Gajera- www.ocfreaks.com
More Embedded tutorials @ www.ocfreaks.com/cat/embedded/
LPC2148 LDR Interfacing Example 2 Source Code for KEIL ARM.
http://www.ocfreaks.com/interfacing-ldr-lpc2148/
Also see: http://www.ocfreaks.com/lpc2148-adc-programming-tutorial/
License: GPL.*/

#include <lpc214x.h>
#include "lib_funcs.h" //OCFreaks LPC214x Tutorials Library Header
#include "uart.h"
#include <stdio.h>
#define CUT_OFF 710 //Define your cut-off value here

int  volt=0;
 
void ADC_Init (void)
{
 PINSEL1 = 0x01000000 ; // P0.28, AD0.1
}
 
unsigned int ADC_GetAdcReading()
{
 unsigned int adcdata;
 AD0CR = 0x01200302 ; // Select AD0.3, Select clock for ADC, Start of conversion
 
 while(!((adcdata = AD0GDR) & 0x80000000)) // Check end of conversion (Done bit) and read result
 {
 }
 return((adcdata >> 6) & 0x3ff) ; // Return 10 bit result
}
 void delay(void){
 ;
 }
int main(void)
{ 
 unsigned int adc;
 
 initClocks(); //Set PCLK = CCLK = 48Mhz
	initTimer0(); //Init Timer for delay functions
 IO1DIR=0xffffffff;
    IO0DIR=0x00000000;
	  IO0DIR |=(1<<7);
    PINSEL0=0x0300;
    VPBDIV=0x02;
    lcd_init();
    show("ADC Value : ");
	 cmd(0xc0);
	show("volt:");
	
	IO0DIR |= (1<<5); //Select P0.5 as output 
 
 while(1) 
 { 
 adc = ADC_GetAdcReading(); 

	 cmd(0x8b);
        dat((adc/1000)+48);
        dat(((adc/100)%10)+48);
        dat(((adc/10)%10)+48);
        dat((adc%10)+48);
	 volt=adc*3.3/1023;
	
	 cmd(0xc7);
	 dat((adc*3.3/1023)+48);
	 show(".");
	 dat(((adc*33/1023-10*volt))+48);
	  show("V");
	 if(adc<CUT_OFF)
	 {

	 if(IO0PIN & (1<<15))
		{
			IO0SET = (1<<5); //LED ON
			
			//delayMS(170); //wait for some time before sampling again.		 
		}
		
	 else  {
   
     // delayMS(100); //wait for some time before sampling again.		 
			IO0CLR = (1<<5); //LED OFF
	 }
 }
		else  {
		
			IO0CLR = (1<<5); //LED OFF
		}
		delayUS(100); //wait for some time before sampling again.
	}
}
