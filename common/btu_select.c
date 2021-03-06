//*****************************************************************************
//
// btu_select.h
// Gestione del tasto di selezione sulla scheda di sviluppo
// texas Instruments Stellaris EKK-LM3S3748
// Code by Leonardo Berti (c) 2012
//
//*****************************************************************************

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
				   


//-------------Sellaris DriverLib----- 
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"

#include "btu_select.h"

//il tasto select è sulla porta B , pin 7
#define BTU_SEL_PERIPHERAL SYSCTL_PERIPH_GPIOB
#define BTU_SEL_GPIO_BASE  GPIO_PORTB_BASE
#define BTU_SEL_PIN GPIO_PIN_7
//numero interrupt associato alla porta B
#define BTU_PORT_INTERRUPT INT_GPIOB 

//======================== enableSelectButton =================================

/**
  Abilia il tasto di selezione
*/  
void enableSelectButton(void)
{
   //abilita la porta B sulla quale è collegato il tasto
   ROM_SysCtlPeripheralEnable(BTU_SEL_PERIPHERAL);
   //imposta il pin di selezione come pin di input
   ROM_GPIODirModeSet(BTU_SEL_GPIO_BASE, BTU_SEL_PIN, GPIO_DIR_MODE_IN);
   //configura l'amperaggio (drive strength a 2 mA) inoltre abilita la resistenza
   //di pull up (WPU=Weak Pull Up)
   ROM_GPIOPadConfigSet(BTU_SEL_GPIO_BASE, BTU_SEL_PIN,
                         GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}


//=========================== setButtonIterruptEnabled ========================

/**
 Abilita l'interrupt sul pin al quale è collegato il pulsante
*/
void setButtonIterruptEnabled(BOOL enabled)
{
	if (enabled)
	{
	   //configura il pin input
	   ROM_GPIOPinTypeGPIOInput(BTU_SEL_GPIO_BASE,BTU_SEL_PIN);
	   
	   //setta la sorgente di interrupt:usa il falling edge
	   //quando il segnale passa da alto a basso significa che il tasto è premuto
	   ROM_GPIOIntTypeSet(BTU_SEL_GPIO_BASE,BTU_SEL_PIN,GPIO_FALLING_EDGE); 		
	   																		 
	   //abilita la gestione dell'interrupt sul pin di selezione del tasto
	   ROM_GPIOPinIntEnable(BTU_SEL_GPIO_BASE,BTU_SEL_PIN);
	   
	   //abilita la gestione dell'interrupt sulla porta del tasto															   
       ROM_IntEnable(BTU_PORT_INTERRUPT);
	   
	   //abilita la gestione degli interrupt nella CPU
	   IntMasterEnable();	   

	}
	else
	{
	   //maschera l'interrupt
	   ROM_GPIOPinIntDisable(BTU_SEL_PERIPHERAL,BTU_SEL_PIN);
	}														
}

//=========================== clearButtonInterrupt ============================

/**
 Resetta l'interrupt
 */ 
void clearButtonInterrupt(void)
{
   //resetta l'interrupt
   ROM_GPIOPinIntClear(BTU_SEL_GPIO_BASE,BTU_SEL_PIN);

   //nota:eseguire il reset dell'interrupt all'inizio della ISR
   //perché occorrono alcuni cicli prima che l'interrupt venga resettato

}

//======================== isSelected =========================================

/**
  Determina se il asto è premuto:se il tasto è premuto il pin è in stato basso
*/  
BOOL isSelButtonPressed(void)
{
   
   unsigned long state=0;

   //legge lo stato della porta mascherando tutti i pin escluso il 7
   //lo stato del pin 7 viene restituito nel bit 7 del byte restituito
   state=ROM_GPIOPinRead(BTU_SEL_GPIO_BASE, GPIO_PIN_7);

   //nota:quando l'ingresso è basso significa che il tasto è premuto
   //se alto è rilasciato
   return state ? FALSE:TRUE;

}


