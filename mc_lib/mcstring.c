//*************************************************
//  Modulo di manipolazione di stringhe
//  C-like  
//
//  Code by Leonardo Berti (c) 2012
//  21-01-2012
//*************************************************

#include "mcstring.h"

static const char* const g_digits = "0123456789ABCDEF";

//==============  mc_strlen =======================

unsigned long mc_strlen(const char* text) {
    char* p = (char*) text;

    int l = 0;

    if (!text) return 0;

    while (*p) {
        ++l;
        p++;
    }

    return l;
}

//============== mc_itoa ==========================

int mc_itoa(char* dest, int value, unsigned long buff_size, unsigned long base) {

    int r = value;
    unsigned long offs = 0;
    unsigned long offs2;
    unsigned long offs3;
    char temp;
    int c;
    int start = 0;

    if (!dest) return 0;

    if (base > 16) return 0;

    --buff_size; //diminuisce di 1 la dimensione del buffer per preservare il carattere nullo alla fine

    if (r < 0) {
        dest[offs++] = '-';
        r = -r;
        if (offs >= buff_size) return 0;
        start = 1;
    }

    while (1) {
        c = r % base;

        r /= base;

        dest[offs++] = g_digits[c];

        if (offs >= buff_size) {
            for (c = 0; c < buff_size; c++) {
                dest[c] = '?';
            }
            return 0;
        }

        if (r == 0) break;
    }

    offs2 = (offs - start) / 2;

    //inverte la stringa
    for (c = 0; c < offs2; c++) {
        offs3 = c + start;
        temp = dest[offs3];
        dest[offs3] = dest[offs - c - 1];
        dest[offs - c - 1] = temp;
    }

    return 1;

}

//=============== mc_atoi =============================

int mc_atoi(const char* text, unsigned long base) {

    int r = 0;
    int d;
    int ng = 1;
    int limit=0;
    int limit1=0;
    char *p = (char*) text;
    char *pmsb = (char*) text;
    char c;

    if (base > 16) return 0;

    if (base > 10) {
        limit = 'A' + base - 10;
        limit1 = 'a' + base - 10;
    }

    //trim
    while (*p == ' ' || *p=='\t') {
        ++p;
    }

    if (*p == '-') {
        ng = -1;
        ++p;
    }

    pmsb = p;

    //trova la cifra meno significativa
    while (1) {
        c = *p;

        if (c == 0) break;

        if (base > 10) {
            if (!((c >= '0' && c <= '9') || (c >= 'A' && c < limit) || (c >= 'a' && c < limit1))) break;
        } else {
            if (!(c >= '0' && c <= '9')) break;
        }
        ++p;
    }

    --p;

    d = 1;

    if (base > 10) {

        while (p >= pmsb) {

            c = *p;

            if (c >= 'A' && c < limit) {
                r += (c - 'A' + 10) * d;
            } else if (c >= 'a' && c < limit1) {
                r += (c - 'a' + 10) * d;
            } else {
                r += (c - '0') * d;
            }
            d *= base;
            --p;
        }

    } else {

        while (p >= pmsb) {
            c = *p;
            r += (c - '0') * d;
            d *= base;
            --p;
        }
    }

    if (ng < 0) r = -r;

    return r;

}

//===============mc_mem_clear =========================

void mc_memclear(void* dest, unsigned long buff_size) {
    unsigned long i;
    unsigned char* p = (unsigned char*) dest;

    for (i = 0; i < buff_size; i++) {
        *(p) = 0x0;
        ++p;
    }
}

//=================== mc_memcpy =======================

void *mc_memcpy(void *str1, const void *str2, unsigned long n) {
    unsigned long i;

    unsigned char* p = (unsigned char*) str1;
    const unsigned char *ps = (unsigned char*) str2;

    for (i = 0; i < n; i++) {
        *p = *ps;
        ++p;
        ++ps;
    }

    return str1;

}

//=============== mc_memchr =============================

char *mc_strchr(const char *str, int c) {
    
    char *p;

    if (!str) return 0;

    p = (char*) str;

    while (*p) {
        if (*p == c) return p;
        ++p;

    }

    return 0; //non trovato
}

//=============== mc_strstr ==============================

char *mc_strstr(const char *str1, const char *str2) {

    
    char *p = (char*) str1;
    char *p1,*p2;

    if (!str1 || !str2) return 0;

    while (*p) {

        p1=p;
        p2=(char*)str2;

        while(*p1==*p2) {

            ++p1;
            ++p2;

            if (*p2==0) {
                return p;
            }

            if (*p1==0) break;            
        }

        ++p;
    }

    return 0; //non trovato
}

//==================== mc_memcmp =====================

int mc_memcmp(const void *str1, const void *str2, unsigned long n) {
    unsigned long i;

    unsigned char *p1 = (unsigned char*) str1;
    unsigned char *p2 = (unsigned char*) str2;

    for (i = 0; i < n; i++) {
        if (*p1 != *p2) {
            return (*p1 - *p2);
        }

        ++p1;
        ++p2;
    }

    return 0;
}

//====================== mc_memset ======================

void *mc_memset(void *str, int c, unsigned long n) {

    unsigned long i;

    unsigned char* p = (unsigned char*) str;

    for (i = 0; i < n; i++) {
        p[i] = c;
    }

    return str;
}

//======================= mc_ucase ========================

char* mc_ucase(char* text)
{
    char *p=text;
    int d='a'-'A';

    while (*p) {

        char c=*p;

        if (c>='a' && c<='z') {
            *p=(char)(c-d);
        }

        ++p;
    }

    return text;
}

//======================== mc_lcase =======================

char* mc_lcase(char* text)
{
    char *p=text;
    int d='a'-'A';

    while (*p) {

        char c=*p;

        if (c>='A' && c<='Z') {
            *p=(char)(c+d);
        }

        ++p;
    }

    return text;   				
}




