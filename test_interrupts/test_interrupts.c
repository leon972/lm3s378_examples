//*****************************************************************************
//  test_interrupts.c
//  Applicazione di test degli interrupts
//  sulla scheda di valutazione EKK-LM3S3748.
//  Viene attivato l'interrupt sul pin zero della porta D
//  il programma attende 2 secondi, poi accende il led di stato,
//  attende altri due secondi e genera l'interrupt sul pin 0 porta d
//  mettendo il pin in stato alto.A questo punto viene invocata la ISR
//  ISRPortD che scrive un messaggio di debug sul display. 
// 
//
//	Code by Leonardo Berti (c) 2012
//
//*****************************************************************************

#include "cmdefs.h"
#include "lcddebug.h"  //Debug sul display LCD
#include "mcstring.h"  //minilib string
#include "mcbuff.h"	   //minilib string builder
#include "led.h"       //onboard status led
//#include "timers_util.h" //utilità gestione timers
#include "btu_select.h" //gestione del tasto select sulla scheda
#include "systick_util.h" //systick util (funzione wait)

#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_ints.h"
#include "hw_nvic.h"   //nested vectored interrupt controller
#include "sysctl.h"    //costanti system control
#include "rom.h"	   //funzioni ROM
#include "gpio.h"

//Stellaris Driverlib
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "test_interrupts.h"

#define INT_PIN_PERIPHERAL SYSCTL_PERIPH_GPIOD
#define INT_TEST_PORT GPIO_PORTD_BASE
#define INT_TEST_PIN GPIO_PIN_0
#define PORT_INT INT_GPIOD

/**
Handler interrupt porta D
Questo handler è mappato nel vettore interrupt (vedere file startup.s)
*/
void ISRPortD(void)
{ 
   //resetta il flag di interrupt
   ROM_GPIOPinIntClear(INT_TEST_PORT,INT_TEST_PIN);	

   LCD_writeTextToRow(2,"INT OK",TRUE);

   LCD_Flush();

}

void enablePinInterrupt(void)
{

	//setta la sorgente di interrupt
	ROM_GPIOIntTypeSet(INT_TEST_PORT,INT_TEST_PIN,GPIO_RISING_EDGE); 		
	  				   
	ROM_GPIOPinIntEnable(INT_TEST_PORT,INT_TEST_PIN);	 
														   
    ROM_IntEnable(PORT_INT);
	   
	//abilita la gestione degli interrupt nella CPU
	IntMasterEnable();	 
}

void configureIntPin()
{
    SysCtlPeripheralEnable(INT_PIN_PERIPHERAL);
	    
	//Setta il pin come I/O digitale GPIO (altre modalità sono OD=open drain, I2C ecc...)
	ROM_GPIOPinTypeGPIOOutput(INT_TEST_PORT,INT_TEST_PIN);  
	  
	
	//Imposta la "direzione del pin", cioè specifica se è un pin di output o input
	//in questo caso è un output
	GPIODirModeSet(INT_TEST_PORT,INT_TEST_PIN,GPIO_DIR_MODE_OUT);	
}

void digitalPinWrite(unsigned long value)
{
     ROM_GPIOPinWrite(INT_TEST_PORT,INT_TEST_PIN,value ? HIGH : LOW);
}

int main(void)
{   
   //setta il clocking del sistema
   ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_8MHZ);

   LCD_InitDisplay("Test Interrupts",ClrWhite,ClrDarkBlue);

   //configura il pin
   configureIntPin();
//
   digitalPinWrite(LOW);
//
//   //abilita l'interrupt sulla porta del pin (gestito dalla ISR ISRPortD)
   enablePinInterrupt();

   enableLed(TRUE);

   waitSec(2);

   switchLed(SWITCH_ON);   

   waitSec(2);

   digitalPinWrite(HIGH);
						   
   while(1)
   {
      //esecuzione infinita	  
   }
}

