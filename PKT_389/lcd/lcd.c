/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*****************************************************************************
 *   Copyright (C) 2009 Peter "woggle" Mack, mac@denich.net                  *
 *   - original LCD control by Thomas "thkais" Kaiser                        *
 *   - special number formating routines taken from C-OSD                    *
 *      from Claas Anders "CaScAdE" Rathje                                   *
 *   - some extension, ellipse and circ_line by Peter "woggle" Mack          *
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
 *                                                                           *
 *****************************************************************************/

//############################################################################
//# HISTORY  lcd.c
//#
//# 23.10.2015 cebra
//# - add: set_lcd_orientation , Funktion wieder eingebaut
//#
//# 10.11.2014 cebra
//# - fix: lcd_init() weiter Helligkeitseinstellungen korrigiert (OCR2A)
//#
//# 28.06.2014 OG
//# - chg: lcdx_cls_rowwidth() kompatibel zu mode MINVERSX und MNORMALX
//# - chg: lcdx_cls_row() auf lcdx_cls_rowwidth() umgestellt
//# - fix: lcdx_printp() umgestellt auf strlen_P() statt strlen()
//#
//# 27.06.2014 OG
//# - add: die lcd-print Funktionen wurden um MINVERSX und MNORMALX erweitert
//#        um einen zusaetzlichen Rand links und oben als Linie zu zeichnen
//#        (wird z.B. in followme.c verwendet)
//#
//# 11.06.2014 OG
//# - add: lcd_set_contrast()
//# - add: include <avr/interrupt.h>
//#
//# 04.06.2014 OG
//# - add: lcdx_cls_rowwidth()
//#
//# 26.05.2014 OG
//# - chg: LCD_Init() - LCD-Modus nur noch normal (kein Invers mehr)
//#
//# 02.05.2014 OG
//# - add: Popup_Draw() (ehemals in osd.c)
//#
//# 13.04.2014 OG
//# - add: lcd_print_LF()
//#
//# 11.04.2014 OG
//# - add: lcdx_cls_row()
//#
//# 10.04.2014 OG
//# - add: include: lipo.h
//#
//# 08.04.2014 OG
//# - add: lcdx_printf_center(), lcdx_printf_center_P()
//#
//# 07.04.2014 OG
//# - add: lcd_setpos(), lcd_print_char()
//#
//# 04.04.2014 OG
//# - add: ShowTitle_P()
//#
//# 28.02.2014 OG
//# - del: show_baudrate()
//# - chg: PRINTF_BUFFER_SIZE von 80 auf 40
//#
//# 16.02.2014 OG
//# - add: lcdx_printp_center(), lcdx_print_center()
//#
//# 13.02.2014 OG
//# - add: lcd_frect_round()
//# - chg: lcd_rect_round() um R2 ergaenzt
//#
//# 12.02.2014 OG
//# - add: lcd_rect_round()
//#
//# 04.02.2014 OG
//# - fix: writex_ndigit_number_u_100th() Aufbau der Formatmaske
//#
//# 03.02.2014 OG
//# - fix: writex_ndigit_number_u_100th() Berechnung Formatstring korrigiert
//# - fix: writex_ndigit_number_u_100th() Parameter 'mode' fehlte
//#
//# 16.06.2013 OG
//# - chg: LCD_ORIENTATION festgesetzt auf 0 (Normal)
//#
//# 04.05.2013 OG
//# - chg: angepasst auf xutils.c
//# - chg: writex_datetime_time(), writex_datetime_date() Lokalzeitberechnung
//#        via UTCdatetime2local()
//#
//# 03.05.2013 OG
//# - fix: writex_gpspos() - Anzeige negativer GPS-Koordinaten
//# - fix: Anzeigefehler writex_datetime_time()
//# - chg: writex_datetime_date() & writex_datetime_time() Parameter
//#        'timeoffset' entfernt da das durch PKT-Config geregelt werden soll
//#
//# 29.04.2013 OG
//# - chg: lcd_cls() geaendert auf memset
//#
//# 28.04.2013 OG
//# - chg: high-level Funktionen wie writex_ndigit... auf xprintf umgebaut
//# - add: lcdx_printf_at(), lcdx_printf_at_P()
//#        lcd_printf_at(), lcd_printf_at_P()
//#        siehe dazu: Doku in utils/xstring.h fuer xprintf
//#
//# 27.03.2013 OG
//# - add: writex_datetime_time()
//# - add: writex_datetime_date()
//# - add: Show_PKTError_NoRAM()
//#
//# 22.03.2013 OG
//# - add: writex_gpspos()
//#
//# 11.03.2013 OG
//# - fix: writex_time() Ausgabe Doppelpunkt ":" ergaenzt um mode,xoffs,yoffs
//#
//# 07.03.2013 OG
//# - del: Kompatibilitaetsfunktion lcd_printpj_at() entfernt
//#
//# 06.03.2013 OG
//# - lcdx_putc() wurde erweitert mit Unterstuetzung des 8x8 Font (alte Jeti
//#   Funktionen) und Pixel-Verschiebung (xoffs,yoffs)
//# - etliche Char basierte Funktionen wurden erweitert um Parameter xoffs,yoffs
//#   um die Ausgabe um +/- Pixel zu verschieben. Fuer Pixelgenaue Positionierung
//#   von OSD-Screen Elementen.
//#   Die neuen Funktionen mit Pixel-Verschiebung beginnen mit lcdx_, writex_ ...
//# - add: Kompatibilitaet gewaehrleistet durch Mapper-Funktionen
//# - add: defines fuer Char-Drawmode's (MNORMAL, MINVERS, MBIG, MBIGINVERS)
//# - del: Jeti-Funktionen (teilweise ersetzt durch Kompatibilitaetsfunktionen)
//############################################################################


#include "../cpu.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>

#include "../eeprom/eeprom.h"
#include "../timer/timer.h"
#include "lcd.h"
#include "../main.h"
#include "../HAL_HW3_9.h"
#include "../utils/xutils.h"            // xprintf, UTCdatetime2local
#include "../lipo/lipo.h"            // show_Lipo()


#include "font8x6.h"
#ifdef USE_FONT_BIG
  #include "Font8x8.h"
#endif


#define DISP_W 128
#define DISP_H 64

#define DISP_BUFFER ((DISP_H * DISP_W) / 8)
#define LINE_BUFFER (((DISP_H/8) * DISP_W) / 8)

#define Jeti 1  // Jeti Routinen

volatile uint8_t display_buffer[DISP_BUFFER];   // Display-Puffer, weil nicht zurückgelesen werden kann
volatile uint8_t line_buffer[LINE_BUFFER];      // Zeilen-Puffer, weil nicht zurückgelesen werden kann

volatile uint16_t display_buffer_pointer;       // Pointer auf das aktuell übertragene Byte
volatile uint8_t display_buffer_counter;        // Hilfszähler zur Selektierung der Page
volatile uint8_t display_page_counter;          // aktuelle Page-Nummer
volatile uint8_t display_mode;                  // Modus für State-Machine
volatile uint8_t LCD_ORIENTATION = 0;           // 0=Normal / 1=reversed

// DOG: 128 x 64 with 6x8 Font => 21 x 8
// MAX7456: 30 x 16

uint8_t lcd_xpos;
uint8_t lcd_ypos;

char s[7];



char format_buffer[20];

//-----------------------------------------------------------
void send_byte( uint8_t data )
{
    clr_cs ();
    SPDR = data;
    while (!(SPSR & (1<<SPIF)));
    //SPSR = SPSR;
    set_cs ();
}


//-----------------------------------------------------------
// * Writes one command byte
// * cmd           - the command byte
//
void lcd_command( uint8_t cmd )
{
//  LCD_SELECT();
//  LCD_CMD();
//  spi_write(cmd);
//  LCD_UNSELECT();
    clr_cs ();
    SPDR = cmd;
    while (!(SPSR & (1<<SPIF)));
    //SPSR = SPSR;
    set_cs ();
}



