//*************************************************
//  Modulo di manipolazione di stringhe
//  C-like (minilibreria utilizzabile su MCU)
//  (String Builder)
//  Code by Leonardo Berti (c) 2012
//  24-01-2012
//*************************************************

#ifndef MCBUFF_H
#define	MCBUFF_H

#ifdef	__cplusplus
extern "C" {
#endif

    /**
     * Imposta la lunghezza di un buffer
     * @param buff buffer
     * @param new_len nuova lunghezza (<=buff_size)
     * @param buff_size dimensione del buffer compreso il carattere finale 0
     * @return
     */
    void mc_buff_set_len(char* buff, unsigned long new_len, unsigned long buff_size);

    /**
     *Aggiunge al buffer buff il testo text e imposta il nuovo carattere finale
     */
    void mc_buff_append(char* buff, const char* text, unsigned long buff_size);

    /**
     * Aggiunge al buffer un intero
     * Le dimensioni del buffer non vengono mai superate ed è preservato il carattere zero finale
     * @param buff buffer destinazione
     * @param i
     * @return
     */
    void mc_buff_iappend(char* buff, int i, unsigned long buff_size);

    /**
     * Aggiunge al buffer un intero specificando la base
     * @param buff
     * @param i
     * @param base
     * @param buff_size
     */
    void mc_buff_ibappend(char* buff, int i,unsigned long base, unsigned long buff_size);

    /**
     * Aggiunge al buffer un carattere
     * @param buff
     * @param i
     * @param buff_size
     */
    void mc_buff_cappend(char* buff, char c, unsigned long buff_size);

#ifdef	__cplusplus
}
#endif

#endif	/* MCBUFF_H */

