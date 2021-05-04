/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*****************************************************************************
 *   Copyright (C) 2013 Oliver Gemesi                                        *
 *                                                                           *
 *   This program is free software; you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation; either version 2 of the License.          *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program; if not, write to the                           *
 *   Free Software Foundation, Inc.,                                         *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.               *
 *****************************************************************************/

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+ xutils.c - erweiterte String-Funktionen, xprintf (Info nach History)
//+            und weitere Hilfsfunktionen wie z.B. UTCdatetime2local()
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//############################################################################
//# HISTORY  xutils.c
//#
//# 12.04.2014 OG
//# - chg: strncpyat(), strncpyat_P(), _strncpyat() erweitert um Parameter 'sepcharcount'
//#
//# 08.04.2014 OG
//# - add: strncpyat(), strncpyat_P(), _strncpyat()
//#
//# 28.02.2014 OG
//# - add: buffered_sprintf(), buffered_sprintf_P()
//# - add: buffer sprintf_buffer[]
//# - chg: PGMBUFF_SIZE und ARGBUFF_SIZE von 80 auf 40 geaendert
//#
//# 24.06.2013 OG
//# - add: strrtrim() entfernt Leerzeichen auf der rechten Seite
//#
//# 14.05.2013 OG
//# - chg: Kommentarkopf von UTCdatetime2local() aktualisiert
//#
//# 05.05.2013 OG
//# - chg: UTCdatetime2local() auf Config.timezone/summertime umgestellt
//# - add: include eeprom.h
//#
//# 04.05.2013 OG
//# - chg: umbenannt zu xutils.c
//#
//# 03.05.2013 OG
//# - add: UTCdatetime2local()
//# - fix: _xvsnprintf() Darstellung kleiner negativer Nachkommazahlen (-1<z<0)
//#
//# 29.04.2013 OG
//# - chg: Doku zu xprintf Ergaenzt bei 'bekannte Einschraenkungen'
//# - chg: includes reduziert auf das Notwendige
//#
//# 28.04.2013 OG - NEU
//############################################################################


//############################################################################
//# xprintf
//#
//# Diese Variante von printf ist angepasst auf das PKT:
//#
//#  - Unterstuetzung von Festkomma-Integer Anzeige
//#  - Overflow-Anzeige durch '*' wenn eine Zahl die Format-Maske sprengt
//#  - progmen wird optional unterstuetz fuer den format-String als auch fuer
//#     String Argumente
//#  - strikte Einhaltung von Laengen einer Format-Maske
//#    (ggf. wird die Ausgabe gekuerzt)
//#
//# In diesem Source sind nur die Basis-xprintf zum Erzeugen von formatierten
//# Strings. Die direkten Ausgabefunktionen auf den Screen sind in lcd.c
//# als lcdx_printf_at() und lcdx_printf_at_P().
//#
//# FORMAT ANGABEN:
//#
//#   %d: dezimal int signed (Rechtsbuendige Ausgabe)   (Wandlung via itoa)
//#       "%d"    arg: 1234 -> "1234"
//#       "%5d"   arg: 1234 -> " 1234"
//#       "%5.2d" arg: 1234 -> "   12.34"
//#       "%05d"  arg: 123  -> "00123"
//#       "%3.2d" arg: -13  -> " -0.13"
//#
//#   %u: dezimal int unsigned (Rechtsbuendige Ausgabe) (Wandlung via utoa)
//#       wie %d jedoch mittels utoa
//#
//#   %h: hex int unsigned    -> Hex-Zahl     Rechtsbuendig, Laenge wird unterstuetzt z.B. "%4h"
//#   %o: octal int unsigned  -> Octal-Zahl   Rechtsbuendig, Laenge wird unterstuetzt z.B. "%2o"
//#   %b: binary int unsigned -> Binaer-Zahl  Rechtsbuendig, Laenge wird unterstuetzt z.B. "%8b"
//#
//#   %ld, %lu, %lh, %lo, %lb:
//#       wie die obigen jedoch fuer long-zahlen (via ltoa, ultoa)
//#       nur wenn define USE_XPRINTF_LONG gesetzt ist!
//#       Bespiele: "%ld", "%5.6ld" usw.
//#
//#   %s: String aus RAM      -> Linksbuendig, Laenge wird unterstuetzt (z.B. "%8s")
//#   %S: String aus progmem  -> Linksbuendig, Laenge wird unterstuetzt (z.B. "%8s")
//#
//#   %c: einzelnes char      -> Linksbuendig, Laenge wird unterstuetzt (z.B. "%8s")
//#   %%: Ausgabe von "%"
//#
//#
//# BEISPIELE:
//#
//#  vorhanden in: osd/osdata.c, lcd/lcd.c
//#
//#
//# BEKANNTE EINSCHRAENKUNGEN:
//#
//#  1. padding '0' mit negativen Zahlen wenn padding aktiv ist ergibt z.B. "00-1.5"
//#  2. der Entwickler muss auf eine korrekte Format-Maske achten. Andernfalls
//#     kommt es zu nicht vorhersehbaren Ausgaben.
//#  3. nicht in ISR's verwenden - dazu muss ggf. eine Anpassung durchgefuert werden
//#
//# KOMPILIER OPTIONEN
//#
//#  define: USE_XPRINTF_LONG
//#    Unterstuetzung von long int / unsigned long int ('l' Modifier in Maske)
//     Wird in der main.h gesetzt
//############################################################################