//-----------------------------------------------------------
//-----------------------------------------------------------
void lcd_set_contrast( uint8_t value )
{
    cli();
    clr_A0();
    send_byte( 0x81 );
    send_byte( value );      // Daten zum LCD senden
    set_A0();
    sei();
}

void lcd_set_orientation( uint8_t value )
{
    cli();
    clr_A0 ();
    if (value == 0)
    {
            send_byte (0xA1); // normal
            send_byte (0xC0); // normal
    }
    else
    {
            send_byte (0xA0); // reverse
            send_byte (0xC8); // reverse
    }

    set_A0 ();
    sei();
}




//-----------------------------------------------------------
void lcd_cls( void )
{
    uint16_t i, j;

    memset( (void *)display_buffer, 0, 1024);

//  for (i = 0; i < DISP_BUFFER; i++)
//      display_buffer[i] = 0x00;

    for (i = 0; i < 8; i++)
    {
        clr_A0 ();
        send_byte (0xB0 + i);           //1011xxxx
        send_byte (0x10);               //00010000
//      send_byte(0x04);                //00000100 gedreht plus 4 Byte
//      send_byte(0x00);                //00000000
        send_byte (LCD_ORIENTATION);    //00000000

        set_A0 ();
        for (j = 0; j < 128; j++)
            send_byte (0x00);
    }

    lcd_xpos = 0;
    lcd_ypos = 0;
}


//-----------------------------------------------------------
void set_adress (uint16_t adress, uint8_t data)
{
    uint8_t page;
    uint8_t column;

    page = adress >> 7;

    clr_A0 ();
    send_byte (0xB0 + page);

    column = (adress & 0x7F) + LCD_ORIENTATION;

    send_byte (0x10 + (column >> 4));
    send_byte (column & 0x0F);

    set_A0 ();
    send_byte (data);
}


//-----------------------------------------------------------
void scroll (void)
{
    uint16_t adress;

    for (adress = 0; adress < 896; adress++)
    {
        display_buffer[adress] = display_buffer[adress + 128];
        set_adress (adress, display_buffer[adress]);
    }

    for (adress = 896; adress < 1024; adress++)
    {
        display_buffer[adress] = 0;
        set_adress (adress, 0);
    }
}

//####################################################################################
//####################################################################################

//-----------------------------------------------------------
// + Plot (set one Pixel)
//-----------------------------------------------------------
// mode:
// 0=Clear, 1=Set, 2=XOR
void lcd_plot (uint8_t xpos, uint8_t ypos, uint8_t mode)
{
    uint16_t adress;
    uint8_t mask;

    if ((xpos < DISP_W) && (ypos < DISP_H))
    {
        adress = (ypos / 8) * DISP_W + xpos;        // adress = 0/8 * 128 + 0   = 0
        mask = 1 << (ypos & 0x07);                  // mask = 1<<0 = 1
        adress &= DISP_BUFFER - 1;
        switch (mode)
        {

        case 0:
            display_buffer[adress] &= ~mask;
            break;

        case 1:
            display_buffer[adress] |= mask;
            break;

        case 2:
            display_buffer[adress] ^= mask;
            break;
        }
        set_adress (adress, display_buffer[adress]);
    }
}



//-----------------------------------------------------------
//-----------------------------------------------------------
void lcdx_putc( uint8_t x, uint8_t y, uint8_t c, uint8_t mode, int8_t xoffs, int8_t yoffs )
{
    uint8_t ch;
    uint16_t adress;

    uint8_t x1,y1;
    uint8_t x0,y0;
    uint8_t xw;
    uint8_t mask;
    uint8_t bit;

    uint8_t *font;

    //------------------------
    // char translate
    //------------------------
    switch (c)
    {   // ISO 8859-1

        case 0xc4: c = 0x01; break;     // Ä
        case 0xe4: c = 0x02; break;     // ä
        case 0xd6: c = 0x03; break;     // Ö
        case 0xf6: c = 0x04; break;     // ö
        case 0xdc: c = 0x05; break;     // Ü
        case 0xfc: c = 0x06; break;     // ü
        case 0xdf: c = 0x1e; break;     // ß   c = 0x07;  ° (used by Jeti)
    }

    c &= 0x7f;

    //------------------------
    // Font Parameter setzen
    //------------------------
    #ifdef USE_FONT_BIG
    if( mode <=2 )      // normaler font (8x6)
    {
        font = (uint8_t *) &font8x6[0][0];
        xw   = 6;
    }
    else                // grosser font (8x8)
    {
        font = (uint8_t *) &Font8x8[0][0];
        xw   = 8;
    }
    #else
    font = (uint8_t *) &font8x6[0][0];
    xw   = 6;
    #endif


    //------------------------
    //------------------------
    x0 = (x*xw) + xoffs;
    y0 = (y*8)  + yoffs;


    if( yoffs == 0)
    {
        //----------------------------------------------------------
        // orginaler Character Algo
        //
        // funktioniert auch mit x Verschiebung aber nicht
        // mit y Verschiebung.
        //
        // Da 8 Bit aufeinmal gesetzt werden ist dieser Algo
        // bzgl. Geschwindigkeit effektiver als der Y-Algo.
        //----------------------------------------------------------

        if( mode==MINVERS || mode==MBIGINVERS )
            lcd_frect( (x*xw)+xoffs, (y*8), xw-1, 7, 1); // invertierte Darstellung

        adress = y * 128 + x * xw + xoffs;
        adress &= 0x3FF;

        for( x1 = 0; x1 < xw; x1++)
        {
            ch = pgm_read_byte (font + x1 + c * xw);

            if( mode==MINVERS || mode==MBIGINVERS )
                display_buffer[adress + x1] ^= ch;
            else
                display_buffer[adress + x1] = ch;

            set_adress (adress + x1, display_buffer[adress + x1]);
        }
    }
    else
    {
        //----------------------------------------------------------
        // Y-Algo
        // neuer Character Algo (nur wenn Pixel y-Verschiebung)
        //----------------------------------------------------------
        for( x1 = 0; x1 < xw; x1++ )
        {
            ch = pgm_read_byte (font + x1 + c * xw);

            mask = 1;

            for( y1 = 0; y1 < 8; y1++ )
            {
                bit = (ch & mask);

                if( bit )
                    lcd_plot( x0+x1, y0+y1, ( (mode==MINVERS || mode==MBIGINVERS) ? 0 : 1) );
                else
                    lcd_plot( x0+x1, y0+y1, ( (mode==MINVERS || mode==MBIGINVERS) ? 1 : 0) );

                mask = (mask << 1);
            }
        }
    }
}


//-----------------------------------------------------------
//--- Kompatibilitaet
//-----------------------------------------------------------
void lcd_putc( uint8_t x, uint8_t y, uint8_t c, uint8_t mode )
{
    lcdx_putc( x, y, c, mode, 0,0 );
}



//####################################################################################
//####################################################################################


//-----------------------------------------------------------
void lcd_cls_line (uint8_t x, uint8_t y, uint8_t w)
{
    uint8_t lcd_width;
    uint8_t lcd_zpos;
    uint8_t i;
    uint8_t max = 21;
    lcd_width = w;
    lcd_xpos = x;
    lcd_ypos = y;

    if ((lcd_xpos + lcd_width) > max)
        lcd_width = max - lcd_xpos;

    lcd_zpos = lcd_xpos + lcd_width;

    for (i = lcd_xpos; i < lcd_zpos; i++)
        lcd_putc (i, lcd_ypos, 0x20, 0);
}


//-----------------------------------------------------------
void wait_1ms (void)
{
    _delay_ms (1);
}


//-----------------------------------------------------------
void wait_ms (uint16_t time)
{
    uint16_t i;

    for (i = 0; i < time; i++)
        wait_1ms ();
}


