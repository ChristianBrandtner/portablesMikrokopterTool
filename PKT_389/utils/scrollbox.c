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

//############################################################################
//# HISTORY  scrollbox.c
//#
//# 31.05.2014 OG
//# - chg: ScrollBox_Refresh() - umgestellt auf PKT_KeylineUpDown()
//#
//# 29.03.2014 OG
//# - chg: ScrollBox_Show() umgestellt auf clear_key_all()
//#
//# 22.05.2013 OG
//# - fix: include pkt/pkt.h
//#
//# 19.05.2013 OG
//# - chg: ScrollBox_Show() erweitert um PKT_CtrlHook() um u.a. PKT-Updates
//#        zu ermoeglichen
//#
//# 04.05.2013 OG
//# - chg: angepasst auf xutils.c
//#
//# 28.04.2013 OG
//# - add: ScrollBox_Push() - Variante fuer 'format' im RAM
//# - chg: ScrollBox_Push_P() -> keine Rueckgabe mehr (void)
//# - chg: auf xprintf umgestellt (siehe utils/xstring.c)
//# - add: verschiedene Beschreibungen ueber Funktionen
//#
//# 20.04.2013 OG - NEU
//############################################################################

//#define USE_SCROLLBOX_DEBUG       // Debug-Funktionen einkompilieren/aktivieren (gesendet wird an uart1)

#include "../cpu.h"
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "../main.h"
#include "../lcd/lcd.h"
#include "../eeprom/eeprom.h"
#include "../messages.h"
#include "../pkt/pkt.h"
#include "../utils/xutils.h"        // xprintf
#include "scrollbox.h"

// Debug
#ifdef USE_SCROLLBOX_DEBUG
  #include "../uart/uart1.h"
  #include <util/delay.h>
#endif

#define SCROLLBOX_ALLOC_MEM         // dynamische RAM-Belegen (malloc) verwenden?   (Standardeinstellung: ja)

#define SCROLLBOX_LINESIZE  20+1    // Ausgabezeile der ScrollBox (20 Chars + 0x00)
#define SCROLLBOX_KEYSIZE   4+1     // Textlabel einer Taste (4 Chars + 0x00)

#define SCROLLBOX_W         20      // Textbreite
#define SCROLLBOX_H         7       // Textzeilen
#define MSBLINE             10      // drawmode-code fuer: Linie




//-----------------------------------------------------------
// statischer Speicher wenn nicht SCROLLBOX_ALLOC_MEM
// verwendet wird
//-----------------------------------------------------------
#ifndef SCROLLBOX_ALLOC_MEM
    #define SCROLLBOX_BUFFER_SIZE                   2048    // 2 KByte - ram-size in bytes
    char scrollbox_buffer[SCROLLBOX_BUFFER_SIZE];
#endif


//-----------------------------------------------------------
// typedef: scrollbox_key_t
//-----------------------------------------------------------
typedef struct
{
    uint8_t     active;                         // Taste aktiv? (true/false)
    char        text[SCROLLBOX_KEYSIZE];        // Tastentext
}  scrollbox_key_t;


//-----------------------------------------------------------
// typedef: scrollbox_line_t
//-----------------------------------------------------------
typedef struct
{
    uint8_t     mode;                           // drawmode: MNORMAL, MINVERSE oder MSBLINE
    char        line[SCROLLBOX_LINESIZE];       // malloc: lines * 21 bytes (20 chars + 0)
}  scrollbox_line_t;


//-----------------------------------------------------------
// typedef: scrollbox_t
//-----------------------------------------------------------
typedef struct
{
    scrollbox_line_t    *buffer;                // malloc: lines * 22 bytes (21 chars + 0)
    uint8_t             maxlines;               // max. reservierte Lines (malloc buffer)
    uint8_t             lines;                  // Anzahl gepushte lines
    uint8_t             display_pos;            // aktuelle obere Anzeigezeile
    scrollbox_key_t     key_enter;
    scrollbox_key_t     key_enter_long;
}  scrollbox_t;


//-----------------------------------------------------------
// Buffer & Co.
//-----------------------------------------------------------
scrollbox_t     scrollbox;
char            buffer_sbline[SCROLLBOX_LINESIZE];


//#############################################################################################
//#############################################################################################



