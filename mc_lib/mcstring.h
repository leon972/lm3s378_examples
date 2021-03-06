//*************************************************
//  Modulo di manipolazione di stringhe
//  C-like (minilibreria utilizzabile su MCU)
//
//  Code by Leonardo Berti (c) 2012
//  21-01-2012
//*************************************************

#ifndef __MC_STRING__
#define __MC_STRING__

#ifdef __cplusplus
extern "C"
{
#endif

/**
  Calcola la lunghezza di una stringa
*/
unsigned long mc_strlen(const char* text);

/**
* Converte un numero in string utilizzando una base specifica
* Se la stringa di destinazione non è capace di contenere il numero
* da convertire la conversione si interrompe preservando il carattere 0 finale di dest
* @param dest una stringa di lunghezza capace di contenere il numero da convertire
 *@param buff_size dimensione del buffer dest compreso il carattere zero finale
* @return un valore diverso da zero se la conversione è avvenuta con successo,zero in caso
 * contrario (es. raggiunta dimensione massima di dest)
*/
int mc_itoa(char* dest,int value,unsigned long buff_size,unsigned long base);

/**
 * Converte un valore numerico rappresentato come stringa
 * in un intero
 * @param text la stringa da convertire
 * @param base la base in cui è espressa la stringa
 * @param base
 * @return il valore convertito o zero se non convertibile
 */
int mc_atoi(const char* text,unsigned long base);

/**
  Setta a zero tutti i byte di una porzione di memoria
*/
void mc_memclear(void* dest,unsigned long buff_size);

/**
 * Copia la memoria da str2 a str1.La copia avviene per i primi n caratteri senza fermarsi
 * quando trova 0.
 * @param str1 indirizzo destinazione
 * @param str2 indirizzo sorgente
 * @param n numero di caratteri copiati
 * @return l'argomento str1
 */
void *mc_memcpy(void *str1, const void *str2, unsigned long n);

/**
 * Confronta due aree di memoria
 * @param str1 primo indirizzo di memoria
 * @param str2 secondo indirizzo di memoria
 * @param n numero di caratteri da confrontare
 * @return 0 se i primi n caratteri sono uguale, oppure un valore >0 o <0 se *str1>*str2 o *str2>*str1
 */
int mc_memcmp(const void *str1, const void *str2, unsigned long n);

/**
 * Copia il carattere c nei primi n caratteri di str
 * @param str indirizzo di memoria da settare
 * @param c carattere da copiare
 * @param n numero di caratteri di str in cui viene copiato c
 * @return str
 */
void *mc_memset(void *str, int c, unsigned long n);

/**
 * Ricerca la prima occorrenza dal carattere c in str 
 * 
 * @return il riferimento alla posizione in cui viene trovato c o 0 se non trova il carattere
 */
char *mc_strchr(const char *str,int c);

/**
 * Ricerca la prima occorrenza della stringa str2 in str1
 * @param str1 stringa in cui eseguire la ricerca
 * @param str2 stringa cercata
 * @return il riferimento alla prima occorrenza trovata
 */
char *mc_strstr(const char *str1, const char *str2);

/**
 * Converte in maiuscolo
 * @param text
 */
char* mc_ucase(char* text);

/**
 *Converte in minuscolo
 */
char* mc_lcase(char* text);

#ifdef __cplusplus
}
#endif

#endif



