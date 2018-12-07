#include <lpc214x.h>
#include <stdio.h>
#include "lib_funcs.h"

//Retarget printf to divert output to UART0
//visit http://www.ocfreaks.com/retarget-redirect-printf-scanf-uart-keil/ for complete printf retargetting tutorial
/*struct __FILE
{
	int dummyVar; //Just for the sake of redefining __FILE, we won't we using it anyways ;)
};

FILE __stdout; //STDOUT
FILE __stdin;  //STDIN*/
/* void uartWrite(char c)
{
	while (!(U0LSR & THRE)); //wait till the THR is empty
	if( c == '\n' ) //Send <CR+LF>
	{
		U0THR = CARRIAGE_RETURN;
		while(!(U0LSR & THRE)); 
		U0THR = LINE_FEED;
	}
	else
	{
		U0THR = c; 
	}
}

char uartRead(void)
{
	while(!(U0LSR & RDR)); // wait till any data arrives
	return U0RBR;
}


void print_int(unsigned int integer)
{
   char digit[3];
   unsigned int temp = 0;
   
   digit[0] = integer/100;   //first digit
   
   temp = integer % 100;     //temp = the remainder after dividing by 100
   
   digit[1] = temp/10;       //the second digit
   
   digit[2] = temp % 10;     //third digit
   
   //change digits in to ASCII and print (add 0x30 or 48 to change to ASCII)
   for(int i=0; i<3; i++)
   {
      uartWrite(digit[i]+48);
   }
}
*/

void initTimer0(void)
{
	//Assuming PCLK = 48Mhz
	T0CTCR = 0x0;
	T0PR = 48-1; //48 clock cycles @48Mhz = 1 us
	T0TCR = 0x01; //enable Timer
}

void delayUS(unsigned long microseconds) //Using Timer0
{
	T0TCR = 0x02; //Reset Timer
	T0TCR = 0x01; //Enable timer
	while(T0TC < microseconds); //wait until timer counter reaches the desired delay
	T0TCR = 0x00; //Disable timer
}
void delayMS(unsigned int milliseconds)
{
	delayUS(milliseconds * 1000);
}


//---------PLL Related Functions :---------------

//Using PLL settings as shown in : http://www.ocfreaks.com/lpc214x-pll-tutorial-for-cpu-and-peripheral-clock/

void setupPLL0(void)
{
	//Note : Assuming 12Mhz Xtal is connected to LPC2148.
	
	//PLL0CON = 0x01; 
	PLL0CFG = 0x13; 
}

void feedSeq(void)
{
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;
}

void feedSeq2(void){
	
	PLL0CON=0x1;
	
	PLL0FEED = 0xAA;
	PLL0FEED = 0x55;
  
}

void connectPLL0(void)
{
	while( !(PLL0STAT & 0x0400));
	PLL0CON = 0x02;
}

void initClocks(void)
{
	setupPLL0();
	feedSeq(); //sequence for locking PLL to desired freq.
	feedSeq2();
	connectPLL0();
	feedSeq(); //sequence for connecting the PLL as system clock
   
	//SysClock is now ticking @ 60Mhz!
       
	VPBDIV = 0x01; // PCLK is same as CCLK i.e 60Mhz

	//PLL0 Now configured!
}
