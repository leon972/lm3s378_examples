//*****************************************************************************
//  timers1.h 
//  Applicazione di test di utilizzo dei timers e del tasto di selezione
//  sulla scheda di valutazione EKK-LM3S3748.
//
//  Code by Leonardo Berti (c) 2012
//
//*****************************************************************************

#ifndef _TIMERS1_INCLUDE_
#define _TIMERS1_INCLUDE_

#include "cmdefs.h"	 //definizioni comuni
#include "timers_util.h" //utilità gestione timers

//timer periodico
#define TM_PERIODIC 0
//timer one shot
#define TM_ONESHOT 1 

/**
 Questa struttura contiene le informazioni sul timer,sugli interrupt gestiti
 dal timer e sulla modalità di visualizzazione sul display
*/
typedef struct TIMER_INFO_TAG
{
   TimerData timer; //dati mappatura timer
   int counter;		//contatore interno
   int displayRow;	//riga del display su cui visualizzare i dati
   int timerType;   //tipo timer TM_PERIODIC,TM_ONESHOT
   const char* name; //nome del timer
   unsigned long fColor; //colore primo piano visualizzazione sul display
   unsigned long bColor; //colore secondo piano
   unsigned long timeoutMillis; //valore del timeout in millisecondi

} TimerInfo,*TimerInfo_Ptr;
   
/**
 Inizializza le informazioni del timer
 @param timer dati timer
 @param type una costate TM_PERIODIC per un timer periodico,TM_ONESHOT per un timer one shot
 @param timer_address indirizzo base timer es. TIMER0_BASE
 @param timeoutMillis timeout in 1/1000 s
 @param name nome timer visualizzato
 @param row riga su cui viene visualizzato
 @param fColor colore primo piano
 @param bcolor colore secondo piano
*/
BOOL initTimerInfo(TimerInfo_Ptr timer,int type,unsigned long timer_address,
			unsigned long timeoutMillis,const char* name,int row,
			unsigned long fColor,unsigned long bColor);

/**
 Visualizza le informazioni sul timer usando il display LCD della scheda
*/
void displayTimer(TimerInfo_Ptr timer);


/**
 Gestisce il verificarsi dell'interrupt
*/

void handleInterrupt(TimerInfo_Ptr timer);

/**
Inizializza l'hardware e gli oggetti necessari all'applicazione
*/

BOOL initApplication(void);

/**
 Gestore click su tasto di selezione
*/
void onSelBtuPressed(void);

/**
  Esegue il polling del tasto select per determinare cambiamenti ed intraprendere l'azione corrispondente
*/
void pollSelectButton(void);

/**
  Gestione interrupt del tasto di selezione
*/
void ISRButtonClick(void);

// ----------------- ISR utilizzate dai timers ----------------------------------
//queste isr sono mappate nel file startup.s all'interno del vettore degli interrupt
//

/**
 ISR Timer 0 subtimer A
*/
void ISRTimer0A(void);
void ISRTimer0B(void);
void ISRTimer1A(void);
void ISRTimer1B(void);
void ISRTimer2A(void);
void ISRTimer2B(void);
void ISRTimer3A(void);
void ISRTimer3B(void);

#endif