//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
#ifdef USE_SCROLLBOX_DEBUG
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//--------------------------------------------------------------
// nur wenn define: USE_SCROLLBOX_DEBUG
//--------------------------------------------------------------
void ScrollBox_Debug( void )
{
    char s[33];
    uint8_t i,j;
    char *p;
    scrollbox_line_t *sboxp;
    uint32_t h;

    uart1_puts_p( PSTR("\r\nScrollBox_Debug: BEGIN\r\n\r\n") );

    ltoa( scrollbox.buffer, s, 16);
    uart1_puts_p( PSTR("address: buffer          = 0x") );
    uart1_puts( s );
    ltoa( scrollbox.buffer, s, 10);
    uart1_puts( " (" );
    uart1_puts( s );
    uart1_puts( ")\r\n" );

    ltoa( scrollbox_buffer, s, 16);
    uart1_puts_p( PSTR("address: scrollbox_buffer  = 0x") );
    uart1_puts( s );
    ltoa( scrollbox.buffer, s, 10);
    uart1_puts( " (" );
    uart1_puts( s );
    uart1_puts( ")\r\n" );


    itoa( sizeof(scrollbox_line_t), s, 10);
    uart1_puts_p( PSTR("sizeof(scrollbox_line_t) = ") );
    uart1_puts( s );
    uart1_puts( "\r\n" );

    uart1_puts( "\r\n" );

    itoa( scrollbox.lines, s, 10);
    uart1_puts_p( PSTR("scrollbox.lines = ") );
    uart1_puts( s );
    uart1_puts( "\r\n" );

    itoa( scrollbox.maxlines, s, 10);
    uart1_puts_p( PSTR("scrollbox.maxlines = ") );
    uart1_puts( s );
    uart1_puts( "\r\n" );

    itoa( scrollbox.display_pos, s, 10);
    uart1_puts_p( PSTR("scrollbox.display_pos = ") );
    uart1_puts( s );
    uart1_puts( "\r\n" );

    uart1_puts( "\r\n" );

    for(i=0; i<scrollbox.lines; i++)
    {
        itoa( i, s, 10);
        if( strlen(s)<2 ) uart1_puts( " " );
        uart1_puts( s );
        uart1_puts( ": " );

        //sboxp = scrollbox.buffer + sizeof(scrollbox_line_t)*i;
        sboxp = scrollbox.buffer + i;

        ltoa( sboxp, s, 16); // hex
        uart1_puts( "[0x" );
        uart1_puts( s );
        uart1_puts( "]" );

        ltoa( sboxp, s, 10); // dec
        uart1_puts( "[" );
        uart1_puts( s );
        uart1_puts( "] " );

        itoa( sboxp->mode, s, 10);
        if( strlen(s)<2 ) uart1_puts( " " );
        uart1_puts( s );
        uart1_puts( ": " );

        p = sboxp->line;

        if( sboxp->mode != 10 )
        {
            uart1_puts( p );
        }

        uart1_puts( "|\r\n" );
    }

    //--------------------------------------------------------------
    // Hier wird fuer eine gewisse Zeit (insgesamt 2500 mal) permanent die Zeile 23 ausgegeben.
    // Wenn ich das richtig sehe schreibe ich hierbei nicht ;-)
    // Aber was sagt das Empfangsterminal....
    //--------------------------------------------------------------
    sboxp = scrollbox.buffer + 23;
    for( h=0; h<10; h++)
    {
        itoa( h, s, 10);
        uart1_puts( s );
        uart1_puts( "# " );

        uart1_puts( sboxp->line );
        uart1_puts( "|\r\n" );
        _delay_ms(10);
    }
    //--------------------------------------------------------------

    uart1_puts_p( PSTR("\r\nScrollBox_Debug: END\r\n") );
    uart1_puts( "##########################################\r\n\r\n" );
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
#endif // USER_SCROLLBOX_DEBUG
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx



//--------------------------------------------------------------
// ok = ScrollBox_Create( uint8_t maxlines )
//
// Rueckgabe:
//   true  = ok
//   false = kein Speicher
//
// - reserviert speicher fuer max. maxlines
// - initialisiert scrollbox-Datenstruktur
//--------------------------------------------------------------
uint8_t ScrollBox_Create( uint8_t maxlines )
{
    scrollbox.buffer = 0;

    #ifdef SCROLLBOX_ALLOC_MEM
        scrollbox.buffer = malloc( maxlines * sizeof(scrollbox_line_t));
        //scrollbox.buffer = calloc( maxlines, sizeof(scrollbox_line_t) );
    #else
        scrollbox.buffer = (scrollbox_line_t *) scrollbox_buffer;
    #endif

    if( !scrollbox.buffer ) // kein RAM mehr
    {
        lcd_cls();
        lcd_printp_at( 1, 3, PSTR("Error: ScrollBox"), MNORMAL);
        lcd_printp_at( 1, 4, PSTR("No more RAM"), MNORMAL);
        set_beep ( 500, 0x3333, BeepNormal);
        while (!get_key_short (1 << KEY_ESC));
        return false;
    }

    scrollbox.maxlines      = maxlines;
    scrollbox.lines         = 0;
    scrollbox.display_pos   = 0;

    scrollbox.key_enter.active      = false;
    scrollbox.key_enter_long.active = false;
    return true;
}



//--------------------------------------------------------------
// ScrollBox_Destroy()
//
// gibt reservierten Speicher wieder frei
// Wichtig! Da malloc verwendet wird!
//--------------------------------------------------------------
void ScrollBox_Destroy( void )
{
    if( scrollbox.buffer )
    {
        #ifdef SCROLLBOX_ALLOC_MEM
        free( scrollbox.buffer );
        #endif
    }
    scrollbox.buffer = 0;
}


//--------------------------------------------------------------
// ScrollBox_PushLine()
//
// fuegt eine Trennlinie hinzu
//--------------------------------------------------------------
void ScrollBox_PushLine( void )
{
    scrollbox_line_t *sboxp;

    if( scrollbox.lines < scrollbox.maxlines )
    {
        sboxp = scrollbox.buffer + scrollbox.lines;
        sboxp->mode = MSBLINE;
        scrollbox.lines++;
    }
}



//--------------------------------------------------------------
// _scrollbox_push(...)
//
// intern fuer: ScrollBox_Push_P(), ScrollBox_Push()
// Beschreibung siehe dort
//--------------------------------------------------------------
void _scrollbox_push( uint8_t useprogmem, uint8_t mode, const char *format, va_list ap )
{
    scrollbox_line_t *sboxp;

    if( scrollbox.lines < scrollbox.maxlines )
    {
        sboxp = scrollbox.buffer + scrollbox.lines;

        _xvsnprintf( useprogmem, buffer_sbline, SCROLLBOX_LINESIZE, format, ap);

        strncpyfill( sboxp->line, buffer_sbline, SCROLLBOX_LINESIZE);   // copy: buffer_sbline zur scrollbox
        sboxp->mode  = mode;

        scrollbox.lines++;
    }
}



//--------------------------------------------------------------
// ScrollBox_Push_P( mode, format, ...)
//
// Textzeile hinzufuegen - 'format' ist im PROGMEM.
//
// Parameter:
//   mode   : MNORMAL, MINVERS
//   format : siehe xprint-Doku in utils/xstring.c
//   ...    : Parameterliste fuer 'format'
//--------------------------------------------------------------
void ScrollBox_Push_P( uint8_t mode, const char *format, ... )
{
    va_list ap;

    va_start(ap, format);
    _scrollbox_push( true, mode, format, ap);
    va_end(ap);
}


//--------------------------------------------------------------
// ScrollBox_Push( mode, format, ...)
//
// Textzeile hinzufuegen - 'format' ist im RAM.
//
// Parameter:
//   mode   : MNORMAL, MINVERS
//   format : siehe xprint-Doku in utils/xstring.c
//   ...    : Parameterliste fuer 'format'
//--------------------------------------------------------------
void ScrollBox_Push( uint8_t mode, const char *format, ... )
{
    va_list ap;

    va_start(ap, format);
    _scrollbox_push( false, mode, format, ap);
    va_end(ap);
}


//--------------------------------------------------------------
// key: KEY_ENTER, KEY_ENTER_LONG  (keine weiteren bisher!)
//--------------------------------------------------------------
void ScrollBox_SetKey( uint8_t key, const char *keytext )
{
    if( key == KEY_ENTER )
    {
        scrollbox.key_enter.active   = true;
        strncpyfill( scrollbox.key_enter.text, keytext, SCROLLBOX_KEYSIZE);
    }

    if( key == KEY_ENTER_LONG )
    {
        scrollbox.key_enter_long.active = true;
        strncpyfill( scrollbox.key_enter_long.text, keytext, SCROLLBOX_KEYSIZE);
    }
}



//--------------------------------------------------------------
// ScrollBox_Refresh()
//
// zeigt die ScrollBox - wird normalerweise von ScrollBox_Show()
// automatisch durchgefuehrt
//--------------------------------------------------------------
void ScrollBox_Refresh( void )
{
    uint8_t y;
    uint8_t sh;
    uint8_t sy;
    scrollbox_line_t *sboxp;

    //--------------------------
    // Text
    //--------------------------
    for( y=0; y<7; y++)
    {
        sboxp = scrollbox.buffer + (scrollbox.display_pos + y);

        if( y + scrollbox.display_pos < scrollbox.lines )
        {

            if( sboxp->mode == MSBLINE )
            {
                lcd_frect( (21-SCROLLBOX_W)*6, (y*8), (SCROLLBOX_W*6), 7, 0);   // clear
                lcd_line ( (21-SCROLLBOX_W)*6, (y*8)+3, 125, (y*8)+3, 1);       // line
            }
            else
            {
                lcd_puts_at( 21-SCROLLBOX_W, y, sboxp->line, sboxp->mode);
            }
            //p += sizeof(scrollbox_line_t);
        }
        else    // clear
        {
              lcd_frect( (21-SCROLLBOX_W)*6, (y*8), (SCROLLBOX_W*6), 7, 0); // clear
        }
    }


    //--------------------------
    // Slider
    //--------------------------
    #define SLIDERH   55                                                // Finetuning der Slider-Hoehe

    // Slider: 7 zeilen * 8 pixel = 56 pixel
    sh = (SLIDERH * 7) / scrollbox.lines;                               // Slider: Hoehe
    sh = (sh > SLIDERH) ? SLIDERH : sh;

    sy = (scrollbox.display_pos * (SLIDERH-sh)) / (scrollbox.lines-7);  // Slider: Position

    lcd_frect( 0,  0, 1, SLIDERH, 0);                                   // Slider: Clear
    lcd_frect( 0, sy, 1, sh, 1);    // Slider: draw                     // Slider: Draw


    //--------------------------
    // Keyline
    //--------------------------
    PKT_KeylineUpDown( 1, 7,  0,0);                                     // Keyline: Up / Down
    lcd_printp_at( 12, 7, strGet(ENDE), MNORMAL);                       // Keyline: Ende

    if( scrollbox.key_enter.active )            lcd_print_at (17, 7, (uint8_t *)scrollbox.key_enter.text     , MNORMAL);
    else if ( scrollbox.key_enter_long.active ) lcd_print_at (17, 7, (uint8_t *)scrollbox.key_enter_long.text, MNORMAL);
    else                                        lcd_printp_at(17, 7, PSTR("    ")  , MNORMAL);

    //ScrollBox_Debug();
}


//--------------------------------------------------------------
// key = ScrollBox_Show()
//
//--------------------------------------------------------------
uint8_t ScrollBox_Show( void )
{
    uint8_t keyexit = 0;

    clear_key_all();

    if( scrollbox.buffer )
    {
        ScrollBox_Refresh();

        do
        {
            //--------------------------
            // Pruefe auf PKT-Update und
            // andere interne PKT-Aktionen
            //--------------------------
            if( PKT_CtrlHook() )                  // Update vom Updatetool angefordert?
            {
                lcd_cls();
                ScrollBox_Refresh();
            }

            //--------------------------
            // scrollen: nach unten
            //--------------------------
            if( get_key_press(1 << KEY_PLUS) || get_key_long_rpt_sp( (1 << KEY_PLUS),2 ))       // down
            {
                if( scrollbox.display_pos < ( scrollbox.lines - SCROLLBOX_H) )
                {
                    scrollbox.display_pos++;
                    ScrollBox_Refresh();
                }
                else
                {
                    set_beep ( 25, 0xffff, BeepNormal);                                         // am unteren Ende angelangt
                }
            }

            //--------------------------
            // scrollen: nach oben
            //--------------------------
            if( get_key_press(1 << KEY_MINUS) || get_key_long_rpt_sp( (1 << KEY_MINUS),2 ))     // up
            {
                if( scrollbox.display_pos > 0 )
                {
                    scrollbox.display_pos--;
                    ScrollBox_Refresh();
                }
                else
                {
                    set_beep ( 25, 0xffff, BeepNormal);                                         // am oberen Ende angelangt
                }
            }

            //--------------------------
            //--------------------------
            if( scrollbox.key_enter.active && get_key_short(1 << KEY_ENTER) )
            {
                keyexit = KEY_ENTER;
            }

            //--------------------------
            //--------------------------
            if( scrollbox.key_enter_long.active && get_key_long(1 << KEY_ENTER) )
            {
                keyexit = KEY_ENTER_LONG;
            }

        } while( !get_key_press(1 << KEY_ESC) && (keyexit == 0) );
    }

    //get_key_press(KEY_ALL);   // ersetzt durch obiges clear_key_all()

    return keyexit;
}