#define USE_XPRINTF_LONG  // Unterstuetzung von long integer - Achtung! Muss fuer PKT gesetzt sein
                          // da in Sourcen verwendet!


#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#include "../main.h"
#include "../timer/timer.h"
#include "../eeprom/eeprom.h"


#define PGMBUFF_SIZE 40     // max. 40 chars bei format-String aus progmem
#define ARGBUFF_SIZE 40     // buffer fuer xprintf Parameter (strings, itoa, utoa) (fuer ltoa ggf. anpassen)


#define SPRINTF_BUFFER_SIZE  40                  // max. 40 Chars fuer den Buffer fuer xsnprintf(), xsnprintf_P()
char sprintf_buffer[SPRINTF_BUFFER_SIZE];


//----------------------
// xprintf parser data
//----------------------
typedef struct
{
    uint8_t  parse;     // true / false
    char cmd;           // char: d u h o b s S c %
    uint8_t  prelen;    // Vorkomma
    uint8_t  declen;    // decimal (Nachkomma)
    uint8_t  point;     // true / false
    uint8_t  uselong;   // true / false
    uint8_t  mask;      // true / false
    char pad;           // ' ' oder '0'
} xprintf_t;


//----------------------
// Buffers
//----------------------
char cmddigit[7];
char argbuff[ARGBUFF_SIZE];
char pgmbuff[PGMBUFF_SIZE];


//####################################################################################


