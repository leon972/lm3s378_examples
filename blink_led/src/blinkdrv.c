//******************************************
// blink_led
// blinkdrv.c implementazione interfacci hardware per scheda EKK-LM3S3748
// Programma di test per la scheda di sviluppo
// Stellaris EKK-LM3S3748.
// questo programma accende il led on-board
// con frequenza variabile e (o) visualizza
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

#include "sysctl.h"
#include "gpio.h"
#include "hw_memmap.h"
								 
#define LCD_DEBUG  //abilita il display
#define USE_LED1 //abilita il led

//------------------------------------		 

#ifdef LCD_DEBUG								 
#include "lcddebug.h" //Debug sul display LCD
#endif

#include "mcstring.h" //minilib
#include "mcbuff.h"
#include "blinkdrv.h"
			  																	
#ifdef LCD_DEBUG

static tContext sContext; //contesto grafico per il display
#define BACK_COLOR ClrLightBlue
#define L_COLOR_1 ClrForestGreen
#define L_COLOR_2	ClrRed
#define FORE_COLOR ClrWhite

#endif

#define BUFF_SIZE 64									   
static char temp[BUFF_SIZE]; //buffer per stringhe temporanee
unsigned int curRow=0;
   
/**
	Inizializza i componenti hardware necessari all'applicazione
*/
void initHardware(void)
{ 
    volatile unsigned long ulLoop;

#ifdef LCD_DEBUG
    //
    // Inizializza il displaydriver
    //
    Formike128x128x16Init();

    //
    // Attiva la retroilluminazione del display
    //
    Formike128x128x16BacklightOn();

    //
    // Inizializza il contesto grafico.
    //
    GrContextInit(&sContext, &g_sFormike128x128x16); 

	LCD_debugInit(&sContext);
	
	LCD_clear(BACK_COLOR);
	
	LCD_drawBanner("Blink LED",0,ClrWhite,ClrSeaGreen); 
	
	GrFlush(&sContext); 	   
	
	curRow=2;

#endif

#ifdef USE_LED1


	//il led è collegato al pin 0 della portaf
	//abilita la porta F che contiene il pin per il led della scheda

	//abilita il clocking della porta F che ha il segnale del led sul pin 0
	//(tutte le periferiche sono disabilitate inizialmente)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    
	//Setta il pin come I/O digitale GPIO (altre modalità sono OD=open drain, I2C ecc...)
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_0);    

	//Imposta la "direzione del pin", cioè specifica se è un pin di output o input
	//in questo caso è un output
	GPIODirModeSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_DIR_MODE_OUT);		 
    
	/** Modalità alternativa usando le costanti dei registri di memoria
	    definite in inc/lm3s3748.h 	 (nota in questo caso sembra che l'LCD non funzioni)

	 //Abilita la periferica (clocking gate della porta F)
	 SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF; 
	 GPIO_PORTF_DEN_R = 0x01; //abilita l'output digitale del pin 0
	 //setta il pin 0 della porta F per indicare che è un pin di output
	 GPIO_PORTF_DIR_R = 0x01; 
    */			      
	  	   
#endif


}

/**
   Ciclo di attesa
*/
void wait(unsigned long delay)
{
   unsigned long ulLoop;
   unsigned long totDelay=delay*10000;

   for(ulLoop = 0; ulLoop < totDelay; ulLoop++)
   {
      //ciclo delay
   }
}

/**
  Accende/Spenge il led della scheda
*/
void switchLed(int switch_mode)
{
#ifdef USE_LED1
	if (switch_mode==LED_ON) {

 	   //modo alternativo:usa le difinizioni dei registri in inc/lm3s3748.h
	   //GPIO_PORTF_DATA_R |= 0x01;

	   //setta in stato alto il pin 0 della porta F collegato al LED (accende il led)
	   ROM_GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,0x1);
    }
	else 
	{
	   //resetta il pin 0 lasciando inalterati gli altri pin
	   //GPIO_PORTF_DATA_R &= ~(0x01);

	   //setta in stato basso il pin collegato al led (lascia inalterati gli altri pin)
	   //il MCU permette di settare i singoli pin via hardware (bitbanding)
	   //in modo da evitare il read-modify-write del valore della porta
	   ROM_GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,0x0);
	}
#endif
}


/**
  Determina lo stato attuale del led
*/
int isLedOn(void)
{
#ifdef USE_LED1
    //legge lo stato della porta
   // return (GPIO_PORTF_DATA_R & 0x01); 
   return (ROM_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)& 0x1);
#else
	return 0;
#endif

}

/**
  Avvia una sequenza di lampeggio del led
  @param onDelay tempo in cui resta acceso
  @param offDelay tempo in cui resta spento
  @param cycles per quanti cicli si ripete la sequenza acceso/spento
*/
void blinkSequence(unsigned long onDelay,unsigned long offDelay,unsigned long cycles)
{
   unsigned long i;
   //stato corrente del led
   int state=isLedOn();	
   
#ifdef LCD_DEBUG
      
   //visualizza informazioni sulla sequenza corrente nel display
   mc_buff_set_len(temp,0,BUFF_SIZE);		
   mc_buff_append(temp,"ON/OFF ",BUFF_SIZE);
   mc_buff_iappend(temp,onDelay,BUFF_SIZE);   
   mc_buff_append(temp,"/",BUFF_SIZE);
   mc_buff_iappend(temp,offDelay,BUFF_SIZE); 

   if (curRow>=LCD_getRowsCount())
   {
      curRow=0;
	  LCD_clear(BACK_COLOR);
   }

   LCD_hilightRow(curRow,curRow % 2 ? L_COLOR_1:L_COLOR_2);

   LCD_writeTextToRow(curRow,temp,0);

   GrFlush(&sContext);
   
   ++curRow;            

#endif
    		
   for (i=0;i<cycles;i++)
   {
      state=!state;
      
	  switchLed(state);

	  wait(state==LED_ON ? onDelay:offDelay);
	  
	  state=!state;
      
	  switchLed(state);

	  wait(state==LED_ON ? onDelay:offDelay);	  												               
   }    
}