//-----------------------------------------------------------
void LCD_Init( uint8_t LCD_Mode )    // LCD_Mode 0= Default Mode 1= EEPROM-Parameter)
{
    lcd_xpos = 0;
    lcd_ypos = 0;

//  DDRB = 0xFF;

    // SPI max. speed
    // the DOGM128 lcd controller can work at 20 MHz
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << CPHA) | (1 << CPOL);
    SPSR = (1 << SPI2X);

    set_cs();
    clr_reset();
    wait_ms(10);
    set_reset();

    clr_cs();
    clr_A0();

    send_byte( 0x40 );       //Display start line = 0


//    ------------------------------------------------------------
//      23.10.2015 Cebra
//      wieder aktiviert, weil benötigt
//
//     10.11.2014 cebra
//     Umschaltung der Displayansicht wird nicht mehr benötigt
        if (LCD_Mode == 1)
        {
            if (Config.LCD_Orientation == 0)
            {
                send_byte( 0xA1 ); // A1 normal A0 reverse(original)
                send_byte( 0xC0 ); // C0 normal C8 reverse(original)
            }
            else
            {
                send_byte( 0xA0 ); // A1 normal A0 reverse(original)
                send_byte( 0xC8 ); // C0 normal C8 reverse(original)
            }
        }
        else
        {
        send_byte( 0xA1 ); // A1 normal A0 reverse(original)
        send_byte( 0xC0 ); // C0 normal C8 reverse(original)
        }

    /*
    //------------------------------------------------------------
    // 26.05.2014 OG: einstellen von Normal/Invers durch den User
    // nicht mehr unterstuetzt, da der Modus Invers beim PKT
    // auf verschiedenen Display's nicht gut aussieht.
    // Ab jetzt nur noch LC-Modus Normal.
    //------------------------------------------------------------
    if (LCD_Mode == 1)
    {
        if (Config.LCD_DisplayMode == 0)
            send_byte (0xA6);       //Display normal, not mirrored
        else
            send_byte (0xA7);       //Display reverse, not mirrored
    }
    else
        send_byte (0xA6);
    */

    send_byte( 0xA6 );       //Display normal, not mirrored

    send_byte( 0xA2 );       //Set bias 1/9 (Duty 1/65)
    send_byte( 0x2F );       //Booster, regulator and follower on
    send_byte( 0xF8 );       //Set internal booster to 4x
    send_byte( 0x00 );       //Set internal booster to 4x
    send_byte( 0x27 );       //resistor ratio set

    if( LCD_Mode == 1 )
    {
        send_byte( 0x81 );       //Electronic volume register set
        send_byte( Config.LCD_Kontrast );        //Electronic volume register set
    }
    else
    {
        send_byte( 0x81 );
        send_byte( 0x16 );
    }

    send_byte( 0xAC );       //Cursor
    send_byte( 0x00 );       //No Cursor
    send_byte( 0xAF );       //No indicator



    if( Config.HWSound == 0 )
    {
        if( LCD_Mode == 1 )
        {
            //------------------------------------------------------------
            //10.11.2014 cebra,
            //Helligkeitseinstellungen werden nicht mehr genutzt
            // Helligkeit setzen
            //            OCR2A = Config.LCD_Helligkeit * 2.55;

            //OCR2A = 255;
        }
        else
        {
         //   OCR2A = 255;
        }
    }
    lcd_cls();
}




//-----------------------------------------------------------
// sicher eine Zeile f?r die Statusanzeige
void copy_line (uint8_t y)
{
    uint8_t i;
    uint16_t adress;

    adress = y * 128 + 0 * 6;
    adress &= 0x3FF;

    for (i = 0; i < 6*21; i++)
    {
        line_buffer[i] = display_buffer[adress+i];
        set_adress (adress + i, display_buffer[adress + i]);
    }
}


//-----------------------------------------------------------
// holt gesicherte Zeile wieder zurück
void paste_line (uint8_t y)
{
    uint8_t i;
    uint16_t adress;

    adress = y * 128 + 0 * 6;
    adress &= 0x3FF;

    for (i = 0; i < 6*21; i++)
    {
        display_buffer[adress+i] =line_buffer[i];
        set_adress (adress + i, display_buffer[adress + i]);
    }
}


//-----------------------------------------------------------
//-----------------------------------------------------------
void lcdx_puts_at( uint8_t x, uint8_t y, const char *s, uint8_t mode, int8_t xoffs, int8_t yoffs)
{
    while (*s)
    {
        lcdx_putc(x, y, *s++, mode, xoffs,yoffs);
        x++;
    }
}/* lcd_puts */


//-----------------------------------------------------------
//-----------------------------------------------------------
void lcd_puts_at( uint8_t x, uint8_t y, const char *s, uint8_t mode )
{
  lcdx_puts_at( x, y, s, mode, 0,0 );
}


//-----------------------------------------------------------
void new_line (void)
{
    lcd_ypos++;

    if (lcd_ypos > 7)
    {
        scroll ();
        lcd_ypos = 7;
    }
}


//-----------------------------------------------------------
void lcd_printpns (const char *text, uint8_t mode)
{
    while (pgm_read_byte(text))
    {
        switch (pgm_read_byte(text))
        {

        case 0x0D:
            lcd_xpos = 0;
            break;

        case 0x0A:
            new_line();
            break;

        default:
            lcd_putc (lcd_xpos, lcd_ypos, pgm_read_byte(text), mode);
            lcd_xpos++;
            if (lcd_xpos > 21)
            {
                lcd_xpos = 0;
//              new_line ();
            }
            break;
        }
        text++;
    }
}


//-----------------------------------------------------------
void lcd_printpns_at (uint8_t x, uint8_t y, const char *text, uint8_t mode)
{
    lcd_xpos = x;
    lcd_ypos = y;
    lcd_printpns (text, mode);
}



//--------------------------------------------------------------
// INTERN
//
// erweitert bei mode MINVERSX und MNORMALX links und oben den
// Text um jeweils einen Pixel -> der Invers-Modus sieht besser aus
//--------------------------------------------------------------
uint8_t _lcdx_print_modeextend( uint8_t progmem, uint8_t x, uint8_t y, uint8_t textlen, uint8_t mode, int8_t xoffs, int8_t yoffs )
{
    uint8_t draw = 0;

    if( (mode == MNORMALX) || (mode == MINVERSX) )
    {
        if( mode == MNORMALX )  mode = MNORMAL;
        else                    mode = MINVERS;

        if( mode == MINVERS )
            draw = 1;

        if( (y*8)+yoffs-1 >= 0 )
            lcd_line( (x*6)+xoffs, (y*8)+yoffs-1, (x*6)+xoffs+(textlen*6)-1, (y*8)+yoffs-1, draw);  // horizontale Linie ueber dem Text

        if( (x*6)+xoffs-1 >= 0 )
            lcd_line( (x*6)+xoffs-1, (y*8)+yoffs-1, (x*6)+xoffs-1, (y*8)+yoffs+7, draw);            // vertikale Linie links neben dem Text
    }

    return mode;
}



//-----------------------------------------------------------
//-----------------------------------------------------------
void _lcdx_print_outchar( unsigned char c, uint8_t mode, int8_t xoffs, int8_t yoffs)
{
    switch( c )
    {
        case 0x0D:  lcd_xpos = 0;
                    break;

        case 0x0A:  new_line();
                    break;

        default:    lcdx_putc( lcd_xpos, lcd_ypos, c, mode, xoffs,yoffs );
                    lcd_xpos++;
                    if( lcd_xpos > 21 )
                    {
                        lcd_xpos = 0;
                        new_line();
                    }
                    break;
    }
}



//-----------------------------------------------------------
//-----------------------------------------------------------
void lcdx_print( uint8_t *text, uint8_t mode, int8_t xoffs, int8_t yoffs)
{
    mode = _lcdx_print_modeextend( false, lcd_xpos, lcd_ypos, strlen( (const char *)text), mode, xoffs, yoffs );    // RAM Modus

    while( *text )
    {
        _lcdx_print_outchar( *text, mode, xoffs,yoffs);
        text++;
    }
}



