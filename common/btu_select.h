//*****************************************************************************
//
// btu_select.h
// Gestione del tasto di selezione sulla scheda di sviluppo
// Code by Leonardo Berti (c) 2012
//
//*****************************************************************************

#ifndef _BTU_SELECT_INCLUDE_
#define _BTU_SELECT_INCLUDE_

#include "cmdefs.h"

/**
 Abilita il tasto di selezione
*/
void enableSelectButton(void);

/**
  Abilita/disabilita l'interrupt associato al pulsante
*/
void setButtonIterruptEnabled(BOOL enabled);

/**
  Resetta l'interrupt relativo al tasto
*/
void clearButtonInterrupt(void);

/**
 Determina lo stato corrente del tasto
*/
BOOL isSelButtonPressed(void);


#endif

