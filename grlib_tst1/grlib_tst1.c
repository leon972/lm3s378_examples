#include "inc/hw_types.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "grlib/grlib.h"
#include "drivers/formike128x128x16.h"
//------------------------------------
//Debug sul display LCD
#include "lcddebug.h"
#include "mcstring.h"

static tContext sContext;

#define BUFF_SIZE 64

static char temp[BUFF_SIZE];
 
//extern const unsigned int myValue;

//void writeBool(unsigned long x,unsigned long y,const char* label,int value)
//{
//  
//  if (sContext.pFont != &g_sFontFixed6x8) {   
//
//		tFont temp=sContext.pFont;
//  		GrContextFontSet(&sContext, &g_sFontFixed6x8);		
//  }
//    
//  GrStringDraw(&sContext, label,-1,x,y,false);    
//  GrStringDraw(&sContext, value ? ":1":":0",-1,x+6*6,y,false);                       
//
//  sContex.pFont=temp;
//
//}
 
void initDisplay()
{
 	//
    // Initialize the display driver.
    //
    Formike128x128x16Init();

    //
    // Turn on the backlight.
    //
    Formike128x128x16BacklightOn();

    //
    // Initialize the graphics context.
    //
    GrContextInit(&sContext, &g_sFormike128x128x16);

}

int main(void)
{
    
  //  tRectangle sRect;

	

	unsigned long len;
	unsigned long i;
				      
	initDisplay();   

	
    LCD_debugInit(&sContext);

    len=LCD_getRowsCount();	   
	
	LCD_hilightRow(0,ClrDarkBlue);		
	LCD_hilightRow(1,ClrDarkRed);
	
	mc_memclear(temp,BUFF_SIZE);
	mc_itoa(temp,-41,BUFF_SIZE,10);
				
	LCD_writeTextToRow(0,"prova",0);
	LCD_writeTextToRow(1,temp,1);
										
	for (i=2;i<len;i++)
	{
		if (i%2)
		{
		   LCD_hilightRow(i,ClrDarkBlue);
		   LCD_debugColors(ClrWhite,ClrBlue);
		}
		else {

		   LCD_hilightRow(i,ClrDarkKhaki);
		   LCD_debugColors(ClrPink,ClrAzure);
		}
		
		LCD_writeBoolToRow(i,"lineax",i%2);										  	  								 

	}
   
    LCD_drawBanner("Prova",0,ClrWhite,ClrRed); 

    //
    // Flush any cached drawing operations.
    //
    GrFlush(&sContext);

    //
    // Loop forever.
    //
    while(1)
    {
    }
}

