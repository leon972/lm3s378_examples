//*****************************************************************************
//  timers_util.c - Utility gestione timers (implementazione)
//  MCU LM3S3748 - (ARM Cortex M3)
//  Code by Leonardo Berti (c) 2012
//
//*****************************************************************************

#include "timers_util.h"

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"


//-------------Sellaris DriverLib----- 
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"



/**clock corrente in Hz*/
unsigned long m_procClock=0;

//======================= getCPUClock =========================================

unsigned long getCPUClock(void)
{
   if (!m_procClock)
   {
      //ROM_SysCtlClockGet restituisce la frequenza di clock in Hz della CPU
	  //es. rende 8000000 (8 MHz) se il clock è settato con
	  //ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
      //                 SYSCTL_XTAL_8MHZ);
	  //il valore restituito non è accurato se quando viene settato il clock
	  //si usano valori non congruenti o non supportati
   	  
	  m_procClock=ROM_SysCtlClockGet();	   	   
   }   

   return m_procClock; 
}

//======================= millisToCycles ======================================

unsigned long millisToCycles(unsigned long millis)
{
   //attenzione all'overflow!
   return (millis*(getCPUClock()/1000));
}
										 										 										 
//====================== initTimer ============================================

BOOL 
initTimer(TimerData_Ptr timerData,unsigned long ulTimer,unsigned long ulSubTimer,
			unsigned long ulConfig,unsigned long timeoutCycles,BOOL enableInterrupt
			,BOOL enableTimer) 
{

	 
	  unsigned long ulInt=0; //interrupt associato
	  unsigned long ulIntSource; //sorgente di interrupt	
	  unsigned long peripheral; //perifierica associata al timer

      if (!timeoutCycles) return FALSE;

	  if (ulConfig != TIMER_CFG_16_BIT_PAIR)
	  {
	    //è un timer a 32 bit : in questo caso il sub timer deve essere l'A
	  	ulSubTimer=TIMER_A;
	  }	
      								  
	  switch (ulTimer)
	  {
	     case TIMER0_BASE:
		 
			ulInt=(ulSubTimer==TIMER_A ? INT_TIMER0A : INT_TIMER0B);
			peripheral=SYSCTL_PERIPH_TIMER0;
			break;
		 case TIMER1_BASE:
		 
			ulInt=(ulSubTimer==TIMER_A ? INT_TIMER1A : INT_TIMER1B);
			peripheral=SYSCTL_PERIPH_TIMER1;
			break;
		 case TIMER2_BASE:
		 
			ulInt=(ulSubTimer==TIMER_A ? INT_TIMER2A : INT_TIMER2B);
			peripheral=SYSCTL_PERIPH_TIMER2;
			break;
		 case TIMER3_BASE:
		 
			ulInt=(ulSubTimer==TIMER_A ? INT_TIMER3A : INT_TIMER3B);
			peripheral=SYSCTL_PERIPH_TIMER3;
			break;

		 default:
		 	//timer base non valido
			return FALSE;
	  }

	  //sub timer non valido
	  if (!(ulSubTimer==TIMER_A || ulSubTimer==TIMER_B)) return FALSE;
	  
	  //abilita il timer selezionato
	  ROM_SysCtlPeripheralEnable(peripheral);

	  //configura il timer:specifica se è di tipo one-show,periodico ecc...
      ROM_TimerConfigure(ulTimer,ulConfig);  					       

	  //carica il valore di timeout/match del timer
	  ROM_TimerLoadSet(ulTimer, ulSubTimer, timeoutCycles);										

	  timerData->ulTimer=ulTimer;
	  timerData->ulSubTimer=ulSubTimer;
	  timerData->ulInterrupt=ulInt;
      timerData->ulIntSource=0;

	  if (enableInterrupt)
	  {
	     //abilita la gestione degli interrupt da parte del processore
	     IntMasterEnable();
	     //abilita l'interrupt
	  	 ROM_IntEnable(ulInt);
	  	 
		 ulIntSource=ulSubTimer == TIMER_A ? TIMER_TIMA_TIMEOUT:TIMER_TIMB_TIMEOUT;	

		 //abilita la sorgente di interrupt per il timer utilizzato
		 ROM_TimerIntEnable(ulTimer,ulIntSource);		 
		 
		 timerData->ulIntSource=ulIntSource;
		 		   
	  }

	  if (enableTimer) {

	  	//abilita il timer
	  	ROM_TimerEnable(ulTimer,ulSubTimer);

	  }

	  return TRUE;
}

//=========================== enabledTimer =========================================

void enableTimer(TimerData_Ptr timerData,BOOL enable)
{
  if (enable)
  {	
	  ROM_TimerEnable(timerData->ulTimer,timerData->ulSubTimer);	
  }
  else
  {
  	  ROM_TimerDisable(timerData->ulTimer,timerData->ulSubTimer);
  }   
}

//=========================== isTimerValid =========================================

BOOL isTimerValid(TimerData_Ptr timerData)
{
 
  unsigned long temp;	

  if (!timerData) return FALSE;
  
  temp=timerData->ulTimer;
  
  if (temp==TIMER0_BASE || temp==TIMER1_BASE || TIMER2_BASE || TIMER3_BASE)
  {
     return (timerData->ulSubTimer==TIMER_A || timerData->ulSubTimer==TIMER_B);
  }

  return FALSE;
}

//============================ timerLoad ===========================================

void timerLoadMillis(TimerData_Ptr timerData,unsigned long timeoutMillis)
{
   if (!isTimerValid(timerData)) return;
   ROM_TimerLoadSet(timerData->ulTimer, timerData->ulSubTimer, millisToCycles(timeoutMillis));		
}

//============================ stallTimer ==========================================

void stallTimer(TimerData_Ptr timer,BOOL bStall)
{

 if (isTimerValid(timer))
 {
 	ROM_TimerControlStall(timer->ulTimer,timer->ulSubTimer,bStall);	 	
 }
	
}



