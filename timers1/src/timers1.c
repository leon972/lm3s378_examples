//*****************************************************************************
//  timers1.c 
//  Applicazione di test di utilizzo dei timers e del tasto di selezione
//  sulla scheda di valutazione EKK-LM3S3748.
//
//	Code by Leonardo Berti (c) 2012
//
//*****************************************************************************

#include "timers1.h"
#include "lcddebug.h"  //Debug sul display LCD
#include "mcstring.h"  //minilib string
#include "mcbuff.h"	   //minilib string builder
#include "led.h"       //onboard status led
#include "timers_util.h" //utilità gestione timers
#include "btu_select.h" //gestione del tasto select sulla scheda

#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_ints.h"
#include "hw_nvic.h"   //nested vectored interrupt controller
#include "sysctl.h"    //costanti system control
#include "rom.h"	   //funzioni ROM

//Stellaris Driverlib
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#define BUFF_SIZE 48

#define MAX_CNT 65535

//timeout in millisecondi per il timer one shot
#define ONE_SHOT_TIMEOUT 6000 

//se definito, utilizza gli interrupt per l'intercettazione dei click del tasto 
//di selezione	
#define USE_BTU_INTERRUPT 

static char tempBuff[48]; //general purpose string

extern void wait(unsigned long); //led.c

BOOL pressed=FALSE;
BOOL lastPressed=FALSE;

//timers
TimerInfo timer1;
TimerInfo timer2;
TimerInfo timer3;
TimerInfo timer4;

void ISRTimer0A(void)
{
     //appena invocata la ISR ocorre subito resettare l'interrupt
	 //per evitare una doppia invocazione (viene specificata 
	 //anche la sorgente di interrupt)	
	 handleInterrupt(&timer1); 
	 	   				   
}

void ISRTimer0B(void)
{

}

void ISRTimer1A(void)
{
   handleInterrupt(&timer2);  

}

void ISRTimer1B(void)
{

}

void ISRTimer2A(void)
{

   handleInterrupt(&timer3); 

}

void ISRTimer2B(void)
{

}

void ISRTimer3A(void)
{						   
  handleInterrupt(&timer4);   

}

void ISRTimer3B(void)
{

}

/**
  Gestisce l'interrupt:il comportamento è diverso per timer one-shot
  e periodici.
*/
void handleInterrupt(TimerInfo_Ptr timer)
{

   ROM_TimerIntClear(timer->timer.ulTimer,TIMER_TIMA_TIMEOUT);

   ++timer->counter;

   if (timer->counter>MAX_CNT) timer->counter=0;

   displayTimer(timer);

}

/**
 Resetta i contatori periodici
*/
void resetAllCounters()
{
   //disabilita gli interrupt
   IntMasterDisable();

   timer1.counter=0;
   timer2.counter=0;
   timer3.counter=0;   

   //riabilita gli interrupt
   IntMasterEnable();
}					  

/**
 Inizializza le informazioni del timer
*/
BOOL initTimerInfo(TimerInfo_Ptr timer,int type,unsigned long timer_address,
			unsigned long timeoutMillis,const char* name,int row,
			unsigned long fColor,unsigned long bColor) {
   
   unsigned long ulConfig;
   
   timer->name=name;
   timer->counter=0;
   timer->displayRow=row;
   timer->fColor=fColor;
   timer->bColor=bColor;
   timer->timeoutMillis=timeoutMillis;
   timer->timerType=type;

   switch (type)
   {
      case TM_PERIODIC:
	  	ulConfig=TIMER_CFG_32_BIT_PER; //timer periodico:genera un interrupt timeout quando arriva a zero e poi ricarica il valore
	  	break;
	  case TM_ONESHOT:
	    ulConfig= TIMER_CFG_32_BIT_OS; //timer oneshot:genera un interrupt quando arriva a zero e poi resta a zero senza generare altri interrupt
		break;
	  default:
	  	return false;
   }

   //inizializza il timer
   return initTimer(&timer->timer,timer_address,TIMER_A,ulConfig,millisToCycles(timeoutMillis),
                    TRUE,FALSE); 
   
}