//-----------------------------------------------------------
//-----------------------------------------------------------
void lcdx_printp( const char *text, uint8_t mode, int8_t xoffs, int8_t yoffs)
{
    char c;

    mode = _lcdx_print_modeextend( true, lcd_xpos, lcd_ypos, strlen_P( (const char *)text), mode, xoffs, yoffs );   // PROGMEM Modus

    while( (c = pgm_read_byte(text)) )
    {
        _lcdx_print_outchar( c, mode, xoffs,yoffs);
        text++;
    }
}






/*
//-----------------------------------------------------------
//-----------------------------------------------------------
void lcdx_print( uint8_t *text, uint8_t mode, int8_t xoffs, int8_t yoffs)
{
    mode = _lcdx_print_invers_extend( false, lcd_xpos, lcd_ypos, strlen( (const char *)text), mode, xoffs, yoffs ); // RAM Modus


    while( *text )
    {
        switch( *text )
        {
            case 0x0D:  lcd_xpos = 0;
                        break;

            case 0x0A:  new_line();
                        break;

            default:    lcdx_putc (lcd_xpos, lcd_ypos, *text, mode, xoffs,yoffs);
                        lcd_xpos++;
                        if( lcd_xpos > 21 )
                        {
                            lcd_xpos = 0;
                            new_line();
                        }
                        break;
        }
        text++;
    }
}



//-----------------------------------------------------------
//-----------------------------------------------------------
void lcdx_printp( const char *text, uint8_t mode, int8_t xoffs, int8_t yoffs)
{
    mode = _lcdx_print_invers_extend( true, lcd_xpos, lcd_ypos, strlen( (const char *)text), mode, xoffs, yoffs );  // PROGMEM Modus


    while( pgm_read_byte(text) )
    {
        switch( pgm_read_byte(text) )
        {
            case 0x0D:  lcd_xpos = 0;
                        break;

            case 0x0A:  new_line();
                        break;

            default:    lcdx_putc (lcd_xpos, lcd_ypos, pgm_read_byte(text), mode, xoffs,yoffs);
                        lcd_xpos++;
                        if( lcd_xpos > 21 )
                        {
                            lcd_xpos = 0;
                            new_line();
                        }
                        break;
        }
        text++;
    }
}
*/





//-----------------------------------------------------------
//-----------------------------------------------------------
void lcd_printp (const char *text, uint8_t mode)
{
    lcdx_printp ( text, mode, 0,0);
}


//-----------------------------------------------------------
//-----------------------------------------------------------
void lcdx_printp_at (uint8_t x, uint8_t y, const char *text, uint8_t mode, int8_t xoffs, int8_t yoffs)
{
    lcd_xpos = x;
    lcd_ypos = y;
    lcdx_printp (text, mode, xoffs,yoffs);
}




//-----------------------------------------------------------
//-----------------------------------------------------------
void lcd_printp_at (uint8_t x, uint8_t y, const char *text, uint8_t mode)
{
    lcdx_printp_at ( x, y, text, mode, 0,0);
}



//-----------------------------------------------------------
//--- lcd_print: Kompatibilitaet
//-----------------------------------------------------------
void lcd_print (uint8_t *text, uint8_t mode )
{
    lcdx_print (text, mode, 0,0 );
}


//-----------------------------------------------------------
void lcdx_print_at (uint8_t x, uint8_t y, uint8_t *text, uint8_t mode, int8_t xoffs, int8_t yoffs)
{
    lcd_xpos = x;
    lcd_ypos = y;
    lcdx_print (text, mode, xoffs, yoffs);
}



//-----------------------------------------------------------
//-----------------------------------------------------------
void lcdx_print_center( uint8_t y, uint8_t *text, uint8_t mode, int8_t xoffs, int8_t yoffs)
{
    xoffs = xoffs + ((uint8_t)(64 - ( (strlen((const char *)text)*6) /2)));       // Pixelgenau zentrieren (fuer 6x8 font)
    lcdx_print_at( 0, y, text, mode,   xoffs,yoffs);
}


//-----------------------------------------------------------
//-----------------------------------------------------------
void lcdx_printp_center( uint8_t y, const char *text, uint8_t mode, int8_t xoffs, int8_t yoffs)
{
    xoffs = xoffs + ((uint8_t)(64 - ( (strlen_P(text)*6) /2)));       // Pixelgenau zentrieren (fuer 6x8 font)
    lcdx_printp_at( 0, y, text, mode,   xoffs,yoffs);
}


//-----------------------------------------------------------
// lcdx_printf_center( y, mode, xoffs,yoffs, format, ...)
//
// Ausgabe von n-Parametern via Formattierung
//   mit erweitertem xoffs,yoffs
//
// Die Ausgabe wird zentriert,
//
// Parameter:
//  y          : Position in Char y
//  mode       : MNORMAL, MINVERS, ...
//  xoffs,yoffs: Verschiebung in Pixel
//  format     : String aus PROGMEM (siehe: xprintf in utils/xstring.h)
//  ...        : Parameter fuer 'format'
//-----------------------------------------------------------
void lcdx_printf_center( uint8_t y, uint8_t mode, int8_t xoffs, int8_t yoffs, const char *format, ... )
{
    va_list ap;

    va_start( ap, format );
    _xvsnprintf( false, printf_buffer, PRINTF_BUFFER_SIZE, format, ap );
    va_end(ap);
    lcdx_print_center( y,  (unsigned char *)printf_buffer, mode, xoffs, yoffs);
}



//-----------------------------------------------------------
// lcdx_printf_center_P( y, mode, xoffs,yoffs, format, ...)
//
// Ausgabe von n-Parametern via Formattierung
//   mit erweitertem xoffs,yoffs (PROGMEN-Version)
//
// Die Ausgabe wird zentriert,
//
// Parameter:
//  y          : Position in Char y
//  mode       : MNORMAL, MINVERS, ...
//  xoffs,yoffs: Verschiebung in Pixel
//  format     : String aus PROGMEM (siehe: xprintf in utils/xstring.h)
//  ...        : Parameter fuer 'format'
//-----------------------------------------------------------
void lcdx_printf_center_P( uint8_t y, uint8_t mode, int8_t xoffs, int8_t yoffs, const char *format, ... )
{
    va_list ap;

    va_start( ap, format );
    _xvsnprintf( true, printf_buffer, PRINTF_BUFFER_SIZE, format, ap );
    va_end(ap);
    lcdx_print_center( y,  (unsigned char *)printf_buffer, mode, xoffs, yoffs);
}



//-----------------------------------------------------------
//--- lcd_print_at: Kompatibilitaet
//-----------------------------------------------------------
void lcd_print_at (uint8_t x, uint8_t y, uint8_t *text, uint8_t mode )
{
    lcdx_print_at ( x, y, text, mode, 0,0);
}


//-----------------------------------------------------------
void print_display (uint8_t *text)
{
    while (*text)
    {
        lcd_putc (lcd_xpos, lcd_ypos, *text, 0);
        lcd_xpos++;
        if (lcd_xpos >= 20)
        {
            lcd_xpos = 0;
            new_line ();
        }
        text++;
    }
}


//-----------------------------------------------------------
void print_display_at (uint8_t x, uint8_t y, uint8_t *text)
{
    lcd_xpos = x;
    lcd_ypos = y;
    print_display (text);
}


//-----------------------------------------------------------
// + Line (draws a line from x1,y1 to x2,y2
// + Based on Bresenham line-Algorithm
// + found in the internet, modified by thkais 2007
//-----------------------------------------------------------

