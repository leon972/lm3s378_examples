//*****************************************************************************
//  led.h - Gestione del led general purpose sulla cheda di valutazione
//  Code by Leonardo Berti (c) 2012
//
//*****************************************************************************

#ifndef _LED_INCLUDE_
#define _LED_INCLUDE_

#define SWITCH_ON  1
#define SWITCH_OFF 0

/**
 Abilita/disabilita il led
*/
void enableLed(unsigned long enabled);

/**
  Accende/Spenge il led principale della scheda
*/
void switchLed(unsigned long switch_mode);

/**
  Accende/spenge il led principale e attende
  @param tempo accensione (valore tipico 5 - 400)
  @param tempo spegnimento (valore tipico 5 - 400)
  \warning i tempi riportati dipendono dal clock della CPU e non sono in unità di tempo

*/
void blinkSequence(unsigned long onDelay,unsigned long offDelay,unsigned long cycles);

/**
  Determina lo stato attuale del led principale
*/
int isLedOn(void);

#endif