/**
 Visualizza le informazioni sul timer usando il display LCD della scheda
*/
void displayTimer(TimerInfo_Ptr timer)
{
   //disabilita gli interrupt
   IntMasterDisable(); 											
        			      
   if (timer->timerType==TM_ONESHOT)
   {

        LCD_hilightRow(10,ClrBlack);        

        if (!timer->counter)
		{
           LCD_hilightRow(timer->displayRow,ClrOrange); 
   
           LCD_debugColors(ClrBlue,ClrBlue);

		    //timeout non ancora avvenuto
		   mc_buff_set_len(tempBuff,0,BUFF_SIZE);
		   mc_buff_append(tempBuff,timer->name,BUFF_SIZE);
		   mc_buff_append(tempBuff,":WAITING...",BUFF_SIZE);
		   
		   LCD_writeTextToRow(timer->displayRow,tempBuff,false);

		}
		else
		{
		   
		   LCD_hilightRow(timer->displayRow,ClrGreen); 
   
           LCD_debugColors(ClrWhite,ClrWhite);

		   mc_buff_set_len(tempBuff,0,BUFF_SIZE);
		   mc_buff_append(tempBuff,timer->name,BUFF_SIZE);
		   mc_buff_append(tempBuff,":TIMEOUT!",BUFF_SIZE);
		   
		   LCD_writeTextToRow(timer->displayRow,tempBuff,false);		   

		   LCD_writeTextToRow(10,"Press to reload.",true);
		}
   }
   else {

        LCD_hilightRow(timer->displayRow,timer->bColor); 
   
        LCD_debugColors(timer->fColor,timer->fColor);

   		LCD_writeIntToRow(timer->displayRow,timer->name,timer->counter);

   }

   LCD_Flush();

   //riabilita gli interrupt
   IntMasterEnable();
}

/**
Inizializza l'hardware e i timers senza abilitarli
*/
BOOL initApplication(void)
{

   LCD_InitDisplay("Timers1",ClrWhite,ClrDarkBlue);
   
   //inizializza i timers a 32 bit
   if (!initTimerInfo(&timer1,TM_PERIODIC,TIMER0_BASE,1000,"timer1",4,ClrWhite,ClrDarkRed)) return FALSE;

   if (!initTimerInfo(&timer2,TM_PERIODIC,TIMER1_BASE,500,"timer2",5,ClrWhite,ClrDarkGreen)) return FALSE;

   if (!initTimerInfo(&timer3,TM_PERIODIC,TIMER2_BASE,3000,"timer3",6,ClrWhite,ClrDarkTurquoise)) return FALSE;
   
   //timer di tipo one shot
   if (!initTimerInfo(&timer4,TM_ONESHOT,TIMER3_BASE,ONE_SHOT_TIMEOUT,"timer4",7,ClrLightBlue,ClrOrange)) return FALSE; 	    

   return TRUE;

}
/**
  Gestione interrupt del tasto di selezione
*/
void ISRButtonClick(void)
{
//   if (isSelButtonPressed())
//   {          
	   clearButtonInterrupt();           

	   onSelBtuPressed();  
   //}
}

/**
  Azione eseguita in seguito alla pressione del tasto di selezione
*/
void onSelBtuPressed(void)
{

   //resetta i contatori periodici
   resetAllCounters();
   //ricarica il valore di time out nel timer one shot
   timerLoadMillis(&timer4.timer,ONE_SHOT_TIMEOUT);
   //resetta il contatore del timer che indica se è avvenuto o meno il timeout	
   timer4.counter=0;
   //riabilita il timer one shot disabilitato automaticamente dopo il timeout 
   enableTimer(&timer4.timer,TRUE);	

   displayTimer(&timer4);

}

/**
  Esegue il polling del tasto select per determinare cambiamenti ed intraprendere l'azione corrispondente
*/
void pollSelectButton(void)
{

   	wait(20);
		
	    pressed=isSelButtonPressed();
		
		if (pressed!=lastPressed)
		{
		    lastPressed=pressed;
			
            LCD_hilightRow(8,ClrBlack);

			if (pressed)
			{
			    LCD_writeTextToRow(8,"PRESSED!",TRUE);

				onSelBtuPressed();
			}
			else
			{
			    LCD_writeTextToRow(8,"not pressed.",TRUE);
			}

			LCD_Flush();
					
		}

}

//entry point
int main(void)
{

   //usa l'oscillatore come clock di sistema e lo stesso 
   //oscillatore come sorgente di clock
   ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_8MHZ);

   if (!initApplication())
   {
   		LCD_clear(ClrRed);
		LCD_debugColors(ClrYellow,ClrYellow);
		LCD_writeTextToRow(6,"INIT ERROR!",TRUE);
		LCD_Flush();
   }
   else
   {

      displayTimer(&timer1);
	  displayTimer(&timer2);
	  displayTimer(&timer3);
	  displayTimer(&timer4);

	  LCD_debugColors(ClrLimeGreen,ClrLightPink);

	  LCD_writeIntToRow(3,"CPU clock:",getCPUClock());
	  
	 
	   
	  //avvia il timer:quando va in timeout viene invocata la funzione ISRTimer0A
	  enableTimer(&timer1.timer,TRUE);

	  enableTimer(&timer2.timer,TRUE);

	  enableTimer(&timer3.timer,TRUE);

	  enableTimer(&timer4.timer,TRUE);

	  enableSelectButton();

#ifdef USE_BTU_INTERRUPT

	  setButtonIterruptEnabled(TRUE);

#endif
	  								  
	  //stallTimer(&timer1.timer,TRUE);
	     		  
   }      					    

   while(1)
   { 

#ifndef USE_BTU_INTERRUPT

	 //polling del tasto di selezione
   	 pollSelectButton();

#endif	 								
		     
	 //esecuzione infinita
   }					  
}
