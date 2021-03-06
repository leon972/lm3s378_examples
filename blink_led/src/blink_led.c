//******************************************
// blink_led
// Programma di test per la scheda di sviluppo
// Stellaris EKK-LM3S3748.
// questo programma accende il led on-board
// con frequenza variabile e visualizza
// alcune informazioni di debug sul display LCD
// a colori 128x128 presente sulla scheda.
// Code by Leonardo Berti 25-01-2012
//******************************************

#include "inc/hw_types.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "grlib/grlib.h"
#include "drivers/formike128x128x16.h"
#include "inc/lm3s3748.h" //definizioni MCU LM3S3748 (ARM Cortex M3)

//------------------------------------
//Debug sul display LCD
#include "lcddebug.h"
#include "mcstring.h"
//interfaccia hardware per l'applicazione blink led
#include "blinkdrv.h" 
	 
int main()
{ 

//blink_old(); 
   //inizializza l'hardware
   initHardware();
    
   while(1)
   {																				  
      blinkSequence(10,20,10);
      blinkSequence(50,23,5);       	  
	  blinkSequence(100,60,10);
	  blinkSequence(10,260,10);
	  blinkSequence(155,40,10);
	  blinkSequence(5,65,4);
	  blinkSequence(65,5,4);
	  blinkSequence(400,10,2);

   }

}

