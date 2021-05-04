/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/**
 * a simple Fifo
 * @file fifo.c
 * @author Pascal Schnurr
*/

//############################################################################
//# HISTORY  fifo.c
//#
//# 26.06.2014 OG
//# - add: fifo_isstr()
//#
//# 16.04.2013 Cebra
//# - chg: PROGMEM angepasst auf neue avr-libc und GCC, prog_char depreciated
//#
//############################################################################


#include <string.h>
#include "fifo.h"
//#if defined HWVERSION1_3W || defined HWVERSION3_9 || defined HWVERSION1_2W


//--------------------------------------------------------------
void fifo_init (fifo_t * fifo, uint8_t * buffer, uint16_t size)
{
    fifo->size = size;
    fifo->buffer = buffer;
    fifo->head = 0;
    fifo->count = 0;
}
//--------------------------------------------------------------
uint16_t fifo_getcount (const fifo_t * fifo)
{

      return  fifo->count;
}
//--------------------------------------------------------------
bool fifo_is_empty (const fifo_t * fifo)
{
    return (fifo->count == 0);
}

//--------------------------------------------------------------
bool fifo_is_full (const fifo_t * fifo)
{
    return (fifo->size - fifo->count == 0);
}

//--------------------------------------------------------------
bool fifo_read (fifo_t * fifo, char *data)
{
    if (fifo_is_empty (fifo))
        return false;

    uint8_t *head = fifo->buffer + fifo->head;

    *data = (char) * head;

    fifo->head = ( (fifo->head + 1) % fifo->size);

    fifo->count--;

    return true;
}

//--------------------------------------------------------------
bool fifo_write (fifo_t * fifo, const char data)
{
    if (fifo_is_full (fifo))
        return false;

    uint8_t *end = fifo->buffer + ( (fifo->head + fifo->count) % fifo->size);

    *end = (uint8_t) data;

    fifo->count++;

    return true;
}

//--------------------------------------------------------------
bool fifo_clear (fifo_t * fifo)
{
    fifo->count = 0;
    fifo->head = 0;
    return true;
}

//--------------------------------------------------------------
static bool fifo_cmp_pgm_at (fifo_t * fifo, const char * pgm, const uint16_t index)
{
    uint16_t i;
    uint8_t fifo_byte;
    uint8_t pgm_byte;

    for (i = 0; pgm_read_byte (pgm + i) != 0; i++)
    {
        if (fifo->count <= (i + index))
            return false;

        pgm_byte = pgm_read_byte (pgm + i);

        fifo_byte = * (fifo->buffer + ( (fifo->head + i + index) % fifo->size));

        if (fifo_byte != pgm_byte)
            return false;
    }

    // We found the string, lets move the pointer
    fifo->head = ( (fifo->head + i + index) % fifo->size);

    fifo->count -= (i + index);

    return true;
}
//--------------------------------------------------------------
bool fifo_search (fifo_t * fifo, const char * pgm)
{
        uint16_t i;
        uint8_t fifo_byte;
        uint8_t pgm_byte;

        for (i = 0; pgm_read_byte (pgm + i) != 0; i++)
        {
                if (fifo->count <= i)
                        return false;

                pgm_byte = pgm_read_byte (pgm + i);

                fifo_byte = * (fifo->buffer + ( (fifo->head + i) % fifo->size));

                if (fifo_byte != pgm_byte)
                        return false;
        }

//        // We found the string, lets move the pointer
//        fifo->head = ( (fifo->head + i + index) % fifo->size);
//
//        fifo->count -= (i + index);

        return true;
}
//--------------------------------------------------------------
/** \brief searches a string in the whole fifo
 * starts at the beginning and searches for the pgm string in the fifo,
 *
 * @param fifo pointer to your initialized fifo_t structure
 * @param pgm a prog_char with the search string
 * @return true if found, false otherwise
 */

bool fifo_cmp_pgm (fifo_t * fifo, const char * pgm)
{
    return fifo_cmp_pgm_at (fifo, pgm, 0);
}
//--------------------------------------------------------------
/** \brief searches a string in the whole fifo
* starts at the beginning and searches for the pgm string in the fifo,
* if they are found previous entrys and the string are removed from the fifo
* @param fifo pointer to your initialized fifo_t structure
* @param pgm a prog_char with the search string
* @return true if found, false otherwise
*/

bool fifo_strstr_pgm (fifo_t * fifo, const char * pgm)
{
    for (uint16_t i = 0; i < fifo->count; i++)
    {
        if (fifo_cmp_pgm_at (fifo, pgm, i))
            return true;
    }

    return false;
}



//--------------------------------------------------------------
// Idee: buffer nach str durchsuchen ohne die fifo Zeiger zu aendern
// ungetestet
//--------------------------------------------------------------
bool fifo_isstr( fifo_t * fifo, const char * str)
{
    /*
    void *memmem(const void *s1,
                 size_t len1,
                 const void *s2,
                 size_t len2);
    The memmem() function finds the start of the first occurrence of the substring s2 of length len2 in the memory area s1 of length len1.
    */

    return memmem( fifo->buffer + fifo->head, fifo->count, str, strlen(str) ) != NULL;
}


//#endif
