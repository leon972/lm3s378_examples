//********************************
//Blinky test
//********************************

//questo header contiene la mappatura della memoria della MCU
//
#include "inc/lm3s3748.h"

/**

  Test porte GPIO della MCU LM3S3748
  
  L'MCU ha 8 porte GPIO (indviduate dalle lettere A,B,C,D,E,F,G,H 
  e un totale di 68 pin sulla scheda.
  Per individuare a quale bit e a quale porta corrisponde ogni pin fare riferimento
  alla pagina 39 del manuale della scheda (Documento LM3S3748_EvalBoard_UM.pdf)
  I pin indicati con un asterisco sono usati per periferiche della scheda.
  Ogni pin è indicato da una sigla, esempio PD4/ADC7 questo indica che il pin indicato come
  ADC7 sulla scheda (I/O Break-out headers) corrisponde al bit 4 della porta D

*/



//*****************************************************************************
//
// Blink the on-board LED.
// Il led di stato sulla scheda è associato al pin 0 della porta F 
//(vedere lo schema a pag. 26 del manuale della scheda)

//
//*****************************************************************************
int
main(void)
{
    volatile unsigned long ulLoop;

    //
    // Enable the GPIO port that is used for the on-board LED.

	//Abilita la porta GPIOF che è collegata al led sulla scheda (bit 0)
    //System control register RGC2=Run Mode Clock Gating Control Register
	//SYSCTL_RCGC2_GPIOF=questo è un valore numerico che indica il bit relativo alla porta F
	//sul registro RGC2
	//NOTA:SYSCTL_RCGC2_R è un puntatore ad un'area della memoria che corrisponde
	//al registro RCGC2 quindi la seguente istruzione ha l'effetto di scrivere il valore
	//SYSCTL_RCGC2_GPIOF all'indirizzo individuato da SYSCTL_RCGC2_R

    SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;   

    //
    // Do a dummy read to insert a few cycles after enabling the peripheral.
    //
    ulLoop = SYSCTL_RCGC2_R;

    //
    // Enable the GPIO pin for the LED (PF0).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    //
	//scrive il bit 1 nel data direction register per settare il pin in modalità output
	//
    GPIO_PORTF_DIR_R = 0x01;   //DIR =Direction register 1=output 0=input
    GPIO_PORTF_DEN_R = 0x01;   //DEN = Digital Enable Abilita l'output digitale

    //
    // Loop forever.
    //
    while(1)
    {
        //
        // Turn on the LED.
		//Nota:l'MCU consente di settare singoli bit della porta senza dover usare la tecnica
		//read-modify write access GPIO_PORTF_DATA_R è un registro della porta F che consente di settare un singolo bit
        //
        GPIO_PORTF_DATA_R |= 0x01;	 //scrive il valore 1 perché il led è collegato al bit zero

        //
        // Delay for a bit.
        //
        for(ulLoop = 0; ulLoop < 200000; ulLoop++)
        {
        }

        //
        // Turn off the LED.
        //
        GPIO_PORTF_DATA_R &= ~(0x01);

        //
        // Delay for a bit.
        //
        for(ulLoop = 0; ulLoop < 200000; ulLoop++)
        {
        }
    }
}
