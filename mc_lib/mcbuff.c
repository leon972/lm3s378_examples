//*************************************************
//  Modulo di manipolazione di stringhe
//  C-like (minilibreria utilizzabile su MCU)
//  (String Builder)
//  Code by Leonardo Berti (c) 2012
//  24-01-2012
//*************************************************

#include "mcstring.h"
#include "mcbuff.h"

#define TEMP_SIZE 16

static char temp[TEMP_SIZE];

/**
 * Imposta la lunghezza di un buffer
 * @param buff buffer
 * @param new_len nuova lunghezza (<=buff_size)
 * @param buff_size dimensione del buffer compreso il carattere finale 0
 * @return
 */
void mc_buff_set_len(char* buff, unsigned long new_len, unsigned long buff_size) {

    int i;

    if (new_len > buff_size - 1) new_len = buff_size - 1;

    for (i = new_len; i < buff_size; i++) {
        buff[i] = 0x0;
    }

}

/**
 *Aggiunge al buffer buff il testo text e imposta il nuovo carattere finale
 */
void mc_buff_append(char* buff, const char* text, unsigned long buff_size) {

    unsigned long ul;
    unsigned long bl;
    unsigned long limit;
    int i;
    char *p = (char*)text;

    if (text == 0) return;

    ul = mc_strlen(text);
    bl = mc_strlen(buff);
    limit = bl + ul;
    if (limit > buff_size - 1) limit = buff_size - 1;

    for (i = bl; i < limit; i++) {
        buff[i] = *p;
        p++;
    }

    buff[i] = 0x0;
}

/**
 * Aggiunge al buffer un carattere
 * @param buff
 * @param i
 * @param buff_size
 */
void mc_buff_cappend(char* buff, char c, unsigned long buff_size) {

    unsigned long bl;

    bl = mc_strlen(buff);

    if ((bl + 1) >= buff_size - 1) return;

    buff[bl]=c;
    buff[bl+1]=0x0;
}

/**
 * Aggiunge al buffer un intero
 * Le dimensioni del buffer non vengono mai superate ed è preservato il carattere zero finale
 * @param buff buffer destinazione
 * @param i
 * @return
 */
void mc_buff_iappend(char* buff, int i, unsigned long buff_size) {

    mc_memclear(temp, TEMP_SIZE);
    mc_itoa(temp, i, TEMP_SIZE, 10);
    mc_buff_append(buff, temp, buff_size);

}

void mc_buff_ibappend(char* buff, int i,unsigned long base, unsigned long buff_size) {
    mc_memclear(temp, TEMP_SIZE);
    mc_itoa(temp, i, TEMP_SIZE, base);
    mc_buff_append(buff, temp, buff_size);
}


