//*****************************************************************************
//  timers_util.h - Utility gestione timers
//  MCU ARM Cortex M3 modello LM3S3748
//  Code by Leonardo Berti (c) 2012
//
//*****************************************************************************

#ifndef _TIMERS_UTIL_INCLUDE_
#define _TIMERS_UTIL_INCLUDE_

#include "cmdefs.h"


/**
  Timer.Contiene le informazioni necessarie ad individuare
  il timer nella memory map.
  I dati di questa struttura devono essere manipolati usando
  le funzioni di questo modulo e non manualmente
*/
typedef struct TIMER_DATA 
{
    /**
	  ulTimer timer es. TIMER0_BASE,TIMER1_BASE ecc...
	  vedere costanti nella mappa della memoria
	  definite in hw_memmap.h
	  */
    unsigned long ulTimer;

	/**
	  ulSubtimer sotto timer, ogni timer ha due sotto timer indipendenti:
	  timer A e timer B, utilizzare le costanti TIMER_A e TIMER_B
	  nel caso di utilizzo del timer a 32 bit si può usare solo TIMER_A
	*/
	unsigned long ulSubTimer;

	/**
	  Vettore interrupt associato al timer
	  usare le costanti INT_TIMER0A,INT_TIMER0B,INT_TIMER1A ecc...
	*/
	unsigned long ulInterrupt;

	/**
	  Sorgente di interrupt associato a questo timer
	  usare le costanti TIMER_TIMA_TIMEOUT,ecc...
	*/
	unsigned long ulIntSource;

} TimerData,*TimerData_Ptr; 

/**
  Restituisce il clock corrente della CPU in Hz
*/
unsigned long getCPUClock(void);

/**
  Converte da millisecondi a cicli.E' utilizzato con la funzione initTimer
  La conversione viene effettuata dividendo il clock della CPU in Hz per 1000 
  e moltiplicando per il parametro millis.
  @param millis valore in millisecondi
*/
unsigned long millisToCycles(unsigned long millis);

/**
  Inizializza e abilita il timer
  @param timer la struttura che contiene i dati del timer e viene popolata da questo metodo
  @param ultimer identificativo del timer.E' l'indirizzo di memoria
  su cui è mappato il timer.Usare costanti TIMER0_BASE,TIMER1_BASE ecc...
  @param ulSubtimer sotto timer, TIMER_A o TIMER_B, se viene creato un timer a 32 bit usa sempre il timer A
  @param ulConfig parametro di configurazione che indica il tipo di timer
  Usare costanti TIMER_CFG_32_BIT_PER definite in timer.h
  @param enableInterrupt se settato abilita l'interrupt relatvo al timer
  Se ad esempio si utilizza il timer 0 subtimer A, verrà abilitato l'interrupt 
  INT_TIMER0A e si dovrà definire nel codice di startup la ISR relativa a questo timer. 
  @param timeoutCycles valore caricato nel timer espresso in cicli della CPU
  @param enableTimer se settato, abilita subito il timer.Di solito questo parametro è FALSE
  Il timer viene abilitato in seguito con enableTimer
  per convertire da millisecondi a cicli utilizzare millisToCycles
  Usare getCPUClock per ottenere il valore in secondi
  @return TRUE se i parametri sono coerenti.
*/

BOOL 
initTimer(TimerData_Ptr timerData,unsigned long ulTimer,unsigned long ulSubTimer,
			unsigned long ulConfig,unsigned long timeoutCycles,
			BOOL enableInterrupt,BOOL enableTimer);

/**
  Abilita/disabilita il timer
*/
void enableTimer(TimerData_Ptr timerData,BOOL enable);

/**
 Determina se i dati del timer sono validi
*/
BOOL isTimerValid(TimerData_Ptr timerData);

/**
 Carica il valore di timeout/match del timer
 @param timerData dati di mappatura del timer
 @param timeoutMillis valore di timeout in millisecondi  
*/
void timerLoadMillis(TimerData_Ptr timerData,unsigned long timeoutMillis);

/**
  Controlla la modalità di stallo del timer quando il processore va in modalità
  debug.E' usata per fermare il timer in modalità debug.
*/
void stallTimer(TimerData_Ptr timer,BOOL bStall);

#endif

