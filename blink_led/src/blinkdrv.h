//******************************************
// blink_led
// blinkdrv.h Interfaccia hardware per l'applicazione blink_led
// Programma di test per la scheda di sviluppo
// Stellaris EKK-LM3S3748.
// questo programma accende il led on-board
// con frequenza variabile e visualizza
// alcune informazioni di debug sul display LCD
// a colori 128x128 presente sulla scheda.
// Code by Leonardo Berti 25-01-2012
//******************************************

#ifndef __BLINKDRV__
#define __BLINKDRV__

#define LED_ON  1
#define LED_OFF 0

/**
	Inizializza i componenti hardware necessari all'applicazione
*/
void initHardware(void);

/**
   Ciclo di attesa
*/
void wait(unsigned long delay);

/**
  Accende/Spenge il led della scheda
*/
void switchLed(int switch_mode);

/**
  Accende/spenge il led e attende
*/
void blinkSequence(unsigned long onDelay,unsigned long offDelay,unsigned long cycles);

/**
  Determina lo stato attuale del led
*/
int isLedOn(void);
  
#endif