//---------------------------------------------------------------------
// Basisfunktion von xprintf
// Doku: siehe oben
//---------------------------------------------------------------------
void _xvsnprintf( uint8_t useprogmem, char *buffer, uint8_t n, const char *format, va_list ap )
{
    const char  *p_fmt;                         // pointer auf den format-String
    char        *p_dst;                         // pointer auf den destination buffer
    const char  *p_str;                         // pointer auf einen arg-String wenn ein String ausgegeben werden soll
    char        *p_cmddigit = 0;                // pointer auf den Digit-Buffer fuer eine Maske (Laenge/Nachkommastellen)
    const char  *p_fmtbuff;                     // pointer auf den format-Buffer (fuer progmem)
    const char  *p_argbuff;                     // pointer auf den argbuffer
    uint8_t     fmtlen, arglen, overflow;
    uint8_t     i,j,dec;
    uint8_t     dstcnt;
    uint8_t     minus;
    xprintf_t   cmd;                            // parser Daten

    p_fmtbuff = format;

    if( useprogmem )                            // format von progmem in's RAM kopieren
    {
        strncpy_P( pgmbuff, format, PGMBUFF_SIZE);
        pgmbuff[PGMBUFF_SIZE-1] = 0;
        p_fmtbuff = pgmbuff;
    }

    cmd.parse = false;
    p_fmt     = p_fmtbuff-1;                    // -1 -> wird am Anfang der Schleife korrigiert
    p_dst     = buffer;
    dstcnt    = 0;

    do
    {
        if( dstcnt >= n )                       // max. Anzahl von Zeichen fuer Ziel 'buffer' erreicht?
            break;

        p_fmt++;                                // naechstes Zeichen von format

        //########################
        //# 1. PARSE
        //########################

        //------------------------
        // START: parse cmd
        //------------------------
        if( cmd.parse == false && *p_fmt == '%' )
        {
            memset( &cmd, 0, sizeof(xprintf_t) );       // init
            cmd.parse  = true;
            cmd.pad    = ' ';
            p_cmddigit = cmddigit;
            continue;
        }

        //------------------------
        // NO parse: copy char
        //------------------------
        if( cmd.parse == false )
        {
            *p_dst = *p_fmt;
            p_dst++;
            dstcnt++;
            continue;
        }

        //------------------------
        // set: pad (eine '0' ganz am Anfang der Formatmaske)
        //------------------------
        if( cmd.parse == true && *p_fmt == '0' && p_cmddigit == cmddigit )
        {
            cmd.pad = '0';
            continue;
        }

        //------------------------
        // set: vor/nach-kommastellen
        //------------------------
        if( cmd.parse == true && *p_fmt >= '0' && *p_fmt <= '9' )
        {
            *p_cmddigit = *p_fmt;
            p_cmddigit++;
            continue;
        }

        //------------------------
        // set: point
        //------------------------
        if( cmd.parse == true && *p_fmt == '.' && cmd.point == false )
        {
            cmd.point   = true;
            *p_cmddigit = 0;
            cmd.prelen  = atoi( cmddigit );
            p_cmddigit  = cmddigit;
            continue;
        }

        //------------------------
        // set: uselong
        //------------------------
        #ifdef  USE_XPRINTF_LONG
        if( cmd.parse == true && *p_fmt == 'l' )
        {
            cmd.uselong  = true;
            continue;
        }
        #endif

        //------------------------
        // END: parse cmd
        //------------------------
        if( cmd.parse == true && (*p_fmt == 'd' || *p_fmt == 'u' || *p_fmt == 'x' || *p_fmt == 'b' || *p_fmt == 'o' ||
                                  *p_fmt == 's' || *p_fmt == 'S' || *p_fmt == 'c' || *p_fmt == '%') )
        {
            cmd.cmd     = *p_fmt;
            cmd.parse   = false;

            *p_cmddigit = 0;
            if( cmd.point == false )    cmd.prelen = atoi(cmddigit);
            else                        cmd.declen = atoi(cmddigit);

            if( cmd.point || cmd.prelen>0 )
                cmd.mask  = true;
        }


        //########################
        //# 2. EXECUTE
        //########################

        //------------------------
        // exec cmd: "d,u,x,b,o"
        //------------------------
        if( cmd.cmd == 'd' || cmd.cmd == 'u' || cmd.cmd == 'x' || cmd.cmd == 'b' || cmd.cmd == 'o' )
        {
            if( cmd.uselong )
            {
#ifdef  USE_XPRINTF_LONG
                switch(cmd.cmd)
                {
                    case 'd': ltoa ( va_arg(ap, long)         , argbuff, 10); break;    // LONG dezimal int signed
                    case 'u': ultoa( va_arg(ap, unsigned long), argbuff, 10); break;    // LONG dezimal int unsigned
                    case 'x': ultoa( va_arg(ap, unsigned long), argbuff, 16); break;    // LONG hex int unsigned
                    case 'b': ultoa( va_arg(ap, unsigned long), argbuff,  2); break;    // LONG binary int unsigned
                    case 'o': ultoa( va_arg(ap, unsigned long), argbuff,  8); break;    // LONG octal int unsigned
                }
#endif
            }
            else
            {
                switch(cmd.cmd)
                {
                    case 'd': itoa( va_arg(ap, int)         , argbuff, 10); break;      // dezimal int signed
                    case 'u': utoa( va_arg(ap, unsigned int), argbuff, 10); break;      // dezimal int unsigned
                    case 'x': utoa( va_arg(ap, unsigned int), argbuff, 16); break;      // hex int unsigned
                    case 'b': utoa( va_arg(ap, unsigned int), argbuff,  2); break;      // binary int unsigned
                    case 'o': utoa( va_arg(ap, unsigned int), argbuff,  8); break;      // octal int unsigned
                }
            }

            minus    = (argbuff[0] == '-');
            arglen   = strlen(argbuff);

            fmtlen   = cmd.prelen + cmd.declen + (cmd.point ? 1 : 0);
            arglen   = strlen(argbuff);

            overflow = cmd.mask &&                                                      // Zahl zu gross -> "*" anzeigen statt der Zahl
                         (arglen > cmd.prelen + cmd.declen || cmd.prelen < 1+minus);

            if( overflow )                                                              // overflow: Zahl passt nicht in Maske
            {                                                                           //   -> zeige '*.*'
                for( i=0; (i < fmtlen) && (dstcnt < n); i++)
                {
                    if( cmd.point && i==cmd.prelen )    *p_dst = '.';
                    else                                *p_dst = '*';
                    p_dst++;
                    dstcnt++;
                }
            }
            else                                                                        // else: if( overflow )
            {
                if( !cmd.mask )                                                         // keine Maske: alles von der Zahl ausgeben
                    fmtlen = arglen;

                p_argbuff = argbuff;
                if( minus )                                                             // wenn Zahl negativ: merken und auf dem argbuff nehmen
                {
                    p_argbuff++;
                    arglen--;
                }

                //-----------------
                // die Zahl wird 'Rueckwaerts' uebertragen
                //-----------------
                dec = -1;                                                               // wird am Anfang der Schleife auf 0 gesetzt
                j   = 1;                                                                // zaehler fuer argbuff
                for( i=1; i<=fmtlen; i++ )
                {
                    dec++;                                                              // Zaehler Dizmalstellen

                    if( dstcnt+fmtlen-i <= n )                                          // wenn Zielbuffer nicht ueberschritten
                    {
                        if( cmd.point && (dec == cmd.declen) )                          // Dezimalpunkt setzen
                        {
                            p_dst[fmtlen-i] = '.';
                            continue;
                        }

                        if( j <= arglen )                                               // Ziffer uebertragen aus argbuff
                        {
                            p_dst[fmtlen-i] = p_argbuff[arglen-j];
                            j++;
                            continue;
                        }

                        if( cmd.declen > 0 &&                                           // Nachkomma und 1. Vorkommastelle ggf. auf '0'
                              (dec < cmd.declen || dec == cmd.declen+1) )               //   setzen wenn die Zahl zu klein ist
                        {
                            p_dst[fmtlen-i] = '0';
                            continue;
                        }

                        if( minus && ( (cmd.pad == ' ') ||                              // ggf. Minuszeichen setzen
                                       (cmd.pad != ' ' && i == fmtlen) ) )              //  Minuszeichen bei '0'-padding an erster Stelle setzen
                        {
                            minus = false;
                            p_dst[fmtlen-i] = '-';
                            continue;
                        }

                        p_dst[fmtlen-i] = cmd.pad;                                      // padding ' ' oder '0'
                    }                                                                   // end: if( dstcnt+fmtlen-i <= n )
                }
                p_dst  += fmtlen;
                dstcnt += fmtlen;
            }
            continue;
        }

        //------------------------
        // exec cmd: "s", "S", "c"
        //------------------------
        if( cmd.cmd == 's' || cmd.cmd == 'S' || cmd.cmd == 'c' )
        {
            switch(cmd.cmd)
            {
                case 's':   p_str = va_arg( ap, char *);                                // String aus dem RAM
                            break;

                case 'S':   strncpy_P( argbuff, va_arg( ap, char *), ARGBUFF_SIZE);     // String liegt im progmem -> in's RAM kopieren
                            argbuff[ARGBUFF_SIZE-1] = 0;
                            p_str = argbuff;
                            break;

                case 'c':   argbuff[0] = va_arg( ap, int);                              // einzelnes char
                            argbuff[1] = 0;
                            p_str = argbuff;
                            break;
            }

            fmtlen = cmd.prelen;
            arglen = strlen(p_str);

            if( !cmd.mask )                                     // keine Maske: alles vom String ausgeben
                fmtlen = arglen;

            for( i=0; i<fmtlen; i++)
            {
                if( dstcnt < n )                                // wenn Zielbuffer nicht ueberschritten
                {
                    if( *p_str )                                // char uebertragen
                    {
                        *p_dst = *p_str;
                        p_str++;
                    }
                    else                                        // padding
                    {
                        *p_dst = ' ';
                    }
                    p_dst++;
                }
                dstcnt++;
            }

            continue;
        }

        //------------------------
        // exec cmd: "%"
        //------------------------
        if( cmd.cmd == '%' )
        {
            *p_dst = '%';
            p_dst++;
            dstcnt++;
            continue;
        }

    } while( (dstcnt < n) && *p_fmt );

    *(p_dst + (dstcnt < n ? 0 : -1)) = 0;       // terminierende 0 im Ausgabebuffer setzen
}



