//*************************************************
//	Modulo utilizzato per l'output di informazioni
//	di debug su un display controllato attraverso
//	la GraphicsLibrary Texas Instruments
//  Implementa l'interfaccia LCD_debug usando il driver
//  del display a colori Formike 128x128x16 
//	Code by Leonardo Berti (c) 2012
//	21-01-2012
//*************************************************

#include "lcddebug.h"
#include "mcbuff.h"	  //minilib: string builder
#include "mcstring.h" //minilib: string
#include "grlib/grlib.h"
#include "drivers/formike128x128x16.h" //interfaccia del driver per il display

/**font usato per il debug*/
#define LCD_DEBUG_FONT &g_sFontFixed6x8

/**larghezza carattere usata come font metrics per 
   calcolo posizione testo
**/

#define CHAR_W 6
#define LCD_LARGE_FONT &g_sFontCmss16

/**altezza linee sul display (dipende dal font usato per debug)*/

#define LCD_ROW_HEIGHT 9

#define LCD_DEFAULT_COLOR1 ClrWhite
#define LCD_DEFAULT_COLOR2 ClrRed
#define LCD_ALT_COLOR1 ClrBlue
#define LCD_ALT_COLOR2 ClrBeige

#define GPBUFF_SIZE 64

/**Contesto grafico utilizzato per invocare le funzioni della GraphicLibrary
viene settato al momento dell'inizializzazione del modulo da LCD_DebugInit
La mofalità di inizializzazione del display del graphic constext dipende
dal driver del display LCD
*/

static tContext *pContext;
static tContext sContext; 					
//buffer temporaneo per scriuttura di valori
static char gpbuff[GPBUFF_SIZE];

static unsigned long ClrDebugCaption=ClrWhite;
static unsigned long ClrDebugValue=ClrPink;

typedef struct STYLE_STRUCT_TAG
{
   const tFont *font;  //font 
   unsigned long color; //colore disegno

} STYLE_STRUCT;

static short stylePushed;

STYLE_STRUCT tempStyle;

/**
 Inizializza il display usando il driver per il display Formike 128x128x16 
*/
void LCD_InitDisplay(const char* banner,unsigned long foreColor,unsigned long backColor)
{

    //inizializza il display driver
    Formike128x128x16Init();

    //attiva la retroilluminazione
    Formike128x128x16BacklightOn();

    //inizializza il contesto grafico
    GrContextInit(&sContext, &g_sFormike128x128x16);

	//inizializza il modulo LCD_debug
	LCD_debugInit(&sContext);
	
	//cancella lo schermo usando il colore di sfondo
	LCD_clear(backColor);

	if (backColor==LCD_DEFAULT_COLOR1 || backColor==LCD_DEFAULT_COLOR2)
	{
	     LCD_debugColors(LCD_ALT_COLOR1,LCD_ALT_COLOR2);
	}
	else
	{
		LCD_debugColors(LCD_DEFAULT_COLOR1,LCD_DEFAULT_COLOR2);
	}

	if (banner)
	{
		LCD_drawBanner(banner,0,foreColor,backColor);
	}

	LCD_Flush();
}
/**
  Salva lo stile corrente
*/
void pushStyle()
{
   tempStyle.font=pContext->pFont;
   tempStyle.color=pContext->ulForeground;
   stylePushed=1;
}

/**
 Ricarica lo stile
*/

void popStyle()
{  
  if (stylePushed)
  {
     pContext->pFont=tempStyle.font;
	 pContext->ulForeground=tempStyle.color;
	 stylePushed=0;
  }  										
}


//===================================================================

void LCD_debugInit(tContext *context)
{
   pContext=context;
}

//============LCD_getRowHeight=========================================

unsigned long LCD_getRowHeight(void)
{
   return (unsigned long)LCD_ROW_HEIGHT;
}

//============LCD_getRowsCount=========================================

unsigned long LCD_getRowsCount(void)
{
   return pContext->pDisplay->usHeight/LCD_ROW_HEIGHT;
}
																	 
//=================LCD_debugColors====================================

void LCD_debugColors(unsigned long captionColor,unsigned long valueColor)
{ 
   ClrDebugCaption=captionColor;
   ClrDebugValue=valueColor;
}
//====================LCD_writeBool===================================
void LCD_writeBool(unsigned long x,unsigned long y,const char* label,int value) {  

  pushStyle();
  										   
  GrContextFontSet(pContext, LCD_DEBUG_FONT);						    											 											   

  GrContextForegroundSet(pContext, ClrDebugCaption);																			   
  GrStringDraw(pContext, label,-1,x,y,false);  
  GrContextForegroundSet(pContext, ClrDebugValue);	 							   
  GrStringDraw(pContext, value ? ":1":":0",-1,x+mc_strlen(label)*CHAR_W,y,false);					  																		   
  
  popStyle();
  														  
}																	  