void lcd_line( unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, uint8_t mode)
{
    int x, y, count, xs, ys, xm, ym;

    x = (int) x1;
    y = (int) y1;
    xs = (int) x2 - (int) x1;
    ys = (int) y2 - (int) y1;
    if (xs < 0)
        xm = -1;
    else
        if (xs > 0)
            xm = 1;
        else
            xm = 0;
    if (ys < 0)
        ym = -1;
    else
        if (ys > 0)
            ym = 1;
        else
            ym = 0;
    if (xs < 0)
        xs = -xs;

    if (ys < 0)
        ys = -ys;

    lcd_plot ((unsigned char) x, (unsigned char) y, mode);

    if (xs > ys) // Flat Line <45 degrees
    {
        count = -(xs / 2);
        while (x != x2)
        {
            count = count + ys;
            x = x + xm;
            if (count > 0)
            {
                y = y + ym;
                count = count - xs;
            }
            lcd_plot ((unsigned char) x, (unsigned char) y, mode);
        }
    }
    else // Line >=45 degrees
    {
        count =- (ys / 2);
        while (y != y2)
        {
            count = count + xs;
            y = y + ym;
            if (count > 0)
            {
                x = x + xm;
                count = count - ys;
            }
            lcd_plot ((unsigned char) x, (unsigned char) y, mode);
        }
    }
}


//-----------------------------------------------------------
// + Filled rectangle
// + x1, y1 = upper left corner
//-----------------------------------------------------------
void lcd_frect( uint8_t x1, uint8_t y1, uint8_t widthx, uint8_t widthy, uint8_t mode)
{
    uint16_t x2, y2;
    uint16_t i;

    if (x1 >= DISP_W)
        x1 = DISP_W - 1;

    if (y1 >= DISP_H)
        y1 = DISP_H - 1;

    x2 = x1 + widthx;
    y2 = y1 + widthy;

    if (x2 > DISP_W)
        x2 = DISP_W;

    if (y2 > DISP_H)
        y2 = DISP_H;

    for (i = y1; i <= y2; i++)
    {
        lcd_line (x1, i, x2, i, mode);
    }
}


//-----------------------------------------------------------
// ausgefuelltes Rechteck mit abgerundeten Ecken
//
// Hinweis:
//  r (Radius) ist aktuell 'pseudo' und unterstuetzt
//  nur R0 (=0), R1 (=1) oder R2 (=2=
//-----------------------------------------------------------
void lcd_frect_round( uint8_t x1, uint8_t y1, uint8_t widthx, uint8_t widthy, uint8_t mode, uint8_t r)
{
    lcd_frect( x1, y1, widthx, widthy, mode);

    switch(r)
    {
        case R0:    break;

        case R2:
                    lcd_plot( x1+1       , y1         , !mode);     // Ecke links oben
                    lcd_plot( x1         , y1+1       , !mode);

                    lcd_plot( x1+widthx-1, y1         , !mode);     // Ecke rechts oben
                    lcd_plot( x1+widthx  , y1+1       , !mode);

                    lcd_plot( x1         , y1+widthy-1, !mode);     // Ecke links unten
                    lcd_plot( x1+1       , y1+widthy  , !mode);

                    lcd_plot( x1+widthx-1, y1+widthy  , !mode);     // Ecke rechts unten
                    lcd_plot( x1+widthx  , y1+widthy-1, !mode);

        case R1:
                    lcd_plot( x1         , y1         , !mode);     // Ecke links oben
                    lcd_plot( x1+widthx  , y1         , !mode);     // Ecke rechts oben
                    lcd_plot( x1         , y1+widthy  , !mode);     // Ecke links unten
                    lcd_plot( x1+widthx  , y1+widthy  , !mode);     // Ecke rechts unten
    }
}


//-----------------------------------------------------------
// + outline of rectangle
// + x1, y1 = upper left corner
//-----------------------------------------------------------
void lcd_rect( uint8_t x1, uint8_t y1, uint8_t widthx, uint8_t widthy, uint8_t mode)
{
    uint16_t x2, y2;

    if (x1 >= DISP_W)
        x1 = DISP_W - 1;
    if (y1 >= DISP_H)
        y1 = DISP_H - 1;
    x2 = x1 + widthx;
    y2 = y1 + widthy;

    if (x2 > DISP_W)
        x2 = DISP_W;

    if (y2 > DISP_H)
        y2 = DISP_H;

    lcd_line (x1, y1, x2, y1, mode);
    lcd_line (x2, y1, x2, y2, mode);
    lcd_line (x2, y2, x1, y2, mode);
    lcd_line (x1, y2, x1, y1, mode);
}


//-----------------------------------------------------------
// Rechteck mit mit abgerundeten Ecken
//
// Hinweis:
//  r (Radius) ist aktuell 'pseudo' und unterstuetzt
//  nur R0 (=0), R1 (=1) oder R2 (=2=
//-----------------------------------------------------------
void lcd_rect_round( uint8_t x1, uint8_t y1, uint8_t widthx, uint8_t widthy, uint8_t mode, uint8_t r)
{
    lcd_rect( x1, y1, widthx, widthy, mode);

    switch(r)
    {
        case R0:    break;

        case R2:
                    lcd_plot( x1+1       , y1         , !mode);     // Ecke links oben
                    lcd_plot( x1         , y1+1       , !mode);
                    lcd_plot( x1+1       , y1+1       ,  mode);

                    lcd_plot( x1+widthx-1, y1         , !mode);     // Ecke rechts oben
                    lcd_plot( x1+widthx  , y1+1       , !mode);
                    lcd_plot( x1+widthx-1, y1+1       ,  mode);

                    lcd_plot( x1         , y1+widthy-1, !mode);     // Ecke links unten
                    lcd_plot( x1+1       , y1+widthy  , !mode);
                    lcd_plot( x1+1       , y1+widthy-1,  mode);

                    lcd_plot( x1+widthx-1, y1+widthy  , !mode);     // Ecke rechts unten
                    lcd_plot( x1+widthx  , y1+widthy-1, !mode);
                    lcd_plot( x1+widthx-1, y1+widthy-1,  mode);

        case R1:
                    lcd_plot( x1         , y1         , !mode);     // Ecke links oben
                    lcd_plot( x1+widthx  , y1         , !mode);     // Ecke rechts oben
                    lcd_plot( x1         , y1+widthy  , !mode);     // Ecke links unten
                    lcd_plot( x1+widthx  , y1+widthy  , !mode);     // Ecke rechts unten
    }
}


//-----------------------------------------------------------
// + outline of a circle
// + Based on Bresenham-algorithm found in wikipedia
// + modified by thkais (2007)
//-----------------------------------------------------------
void lcd_circle (int16_t x0, int16_t y0, int16_t radius, uint8_t mode)
{
    int16_t f = 1 - radius;
    int16_t ddF_x = 0;
    int16_t ddF_y = -2 * radius;
    int16_t x = 0;
    int16_t y = radius;

    lcd_plot (x0, y0 + radius, mode);
    lcd_plot (x0, y0 - radius, mode);
    lcd_plot (x0 + radius, y0, mode);
    lcd_plot (x0 - radius, y0, mode);

    while (x < y)
    {
        if (f >= 0)
        {
            y --;
            ddF_y += 2;
            f += ddF_y;
        }
        x ++;
        ddF_x += 2;
        f += ddF_x + 1;

        lcd_plot (x0 + x, y0 + y, mode);
        lcd_plot (x0 - x, y0 + y, mode);

        lcd_plot (x0 + x, y0 - y, mode);
        lcd_plot (x0 - x, y0 - y, mode);

        lcd_plot (x0 + y, y0 + x, mode);
        lcd_plot (x0 - y, y0 + x, mode);

        lcd_plot (x0 + y, y0 - x, mode);
        lcd_plot (x0 - y, y0 - x, mode);
    }
}