//---------------------------------------------------------------------
//---------------------------------------------------------------------
void xsnprintf( char *buffer, uint8_t n, const char *format, ... )
{
    va_list ap;

    va_start(ap, format);
    _xvsnprintf( false, buffer, n, format, ap);
    va_end(ap);
}


//---------------------------------------------------------------------
//---------------------------------------------------------------------
void xsnprintf_P( char *buffer, uint8_t n, const char *format, ... )
{
    va_list ap;

    va_start(ap, format);
    _xvsnprintf( true, buffer, n, format, ap);
    va_end(ap);
}



//-----------------------------------------------------------
// buffered_sprintf_P( format, ...)
//
// Ausgabe direkt in einen internen Buffer.
// Der Pointer auf den RAM-Buffer wird zurueckgegeben.
// Abgesichert bzgl. Buffer-Overflow.
//
// Groesse des Buffers: PRINTF_BUFFER_SIZE
//
// Parameter:
//  format     : String aus PROGMEM (siehe: xprintf in utils/xstring.h)
//  ...        : Parameter fuer 'format'
//-----------------------------------------------------------
char * buffered_sprintf( const char *format, ... )
{
    va_list ap;

    va_start( ap, format );
    _xvsnprintf( false, sprintf_buffer, SPRINTF_BUFFER_SIZE, format, ap );
    va_end(ap);
    return sprintf_buffer;
}



