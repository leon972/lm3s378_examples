//*****************************************************************************
//  led.c - Gestione del led genral purpose sulla scheda 
//  di valutazione EKK-LM3S3748 (implementazione)
//  Code by Leonardo Berti (c) 2012
//
//*****************************************************************************


#include "led.h"
#include "inc/hw_types.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "gpio.h"
#include "hw_memmap.h"

//periferica a cui è collegato il led (porta F)
#define LED_PERIPHERAL SYSCTL_PERIPH_GPIOF
//porta alla quale è collegato il led
#define LED_PORT GPIO_PORTF_BASE
//pin della porta a cui è collegato il led
#define LED_PIN GPIO_PIN_0

#define DELAY_VALUE 10000

/**
   Ciclo di attesa
*/
void wait(unsigned long delay)
{
   unsigned long ulLoop;
   unsigned long totDelay=delay*DELAY_VALUE;

   for(ulLoop = 0; ulLoop < totDelay; ulLoop++)
   {
      //ciclo delay
   }
}

/**
 Abilita/disabilita il led
*/
void enableLed(unsigned long enabled)
{									
    if (enabled) {	  	  											  
	    //il led è collegato al pin 0 della portaf
		//abilita la porta F che contiene il pin per il led della scheda
	
		//abilita il clocking della porta F che ha il segnale del led sul pin 0
		//(tutte le periferiche sono disabilitate inizialmente)
	    SysCtlPeripheralEnable(LED_PERIPHERAL);
	    
		//Setta il pin come I/O digitale GPIO (altre modalità sono OD=open drain, I2C ecc...)
		ROM_GPIOPinTypeGPIOOutput(LED_PORT,LED_PIN);    
	
		//Imposta la "direzione del pin", cioè specifica se è un pin di output o input
		//in questo caso è un output
		GPIODirModeSet(LED_PORT,LED_PIN,GPIO_DIR_MODE_OUT);		

	}
	else
	{												    
		SysCtlPeripheralDisable(LED_PERIPHERAL);
	}
}

/**
  Accende/Spenge il led principale della scheda
*/
void switchLed(unsigned long switch_mode)
{
    if (switch_mode==SWITCH_ON) {

 	   //modo alternativo:usa le difinizioni dei registri in inc/lm3s3748.h
	   //GPIO_PORTF_DATA_R |= 0x01;

	   //setta in stato alto il pin 0 della porta F collegato al LED (accende il led)
	   ROM_GPIOPinWrite(LED_PORT,LED_PIN,0x1);
    }
	else 
	{
	   //resetta il pin 0 lasciando inalterati gli altri pin
	   //GPIO_PORTF_DATA_R &= ~(0x01);

	   //setta in stato basso il pin collegato al led (lascia inalterati gli altri pin)
	   //il MCU permette di settare i singoli pin via hardware (bitbanding)
	   //in modo da evitare il read-modify-write del valore della porta
	   ROM_GPIOPinWrite(LED_PORT,LED_PIN,0x0);
	}
}

/**
  Accende/spenge il led principale e attende
*/
void blinkSequence(unsigned long onDelay,unsigned long offDelay,unsigned long cycles)
{
   unsigned long i;
   //stato corrente del led
   int state=isLedOn();	

   for (i=0;i<cycles;i++)
   {
      state=!state;
      
	  switchLed(state);

	  wait(state==SWITCH_ON ? onDelay:offDelay);
	  
	  state=!state;
      
	  switchLed(state);

	  wait(state==SWITCH_ON ? onDelay:offDelay);	  												               
   }
}

/**
  Determina lo stato attuale del led principale
*/
int isLedOn(void)						  
{
  return (ROM_GPIOPinRead(LED_PORT,LED_PIN)& 0x1);
}

