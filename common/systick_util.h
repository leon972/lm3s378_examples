//*****************************************************************************
//  systick_util.h - Utility gestione del SysTick
//  
//  Code by Leonardo Berti (c) 2012
//
//*****************************************************************************

#ifndef SYSTICK_UTIL
#define SYSTICK_UTIL

/**
Imposta il SysTick  (timer di sistema general purpose)
*/
void setupSysTick(void);

/**
Interrompe l'esecuzione per il numero specificato di secondi
*/
void waitSec(unsigned long ulSeconds);
  
#endif