//-----------------------------------------------------------
// buffered_sprintf_P( format, ...)
//
// Ausgabe direkt in einen internen Buffer.
// Der Pointer auf den RAM-Buffer wird zurueckgegeben.
// Abgesichert bzgl. Buffer-Overflow.
//
// Groesse des Buffers: PRINTF_BUFFER_SIZE
//
// Parameter:
//  format     : String aus PROGMEM (siehe: xprintf in utils/xstring.h)
//  ...        : Parameter fuer 'format'
//-----------------------------------------------------------
char * buffered_sprintf_P( const char *format, ... )
{
    va_list ap;

    va_start( ap, format );
    _xvsnprintf( true, sprintf_buffer, SPRINTF_BUFFER_SIZE, format, ap );
    va_end(ap);
    return sprintf_buffer;
}



//--------------------------------------------------------------
// kopiert einen String von src auf dst mit fester Laenge und
// ggf. Space paddings rechts
//
// - fuellt ggf. den dst-String auf size Laenge mit Spaces
// - setzt Terminierung's 0 bei dst auf Position size
//--------------------------------------------------------------
void strncpyfill( char *dst, const char *src, size_t size)
{
    uint8_t i;
    uint8_t pad = false;

    for( i=0; i<size; i++)
    {
        if(*src == 0) pad = true;

        if( pad )   *dst = ' ';
        else        *dst = *src;

        src++;
        dst++;
    }
    dst--;
    *dst = 0;
}



//--------------------------------------------------------------
// entfernt rechte Leerzeichen aus einem String
//--------------------------------------------------------------
void strrtrim( char *dst)
{
    char    *p;

    p = dst + strlen(dst) - 1;

    while( (p != dst) && (*p == ' ') ) p--;

    if( (*p != ' ') && (*p != 0) ) p++;
    *p = 0;
}



//--------------------------------------------------------------
// INTERN - fuer strncpyat(), strncpyat_P()
//--------------------------------------------------------------
void _strncpyat( char *dst, const char *src, size_t size, const char sepchar, uint8_t sepcharcount, uint8_t progmem)
{
    uint8_t i;

    if( progmem )
        strncpy_P( dst, src, size);
    else
        strncpy( dst, src, size);

    for( i=0; i<size; i++)
    {
        if( *dst == 0) return;
        if( *dst == sepchar)
        {
            sepcharcount--;
            if( sepcharcount==0 )
            {
                *dst = 0;
                return;
            }
        }
        dst++;
    }
    dst--;
    *dst = 0;
}


//--------------------------------------------------------------
// strncpyat( dst, src, size, sepchar)
//
// kopiert einen String von 'src 'auf 'dst' mit max. Laenge 'size'
// oder bis 'sepchar' gefunden wird.
//
// src in PROGMEM
//--------------------------------------------------------------
void strncpyat( char *dst, const char *src, size_t size, const char sepchar, uint8_t sepcharcount)
{
    _strncpyat( dst, src, size, sepchar, sepcharcount, false);
}