//-----------------------------------------------------------
// + filled Circle
// + modified circle-algorithm thkais (2007)
//-----------------------------------------------------------
void lcd_fcircle (int16_t x0, int16_t y0, int16_t radius,uint8_t mode)
{
    int16_t f = 1 - radius;
    int16_t ddF_x = 0;
    int16_t ddF_y = -2 * radius;
    int16_t x = 0;
    int16_t y = radius;

    lcd_line (x0, y0 + radius, x0, y0 - radius, mode);

    lcd_line (x0 + radius, y0, x0 - radius, y0, mode);

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x + 1;

        lcd_line (x0 + x, y0 + y, x0 - x, y0 + y, mode);
        lcd_line (x0 + x, y0 - y, x0 - x, y0 - y, mode);
        lcd_line (x0 + y, y0 + x, x0 - y, y0 + x, mode);
        lcd_line (x0 + y, y0 - x, x0 - y, y0 - x, mode);
    }
}


//-----------------------------------------------------------
//
void lcd_circ_line (uint8_t x, uint8_t y, uint8_t r, uint16_t deg, uint8_t mode)
{
    uint8_t xc, yc;
    double deg_rad;

    deg_rad = (deg * M_PI) / 180.0;

    yc = y - (uint8_t) round (cos (deg_rad) * (double) r);
    xc = x + (uint8_t) round (sin (deg_rad) * (double) r);
    lcd_line (x, y, xc, yc, mode);
}


//-----------------------------------------------------------
//
void lcd_ellipse_line (uint8_t x, uint8_t y, uint8_t rx, uint8_t ry, uint16_t deg, uint8_t mode)
{
    uint8_t xc, yc;
    double deg_rad;

    deg_rad = (deg * M_PI) / 180.0;

    yc = y - (uint8_t) round (cos (deg_rad) * (double) ry);
    xc = x + (uint8_t) round (sin (deg_rad) * (double) rx);
    lcd_line (x, y, xc, yc, mode);
}


//-----------------------------------------------------------
//
void lcd_ellipse (int16_t x0, int16_t y0, int16_t rx, int16_t ry, uint8_t mode)
{
    const int16_t rx2 = rx * rx;
    const int16_t ry2 = ry * ry;
    int16_t F = round (ry2 - rx2 * ry + 0.25 * rx2);
    int16_t ddF_x = 0;
    int16_t ddF_y = 2 * rx2 * ry;
    int16_t x = 0;
    int16_t y = ry;

    lcd_plot (x0, y0 + ry, mode);
    lcd_plot (x0, y0 - ry, mode);
    lcd_plot (x0 + rx, y0, mode);
    lcd_plot (x0 - rx, y0, mode);
    // while ( 2*ry2*x < 2*rx2*y ) {  we can use ddF_x and ddF_y
    while (ddF_x < ddF_y)
    {
        if(F >= 0)
        {
            y     -= 1;     // south
            ddF_y -= 2 * rx2;
            F     -= ddF_y;
        }
        x     += 1;         // east
        ddF_x += 2 * ry2;
        F     += ddF_x + ry2;
        lcd_plot (x0 + x, y0 + y, mode);
        lcd_plot (x0 + x, y0 - y, mode);
        lcd_plot (x0 - x, y0 + y, mode);
        lcd_plot (x0 - x, y0 - y, mode);
    }
    F = round (ry2 * (x + 0.5) * (x + 0.5) + rx2 * (y - 1) * (y - 1) - rx2 * ry2);
    while(y > 0)
    {
        if(F <= 0)
        {
            x     += 1;     // east
            ddF_x += 2 * ry2;
            F     += ddF_x;
        }
        y     -= 1;         // south
        ddF_y -= 2 * rx2;
        F     += rx2 - ddF_y;
        lcd_plot (x0 + x, y0 + y, mode);
        lcd_plot (x0 + x, y0 - y, mode);
        lcd_plot (x0 - x, y0 + y, mode);
        lcd_plot (x0 - x, y0 - y, mode);
    }
}


//-----------------------------------------------------------
//
void lcd_ecircle (int16_t x0, int16_t y0, int16_t radius, uint8_t mode)
{
    lcd_ellipse (x0, y0, radius + 3, radius, mode);
}


//-----------------------------------------------------------
//
void lcd_ecirc_line (uint8_t x, uint8_t y, uint8_t r, uint16_t deg, uint8_t mode)
{
    lcd_ellipse_line(x, y, r + 3, r, deg, mode);
}


//-----------------------------------------------------------
//
void lcd_view_font (uint8_t page)
{
    int x;
    int y;

    lcd_cls ();
    lcd_printp (PSTR("  0123456789ABCDEF\r\n"), 0);
    lcd_printpns_at (0, 7, PSTR(" \x1a    \x1b     Exit"), 0);

    lcd_ypos = 2;
    for (y = page * 4 ; y < (page * 4 + 4); y++)
    {
        if (y < 10)
        {
            lcd_putc (0, lcd_ypos, '0' + y, 0);
        }
        else
        {
            lcd_putc (0, lcd_ypos, 'A' + y - 10, 0);
        }
        lcd_xpos = 2;
        for (x = 0; x < 16; x++)
        {
            lcd_putc (lcd_xpos, lcd_ypos, y * 16 + x, 0);
            lcd_xpos++;
        }
        lcd_ypos++;
    }
}


//-----------------------------------------------------------
uint8_t hdigit (uint8_t d)
{
    if (d < 10)
    {
        return '0' + d;
    }
    else
    {
        return 'A' + d - 10;
    }
}


//-----------------------------------------------------------
void lcd_print_hex_at (uint8_t x, uint8_t y, uint8_t h, uint8_t mode)
{
    lcd_xpos = x;
    lcd_ypos = y;

    lcd_putc (lcd_xpos++, lcd_ypos, hdigit (h >> 4), mode);
    lcd_putc (lcd_xpos, lcd_ypos, hdigit (h & 0x0f), mode);
}


//-----------------------------------------------------------
void lcd_print_hex (uint8_t h, uint8_t mode)
{
//  lcd_xpos = x;
//  lcd_ypos = y;

    lcd_putc (lcd_xpos++, lcd_ypos, hdigit (h >> 4), mode);
    lcd_putc (lcd_xpos++, lcd_ypos, hdigit (h & 0x0f), mode);
    lcd_putc (lcd_xpos++, lcd_ypos, ' ', mode);
}

//##################################################################################################################################
//##################################################################################################################################

//-----------------------------------------------------------
// lcdx_printf_at( x,y, mode, xoffs,yoffs, format, ...)
//
// Ausgabe von n-Parametern via Formattierung
//   mit erweitertem xoffs,yoffs (RAM-Version)
//
// Parameter:
//  x,y        : Position in Char x,y
//  mode       : MNORMAL, MINVERS, ...
//  xoffs,yoffs: Verschiebung in Pixel
//  format     : String aus RAM (siehe: xprintf in utils/xstring.h)
//  ...        : Parameter fuer 'format'
//-----------------------------------------------------------
void lcdx_printf_at( uint8_t x, uint8_t y, uint8_t mode, int8_t xoffs, int8_t yoffs, const char *format, ... )
{
    va_list ap;

    va_start( ap, format );
    // _xvsnprintf( int useprogmem, char *buffer, size_t n, const char *format, va_list ap )
    _xvsnprintf( false, printf_buffer, PRINTF_BUFFER_SIZE, format, ap );
    va_end(ap);
    lcdx_print_at( x, y, (unsigned char *)printf_buffer, mode, xoffs, yoffs);
}


//-----------------------------------------------------------
// lcdx_printf_at_P( x,y, mode, xoffs,yoffs, format, ...)
//
// Ausgabe von n-Parametern via Formattierung
//   mit erweitertem xoffs,yoffs (PROGMEN-Version)
//
// Parameter:
//  x,y        : Position in Char x,y
//  mode       : MNORMAL, MINVERS, ...
//  xoffs,yoffs: Verschiebung in Pixel
//  format     : String aus PROGMEM (siehe: xprintf in utils/xstring.h)
//  ...        : Parameter fuer 'format'
//-----------------------------------------------------------
void lcdx_printf_at_P( uint8_t x, uint8_t y, uint8_t mode, int8_t xoffs, int8_t yoffs, const char *format, ... )
{
    va_list ap;

    va_start( ap, format );
    _xvsnprintf( true, printf_buffer, PRINTF_BUFFER_SIZE, format, ap );
    va_end(ap);
    lcdx_print_at( x, y, (unsigned char *)printf_buffer, mode, xoffs, yoffs);
}


