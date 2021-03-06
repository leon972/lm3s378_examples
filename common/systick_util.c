//*****************************************************************************
//  systick_util.c - Utility gestione del SysTick
//  
//  Code by Leonardo Berti (c) 2012
//
//*****************************************************************************

#include "inc/hw_types.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "gpio.h"
#include "hw_memmap.h"

#include "systick_util.h"

static short initialized=0;

void setupSysTick(void)
{
	ROM_SysTickPeriodSet(ROM_SysCtlClockGet());
    ROM_SysTickEnable();		
	initialized=1;
}


void waitSec(unsigned long ulSeconds)
{
   if (!initialized)
   {
   		setupSysTick();
   }

   //
    // Loop while there are more seconds to wait.
    //
    while(ulSeconds--)
    {
        //
        // Wait until the SysTick value is less than 1000.
        //
        while(ROM_SysTickValueGet() > 1000)
        {
        }

        //
        // Wait until the SysTick value is greater than 1000.
        //
        while(ROM_SysTickValueGet() < 1000)
        {
        }
    }
}