//============LCD_writeBoolToRow======================================

void LCD_writeBoolToRow(int row,const char* label,int value)
{
	LCD_writeBool(1,row*LCD_ROW_HEIGHT,label,value);
}

//============ LCD_writeIntToRow =====================================

void LCD_writeIntToRow(int row,const char* label,int value)
{

  const unsigned long y=row*LCD_ROW_HEIGHT;
  int unsigned long sz;

  pushStyle();
    											   
  GrContextFontSet(pContext, LCD_DEBUG_FONT);						  

  //etichetta
  mc_buff_set_len(gpbuff,0,GPBUFF_SIZE);
  mc_buff_append(gpbuff,label,GPBUFF_SIZE);
  mc_buff_append(gpbuff,":",GPBUFF_SIZE);
  
  GrContextForegroundSet(pContext, ClrDebugCaption);

  GrStringDraw(pContext, gpbuff,-1,1,y,false); 

  sz=mc_strlen(gpbuff);

  mc_buff_set_len(gpbuff,0,GPBUFF_SIZE);

  //valore
  mc_buff_iappend(gpbuff,value,GPBUFF_SIZE);  
  						 
  GrContextForegroundSet(pContext, ClrDebugValue);	

  GrStringDraw(pContext, gpbuff,-1,1+sz*CHAR_W,y,false);

  popStyle();   
   
}

//=================== LCD_writeTextToRow =============================
void LCD_writeTextToRow(int row,const char* text,int centered)
{   			
	
	long y= row * LCD_ROW_HEIGHT;
								 	
	pushStyle();
											   
	GrContextFontSet(pContext, LCD_DEBUG_FONT);						  												 											   
	
	GrContextForegroundSet(pContext, ClrDebugCaption);	
		
	if (centered)
	{
	   //nota : aggiunge un delta di 3 pixel perché GrStringDrawCentered posiziona la stringa diversamente
	   //da GrStringDraw
	   GrStringDrawCentered(pContext, text, -1, GrContextDpyWidthGet(pContext) / 2, y+3, 0);
	}
	else {

	   GrStringDraw(pContext, text,-1,1,y,0); 
	} 
	  
	popStyle();
}

//================== LCD_getRowY =======================================

int LCD_getRowY(int row)
{
	return row*LCD_ROW_HEIGHT; 
}

//================= LCD_hilightRow ======================================

void LCD_hilightRow(int row,unsigned long color)
{
	const int y=row*LCD_ROW_HEIGHT;
	tRectangle sRect;
	
	pushStyle();

	sRect.sXMin = 0;
	sRect.sYMin = y;
	sRect.sXMax = GrContextDpyWidthGet(pContext) - 1;
	sRect.sYMax = y+LCD_ROW_HEIGHT;    

	GrContextForegroundSet(pContext, color);
	GrRectFill(pContext, &sRect);
    
	popStyle();

}

//================== LCD_drawBanner =====================================

void LCD_drawBanner(const char* text,unsigned long y,unsigned long border_color,unsigned long backcolor)
{
  tRectangle sRect;

  pushStyle();

  sRect.sXMin=0;
  sRect.sYMin=y;
  sRect.sXMax=GrContextDpyWidthGet(pContext) - 1;
  sRect.sYMax=y+LCD_ROW_HEIGHT*2;

  GrContextForegroundSet(pContext, backcolor);
  GrRectFill(pContext, &sRect);
  GrContextForegroundSet(pContext, border_color); 
  GrRectDraw(pContext, &sRect);
   										   
  GrContextFontSet(pContext, LCD_LARGE_FONT);						  
												 											   
	
  GrContextForegroundSet(pContext, ClrDebugCaption);	
		
	
  //nota : aggiunge un delta di 3 pixel perché GrStringDrawCentered posiziona la stringa diversamente
  //da GrStringDraw
  GrStringDrawCentered(pContext, text, -1, GrContextDpyWidthGet(pContext) / 2, y+8, 0);
	
  popStyle();
    
}

//================== LCD_clear ===========================

void LCD_clear(unsigned long color)
{				   
  tRectangle sRect;
  
  pushStyle();

  GrContextForegroundSet(pContext, color);

  sRect.sXMin=0;
  sRect.sYMin=0;
  sRect.sXMax=GrContextDpyWidthGet(pContext) - 1;
  sRect.sYMax=GrContextDpyHeightGet(pContext) - 1;

  GrRectFill(pContext, &sRect);

  popStyle();
}

void LCD_Flush(void)
{
  	GrFlush(pContext); 
}