//-----------------------------------------------------------
// lcdx_printf_at( x,y, mode, xoffs,yoffs, format, ...)
//
// Ausgabe von n-Parametern via Formattierung (RAM-Version)
//
// Parameter:
//  x,y        : Position in Char x,y
//  mode       : MNORMAL, MINVERS, ...
//  format     : String aus RAM (siehe: xprintf in utils/xstring.h)
//  ...        : Parameter fuer 'format'
//-----------------------------------------------------------
void lcd_printf_at( uint8_t x, uint8_t y, uint8_t mode, const char *format, ... )
{
    va_list ap;

    va_start( ap, format );
    // _xvsnprintf( int useprogmem, char *buffer, size_t n, const char *format, va_list ap )
    _xvsnprintf( false, printf_buffer, PRINTF_BUFFER_SIZE, format, ap );
    va_end(ap);
    lcdx_print_at( x, y, (unsigned char *)printf_buffer, mode, 0,0);
}


//-----------------------------------------------------------
// lcd_printf_at_P( x,y, mode, xoffs,yoffs, format, ...)
//
// Ausgabe von n-Parametern via Formattierung (PROGMEN-Version)
//
// Parameter:
//  x,y        : Position in Char x,y
//  mode       : MNORMAL, MINVERS, ...
//  format     : String aus PROGMEM (siehe: xprintf in utils/xstring.h)
//  ...        : Parameter fuer 'format'
//-----------------------------------------------------------
void lcd_printf_at_P( uint8_t x, uint8_t y, uint8_t mode, const char *format, ... )
{
    va_list ap;

    va_start( ap, format );
    _xvsnprintf( true, printf_buffer, PRINTF_BUFFER_SIZE, format, ap );
    va_end(ap);
    lcdx_print_at( x, y, (unsigned char *)printf_buffer, mode, 0,0);
}



//##################################################################################################################################
//##################################################################################################################################


//-----------------------------------------------------------
void lcd_write_number_u (uint8_t number)
{
    uint8_t num = 100;
    uint8_t started = 0;

    while (num > 0)
    {
        uint8_t b = number / num;
        if (b > 0 || started || num == 1)
        {
            lcd_putc (lcd_xpos++, lcd_ypos, '0' + b, 0);
            started = 1;
        }
        number -= b * num;

        num /= 10;
    }
}


//-----------------------------------------------------------
void lcd_write_number_u_at (uint8_t x, uint8_t y, uint8_t number)
{
    lcd_xpos = x;
    lcd_ypos = y;
    lcd_write_number_u (number);
}


//-----------------------------------------------------------
// numtype: 'd' oder 'u'
//
// Ergebnis: ein String in format_buffer
//  -> "%4d"  "%4.2d" "%03.1d"  "%4u"  usw...
//-----------------------------------------------------------
void make_number_format( char numtype, int16_t length, int16_t decimals, uint8_t pad )
{
    register char *p;
    register char *psrc;

    p = format_buffer;
    *p = '%';                       p++;                // start '%'
    if(pad) { *p = '0';             p++; }              // pad '0'

    itoa( length, s, 10 );
    psrc = s; while(*psrc)          *p++ = *psrc++;     // vorkomma

    if( decimals > 0 )
    {
        *p = '.';                   p++;                // punkt '.'
        itoa( decimals, s, 10 );
        psrc = s; while(*psrc)      *p++ = *psrc++;     // nachkomma
    }

    *p = numtype;                   p++;                // 'd' oder 'u'
    *p = 0;
}


//-----------------------------------------------------------
// Write only some digits of a unsigned <number> at <x>/<y> to MAX7456 display memory
// <num> represents the largest multiple of 10 that will still be displayable as
// the first digit, so num = 10 will be 0-99 and so on
// <pad> = 1 will cause blank spaced to be filled up with zeros e.g. 007 instead of   7
//
void writex_ndigit_number_u (uint8_t x, uint8_t y, uint16_t number, int16_t length, uint8_t pad, uint8_t mode, int8_t xoffs, int8_t yoffs)
{
    make_number_format( 'u', length, 0, pad );                          // ergebnis in: format_buffer
    lcdx_printf_at( x, y, mode, xoffs, yoffs, format_buffer, number );
}


//-----------------------------------------------------------
//-----------------------------------------------------------
void write_ndigit_number_u (uint8_t x, uint8_t y, uint16_t number, int16_t length, uint8_t pad, uint8_t mode)
{
    writex_ndigit_number_u( x, y, number, length, pad, mode, 0,0);
}


//-----------------------------------------------------------
// Write only some digits of a signed <number> at <x>/<y> to MAX7456 display memory
// <num> represents the largest multiple of 10 that will still be displayable as
// the first digit, so num = 10 will be 0-99 and so on
// <pad> = 1 will cause blank spaced to be filled up with zeros e.g. 007 instead of   7
//
void writex_ndigit_number_s (uint8_t x, uint8_t y, int16_t number, int16_t length, uint8_t pad, uint8_t mode, int8_t xoffs, int8_t yoffs)
{
    make_number_format( 'd', length, 0, pad );                          // ergebnis in: format_buffer
    lcdx_printf_at( x, y, mode, xoffs, yoffs, format_buffer, number );
}


//-----------------------------------------------------------
//-----------------------------------------------------------
void write_ndigit_number_s (uint8_t x, uint8_t y, int16_t number, int16_t length, uint8_t pad, uint8_t mode)
{
    writex_ndigit_number_s (x, y, number, length, pad, mode, 0,0);
}


//-----------------------------------------------------------
// Write only some digits of a unsigned <number> at <x>/<y> to MAX7456 display memory
// as /10th of the value
// <num> represents the largest multiple of 10 that will still be displayable as
// the first digit, so num = 10 will be 0-99 and so on
// <pad> = 1 will cause blank spaced to be filled up with zeros e.g. 007 instead of   7
//
void writex_ndigit_number_u_10th( uint8_t x, uint8_t y, uint16_t number, int16_t length, uint8_t pad, uint8_t mode, int8_t xoffs, int8_t yoffs)
{
    make_number_format( 'u', length-1, 1, pad );                        // ergebnis in: format_buffer
    lcdx_printf_at( x, y, mode, xoffs, yoffs, format_buffer, number );
}


//-----------------------------------------------------------
//-----------------------------------------------------------
void write_ndigit_number_u_10th( uint8_t x, uint8_t y, uint16_t number, int16_t length, uint8_t pad, uint8_t mode)
{
  writex_ndigit_number_u_10th( x, y, number, length, pad, mode, 0,0);
}


//-----------------------------------------------------------
//-----------------------------------------------------------
void writex_ndigit_number_u_100th( uint8_t x, uint8_t y, uint16_t number, int16_t length, uint8_t pad, uint8_t mode, int8_t xoffs, int8_t yoffs)
{
    make_number_format( 'u', length-2, 2, pad );                        // ergebnis in: format_buffer
    lcdx_printf_at( x, y, mode, xoffs, yoffs, format_buffer, number );
}


//-----------------------------------------------------------
//-----------------------------------------------------------
void write_ndigit_number_u_100th( uint8_t x, uint8_t y, uint16_t number, int16_t length, uint8_t pad)
{
    writex_ndigit_number_u_100th( x, y, number, length, pad, MNORMAL, 0,0);
}


//-----------------------------------------------------------
// Write only some digits of a signed <number> at <x>/<y> to MAX7456 display memory
// as /10th of the value
// <num> represents the largest multiple of 10 that will still be displayable as
// the first digit, so num = 10 will be 0-99 and so on
// <pad> = 1 will cause blank spaced to be filled up with zeros e.g. 007 instead of   7
//
void writex_ndigit_number_s_10th (uint8_t x, uint8_t y, int16_t number, int16_t length, uint8_t pad, uint8_t mode, int8_t xoffs, int8_t yoffs)
{
    make_number_format( 'd', length-1, 1, pad );                        // ergebnis in: format_buffer
    lcdx_printf_at( x, y, mode, xoffs, yoffs, format_buffer, number );
}


