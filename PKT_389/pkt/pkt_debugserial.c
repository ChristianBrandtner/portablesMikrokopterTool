/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*
 * pkt_debugserial.c
 *
 *  Created on: 25.09.2015
 *      Author: cebra
 *
 *  Funktionen zum seriellen Debuggen von PKT Funktionen
 *  Ausgabe von Werten
 *  prüfen von Funktionen mit übergabe der Parameter und Rückgabe der Ergebnisse über PKT USB-serial
 *
 *  !!!!! ACHTUNG: funktioniert nur wenn Bluetooth und SV2-Module nicht aktiv sind       !!!!
 *  !!!!!          für die Ausgabe wird die serielle USB-Schnittstelle exclusiv benötigt !!!!
 *
 */

//############################################################################
//# HISTORY  pkt_debugserial.c
//#
//#
//# 25.09.2015 Cebra - NEU
//############################################################################


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include "../main.h"
#include "../lcd/lcd.h"
#include "../utils/xutils.h"            // xprintf, UTCdatetime2local
#include "../uart/uart1.h"
#include "../eeprom/eeprom.h"
#include "pkt_debugserial.h"


/**************************************************************
 *
 * alle benötigten Vorgänge in die Funktion PKT_Debugfunktion einbauen
 *
 * Durch Eingabe von ESC,ESC, 'd' über die serielle
 * USB-Verbindung werden die Debugfunktionen gestartet
 *
 **************************************************************/




void PKT_Debugfunktion(void)

{
   usbx_printp(PSTR("PKT Serial Debug:"),MCRLF);
   usbx_printf (MCRLF,"PKT EEPROM Version: %d",Config.Version);

   usbx_write_gpspos (0x1f1db5fb,MCRLF);
   usbx_printf(MCRLF,"%lx",0x1f1db5fb);
   usbx_printf(MCRLF,"Testwert dezimal: %d ",0xff);
   usbx_printf(MCRLF,"Testwert hexadezimal: %x ",0xff);
   usbx_printf(MCRLF,"Testwert binaer: %b ",0xff);

}


//******************************************************************

//-----------------------------------------------------------
//
// Ausgabe eines String auf USB Serial
//
// Parameter:
//  mode       : MNORMAL, MCRLF, ...
//  format     : String aus RAM (siehe: xprintf in utils/xstring.h)
//
//-----------------------------------------------------------
void usbx_print( uint8_t *text, uint8_t mode)
{
#ifdef DEBUGSERIAL
    while( *text )
    {

        uart1_putc ( *text);
        text++;
    }
    if (mode == MCRLF)
      {
        uart1_putc ('\n');
        uart1_putc ('\r');
      }
#endif
}


//-----------------------------------------------------------
//
// Ausgabe eines String auf USB Serial aus PROGMEM
//
// Parameter:
//  mode       : MNORMAL, MCRLF, ...
//  format     : String aus RAM (siehe: xprintf in utils/xstring.h)
//
//-----------------------------------------------------------
void usbx_printp( const char *text, uint8_t mode)
{
#ifdef DEBUGSERIAL
    char c;



    while( (c = pgm_read_byte(text)) )
    {
        uart1_putc ( c);
        text++;
    }
    if (mode == MCRLF)
      {
        uart1_putc ('\n');
        uart1_putc ('\r');
      }
#endif
}


//-----------------------------------------------------------
// usbx_printf(mode, format, ...)
//
// Ausgabe von n-Parametern via Formattierung
//
// Parameter:
//  mode       : MNORMAL, MINVERS, ...
//  format     : String aus RAM (siehe: xprintf in utils/xstring.h)
//  ...        : Parameter fuer 'format'
//-----------------------------------------------------------
void usbx_printf( uint8_t mode, const char *format, ... )
{
#ifdef DEBUGSERIAL
    va_list ap;

    va_start( ap, format );
    // _xvsnprintf( int useprogmem, char *buffer, size_t n, const char *format, va_list ap )
    _xvsnprintf( false, printf_buffer, PRINTF_BUFFER_SIZE, format, ap );
    va_end(ap);
    usbx_print((unsigned char *)printf_buffer, mode);
#endif
}


//-----------------------------------------------------------
// writes a single gps-pos to USB
//-----------------------------------------------------------

void usbx_write_gpspos( int32_t GPSpos, uint8_t mode )
{
#ifdef DEBUGSERIAL
    usbx_printf(mode,"%3.5ld", GPSpos/100);
#endif
}








