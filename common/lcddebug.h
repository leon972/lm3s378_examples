//*************************************************
//  Modulo utilizzato per l'output di informazioni
//  di debug su un display controllato attraverso
//  la GraphicsLibrary Texas Instruments per microcontrollori
//	Code by Leonardo Berti (c) 2012
//	21-01-2012
//*************************************************

#ifndef __LCD_DEBUG__
#define __LCD_DEBUG__

#ifdef __cplusplus
extern "C"
{
#endif
   
//prototipi funzioni GraphicsLibrary
//per utilizzarla occorre includere
//nel progetto anche il driver effettivo
//per il tipo di display utilizzato 
//(esmepio file formike128x128x16.c nel caso 
//del display presente sulla scheda di valutazione
//EKK-LM3S3738

#include "grlib/grlib.h"

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

/**
  Inizializza il contesto grafico utilizzando il driver definito nell'implementazione
  e visualizza il banner dell'applicazione
  @param banner titolo applicazione visualizzato in alto (se nullo non visualizza il banner)
  @param foreColor colore di primo piano del banner
  @param backColor colore sfondo del display
*/  
void LCD_InitDisplay(const char* banner,unsigned long foreColor,unsigned long backColor);
																						 
/**
  Inizializza il modulo di debug passando il contesto grafico inizializzato altrove
  @param context contesto grafico ottenuto dal display driver
*/
void LCD_debugInit(tContext *context);


/**
	Imposta i colori per l'output dei valori di debug
	@param captionColor colore per le caption (valore RGB)
	@param valueColor colore per i valori
*/
void LCD_debugColors(unsigned long captionColor,unsigned long valueColor);

/**
   Altezza riga per il testo di debug
*/     
unsigned long LCD_getRowHeight(void);

/**
   Restituisce il numero di righe di testo
   del display in base al font di debug utilizzato
*/
unsigned long LCD_getRowsCount(void);

/**
  Scrive sul display un valore boolean preceduto da una descrizione (etichetta)
  @param x valore ascissa del punto di output
  @param y valore ordinata del punto di output
  @param label etichetta
  @param valore
*/
void LCD_writeBool(unsigned long x,unsigned long y,const char* label,int value); 

/**
  Scrive sul display un valore boolean (1 o 0) su una linea specificata
  @param row linea del display , 0=prima linea
*/
void LCD_writeBoolToRow(int row,const char* label,int value);

/**
  Scrive una valore intero preceduto da una etichetta
*/
void LCD_writeIntToRow(int row,const char* label,int value);

/**
  Scrive del testo in una linea specificata
  @param row indice riga
  @param text testo
  @param centered indica se il testo è centrato
*/
void LCD_writeTextToRow(int row,const char* text,int centered);

/**
  Evidenzia una linea con un colore specificato
  @param row indice linea
  @param color colore RGB
*/
void LCD_hilightRow(int row,unsigned long color);


/**
  Scrive un testo all'interno di un banner centrato che occupa 2 righe
*/
void LCD_drawBanner(const char* text,unsigned long y,unsigned long border_color,unsigned long backcolor);

/**
  Acquisisce l'ordinata di una linea
*/
int LCD_getRowY(int row);

/**
  Cancella lo schermo del display LCD
*/
void LCD_clear(unsigned long color);

/**
  flush display
*/
void LCD_Flush(void);

#ifdef __cplusplus
}
#endif

#endif