//-----------------------------------------------------------
//-----------------------------------------------------------
void write_ndigit_number_s_10th (uint8_t x, uint8_t y, int16_t number, int16_t length, uint8_t pad, uint8_t mode)
{
    writex_ndigit_number_s_10th ( x,  y,  number,  length,  pad,  mode,  0,0);

}


//-----------------------------------------------------------
// write <seconds> as human readable time at <x>/<y> to MAX7456 display mem
//
void writex_time( uint8_t x, uint8_t y, uint16_t seconds, uint8_t mode, int8_t xoffs, int8_t yoffs)
{
    uint16_t min = seconds / 60;
    seconds -= min * 60;

    lcdx_printf_at( x, y, mode, xoffs, yoffs, "%02u:%02u", min, seconds);
    /*
    writex_ndigit_number_u (x, y, min, 2, 0,mode, xoffs,yoffs);
    lcdx_putc (x + 2, y, ':', mode, xoffs,yoffs);
    writex_ndigit_number_u (x + 3, y, seconds, 2, 1,mode, xoffs,yoffs);
    */
}


//-----------------------------------------------------------
//-----------------------------------------------------------
void write_time( uint8_t x, uint8_t y, uint16_t seconds)
{
    writex_time( x,  y,  seconds, 0, 0,0);
}


//--------------------------------------------------------------
// writex_datetime_time()
//
// Anzeigeformat: 'hh:mm:ss'
//
// datetime  : vom Typ PKTdatetime_t
//--------------------------------------------------------------
void writex_datetime_time( uint8_t x, uint8_t y, PKTdatetime_t datetime, uint8_t mode, int8_t xoffs, int8_t yoffs)
{
    PKTdatetime_t dtlocal;

    UTCdatetime2local( &dtlocal, &datetime );
    lcdx_printf_at( x, y, mode, xoffs, yoffs, "%02u:%02u:%02u", (uint8_t)(dtlocal.seconds/3600), (uint8_t)((dtlocal.seconds/60)%60), (uint8_t)(dtlocal.seconds%60));
}


//--------------------------------------------------------------
// writex_datetime_date()
//
// Anzeigeformat: 'dd.mm.yyyy'  (keine Unterstuetzung von anderen Formaten aus aller Welt)
//
// datetime  : vom Typ PKTdatetime_t
//--------------------------------------------------------------
void writex_datetime_date( uint8_t x, uint8_t y, PKTdatetime_t datetime, uint8_t mode, int8_t xoffs, int8_t yoffs)
{
    PKTdatetime_t dtlocal;

    if( datetime.year > 0 )
    {
        UTCdatetime2local( &dtlocal, &datetime );
        lcdx_printf_at( x, y, mode, xoffs, yoffs, "%02u.%02u.%04u", dtlocal.day, dtlocal.month, dtlocal.year);
    }
    else
    {
        // keine UTCZeit -> keine Datumsanzeige
        lcdx_printp_at( x, y, PSTR("  .  .    "), mode, xoffs,yoffs);
    }
}


//-----------------------------------------------------------
// writes a single gps-pos
//-----------------------------------------------------------
void writex_gpspos( uint8_t x, uint8_t y, int32_t GPSpos, uint8_t mode, int8_t xoffs, int8_t yoffs )
{
    lcdx_printf_at( x, y, mode, xoffs, yoffs, "%3.5ld", GPSpos/100);
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void Show_PKTError_NoRAM(void)
{
    lcd_cls();
    lcd_rect( 8, 8, 127-16, 6*8, 1);

    //                       123456789012345678901
    lcd_printp_at( 2,2 , PSTR("** OUT OF RAM **"), MINVERS);
    lcd_printp_at( 2,4 , PSTR("this function is"), MNORMAL);
    lcd_printp_at( 2,5 , PSTR("not available!"), MNORMAL);

    while ( !( (get_key_press (1 << KEY_ENTER)) || (get_key_press (1 << KEY_ESC))  || (get_key_press (1 << KEY_PLUS))  || (get_key_press (1 << KEY_MINUS))  ) );
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void ShowTitle_P( const char *title, uint8_t clearscreen )
{
    if( clearscreen )
        lcd_cls();

    lcd_frect( 0, 0, 127, 7, 1);                                        // Titel: Invers

    if( strlen_P(title) < 17 )
        show_Lipo();

    lcd_printp_at ( 1, 0, title , MINVERS);
}



//-----------------------------------------------------------
//-----------------------------------------------------------
void Popup_Draw( uint8_t heigthC )
{
    uint8_t y, h;

    h = heigthC*8;
    y = 63-h;

    //lcd_frect(     0, (hC*8)-6, 127, 63-(hC*8)+6, 1); // Box white
    lcd_frect(   0, y-8, 127,   3, 0);      // Box clear
    lcd_frect(   0, y-4, 127, h+4, 1);      // Box fill white
    lcd_plot (   0, y-4, 0);                // Ecke links oben 1
    lcd_plot ( 127, y-4, 0);                // Ecke rechts oben 1

    lcd_line (     1, y-4,   0, y-3, 0);    // Ecke links oben 2
    lcd_line ( 127-1, y-4, 127, y-3, 0);    // Ecke rechts oben 2
}



//-----------------------------------------------------------
// lcdx_cls_rowwidth( y, width, mode, xoffs,yoffs )
//
// loescht eine Zeile auf dem Display
// mode ist kompatibel zu MNORMALX, MINVERSX
//
//  width: in Zeichen
//-----------------------------------------------------------
void lcdx_cls_rowwidth( uint8_t y, uint8_t width, uint8_t mode, int8_t xoffs, int8_t yoffs )
{
    int8_t  xadd = 0;
    int8_t  yadd = 0;

    if( mode == MNORMALX || mode == MINVERSX )
    {
        if( xoffs > 0 )         xadd = 1;
        if( (y*8)+yoffs > 0 )   yadd = 1;

        if( mode == MNORMALX )
            mode = MNORMAL;
        else
            mode = MINVERS;
    }

    lcd_frect( xoffs-xadd, (y*8)+yoffs-yadd, (width*6)+xadd, 7+yadd, (mode == MNORMAL ? 0 : 1) );     // Zeile loeschen
}


//-----------------------------------------------------------
// lcdx_cls_row( y, mode, yoffs )
//
// loescht eine Zeile auf dem Display
// mode ist kompatibel zu MNORMALX, MINVERSX
//-----------------------------------------------------------
void lcdx_cls_row( uint8_t y, uint8_t mode, int8_t yoffs )
{
    lcdx_cls_rowwidth( y, 21, mode, 0, yoffs );
}



//####################################################################################
//####################################################################################



//-----------------------------------------------------------
//-----------------------------------------------------------
void lcd_setpos( uint8_t x, uint8_t y )
{
    lcd_xpos = x;
    lcd_ypos = y;
}



//-----------------------------------------------------------
//-----------------------------------------------------------
void lcd_print_char( uint8_t c, uint8_t mode )
{
    switch( c )
    {
        case 0x0D:  lcd_xpos = 0;
                    break;

        case 0x0A:  new_line();
                    break;

        default:    lcdx_putc (lcd_xpos, lcd_ypos, c, mode,  0,0);
                    lcd_xpos++;
                    if( lcd_xpos > 20 )
                    {
                        lcd_xpos = 0;
                        new_line();
                    }
                    break;
    }
}


//----------------------------------------------------
// gibt einen Linefeed aus (CR+LF)
//----------------------------------------------------
void  lcd_print_LF( void )
{
    lcd_print_char( 0x0D, MNORMAL );
    lcd_print_char( 0x0A, MNORMAL );
}