//--------------------------------------------------------------
// strncpyat_P( dst, src, size, sepchar)
//
// kopiert einen String von 'src 'auf 'dst' mit max. Laenge 'size'
// oder bis 'sepchar' gefunden wird.
//
// src in RAM
//--------------------------------------------------------------
void strncpyat_P( char *dst, const char *src, size_t size, const char sepchar, uint8_t sepcharcount)
{
    _strncpyat( dst, src, size, sepchar, sepcharcount, true);
}



//--------------------------------------------------------------
// UTCdatetime2local( PKTdatetime_t *dtbuffer, PKTdatetime_t dt )
//
// konvertiert die UTC-Time 'dt' in die lokale Zeit und speichert
// dieses in 'dtbuffer' ab.
//
// Parameter:
//
//  dtdst: Pointer Destination (PKTdatetime_t) (Speicher muss alloziiert sein!)
//  dtsrc: Pointer Source (PKTdatetime_t)
//
// Hinweise:
//
//  Schaltjahre (bzw. der 29.02.) werden nicht unterstuetzt
//--------------------------------------------------------------
void UTCdatetime2local( PKTdatetime_t *dtdst, PKTdatetime_t *dtsrc )
{
    int8_t  timeoffset;
    int32_t v;
    int8_t  diff;
    //                    01 02 03 04 05 06 07 08 09 10 11 12    Monat
    int8_t  daymonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};

    //--------------------------
    // timezone: Einstellbereich -12 .. 0 .. 12 (+1 = Berlin)
    // summertime: Einstellung: 0 oder 1 (0=Winterzeit, 1=Sommerzeit)
    //--------------------------
    timeoffset = Config.timezone + Config.summertime;
    //timeoffset = 2;                                   // solange noch nicht in PKT-Config: Berlin, Sommerzeit


    memcpy( dtdst, dtsrc, sizeof(PKTdatetime_t) );      // copy datetime to destination

    //--------------------------
    // Zeitzonenanpassung
    //--------------------------
    if( dtdst->year != 0 && dtdst->month >= 1 && dtdst->month <= 12 )   // nur wenn gueltiges Datum vorhanden
    {
        //--------------------------
        // 1. Sekunden
        //--------------------------
        v = (int32_t)dtdst->seconds;
        v += timeoffset*3600;                           // Stunden korrigieren
        diff = 0;

        if( v > 86400 )                                 // Tagesueberschreitung?    (86400 = 24*60*60 bzw. 24 Stunden)
        {
            v -= 86400;
            diff++;                                     //  inc: Tag
        }
        else if( v < 0 )                                // Tagesunterschreitung?
        {
            v += 86400;
            diff--;                                     //  dec: Tag
        }
        dtdst->seconds = (uint32_t)v;                   // SET: seconds

        //--------------------------
        // 2. Tag
        //--------------------------
        v = (int32_t)dtdst->day;
        v += diff;
        diff = 0;

        if( v > daymonth[dtdst->month-1] )              // Monatsueberschreitung?
        {
            v = 1;                                      // erster Tag des Monats
            diff++;                                     //  inc: Monat
        }
        else if( v < 1 )                                // Monatsunterschreitung?
        {
            if( dtdst->month > 1 )
                v = daymonth[dtdst->month-1-1];         // letzter Tag des vorherigen Monats
            else
                v = 31;                                 // letzter Tag im Dezember des vorherigen Jahres
            diff--;                                     //   dec: Monat
        }
        dtdst->day = (uint8_t)v;                        // SET: day

        //--------------------------
        // 3. Monat
        //--------------------------
        v = (int32_t)dtdst->month;
        v += diff;
        diff = 0;

        if( v > 12 )                                    // Jahresueberschreitung?
        {
            v = 1;
            diff++;                                     //  inc: Jahr
        }
        else if( v < 1 )                                // Jahresunterschreitung?
        {
            v = 12;
            diff--;                                     //  dec: Jahr
        }
        dtdst->month = (uint8_t)v;                      // SET: month

        //--------------------------
        // 4. Jahr
        //--------------------------
        dtdst->year += diff;                            // SET: year
    }
}
