/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*-
 * Copyright (c) 2012,2013 Darran Hunt (darran [at] hunt dot net dot nz)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file WiFly RN-XV Library
 */



//############################################################################
//# HISTORY  PKT_WiFlyHQ.c
//#
//# 03.06.2014 OG
//# - fix: find_WiFly() - Benutzer kann Suche nach WiFly jetzt abbrechen
//#        (z.b. weil er kein WiFly angesteckt hat)
//#        von dieser Aenderungen betroffen sind auch noch einige Codechanges
//#        an anderen Funktionen.
//#
//# 15.04.2014 OG
//# - chg: wifly_init() - Logik bei der Versionsverifikation von WIFLY_ADHOC
//#        geaendert da v2.38 ansonsten ggf. den Bach runter gegangen waere.
//#        AKTUELL UNGETESTET DA KEIN WIFLY ADHOC VORHANDEN!
//# - add: Wifly_update() ein paar Kommentare im Code
//# - add: KOMMENTAR zu define DEBUG_WIFLY ! (lesenswert!)
//#
//# 14.04.2014 Cebra
//# - add: Defines für getestete WiFly Softwareversionen
//# - add: wifly_init bei der Versionsabfrage erweitert
//#
//# 13.04.2014 OG
//# - ggf. Probleme beim Downgrade des WiFly auf v2.38 -> deaktiviert
//# - FAST KOMPLETT NEUER CODE
//#
//# 03.04.2014 OG
//# - add: define DELAY_BEFORE_SAVE
//# - add: define DELAY_BEFORE_REBOOT
//#
//# 02.04.2014 OG
//# WIP: etliche weitere Aenderungen....
//# - chg: getVersion(): Rueckgabe/Logik geaendert
//# - add: define DELAY_AFTER_MODUL_ON
//#
//# 01.04.2014 SH
//# - chg: Nicht benötigte Progmem Strings auskommentiert
//#
//# 11.03.2014 SH
//# - add: neue Funktionen wl_reset() und wl_update()
//# - chg: wl_init() wurde komplett auf den neuen AP Modus geändert, die Init Routine vom Ad Hoc Modus befindet sich ausgeklammert darunter
//# - chg: getVersion wurde angepasst + neuer Wert für resp_Version[] PROGMEM = "RN-171\r\n" anstatt WiFly
//# - chg: setSSID() und setPassphrase() wurden um weiteren Parameter ergänzt
//# - chg: createAdhocNetwork angepasst da Funktion setSSID() enthalten
//# - del: ausgeklammerte Funktionen getConnection, getDHCPMode, getSpaceReplace, getopt_Asc und join
//#
//# 12.02.2014 OG
//# - chg: readTimeout() Auskommentiert: "unused variable 'ind'"
//# - chg: wl_init() Auskommentiert: "unused variable 'dhcpMode'"
//#
//# 02.07.2013 cebra
//# - new: Routinen für WiFly WLAN-Modul
//#
//###########################################################################


#include <avr/pgmspace.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <util/delay.h>
#include <inttypes.h>
#include <string.h>
#include "../main.h"
#include "PKT_WiFlyHQ.h"
#include "../eeprom/eeprom.h"
#include "../uart/uart1.h"
#include "../timer/timer.h"
#include "../lcd/lcd.h"
#include "../messages.h"
#include "../pkt/pkt.h"
#include "../utils/xutils.h"
#include "wifly_setup.h"


#ifdef USE_WLAN


//----------------------------------------------------
// DEBUG
//
// Wenn 'DEBUG_WIFLY' eingeschalte wird dann werden die Ausgaben
// des WiFly auf dem PKT-Screen angezeigt - fast wie bei einem
// Terminal Programm!
//
// Man kann dadurch genaueres erkennen wie die Kommunikation
// ablaeuft und mit welchen Strings beispielsweise das WiFly
// ein Ok quittiert.
//----------------------------------------------------
//#define DEBUG_WIFLY

//----------------------------------------------------
// unterstützte WiFly Softwareversionen
//----------------------------------------------------

#define  Adhoc_Version1   232           // getestet Version 2.32
#define  Adhoc_Version2   238           // getestet Version 2.38

#define  Apmode_Version1  441           // getestet Version 4.41


//----------------------------------------------------
// Einstellungen
//----------------------------------------------------
static const char WIFLY_IP_ADDRESS[]        PROGMEM = "169.254.1.1";
static const char WIFLY_IP_PORT[]           PROGMEM = "2000";

static const char WIFLY_IP_GATEWAY[]        PROGMEM = "169.254.1.1";
static const char WIFLY_IP_NETMASK[]        PROGMEM = "255.255.0.0";

static const char WIFLY_VERSION_ADHOC[]     PROGMEM = "2.38.3";
//static const char WIFLY_VERSION_ADHOC[]     PROGMEM = "v2.32";
static const char WIFLY_VERSION_APMODE[]    PROGMEM = "4.41";


//----------------------------------------------------
// Einsstellungen fuer Delay's
//----------------------------------------------------
//#define WIFLY_DEFAULT_TIMEOUT       500 /* 500 milliseconds */
#define WIFLY_DEFAULT_TIMEOUT       100 // n milliseconds


//----------------------------------------------------
//----------------------------------------------------
uint8_t     progress_max = 0;
uint8_t     progress_act = 0;
char        progress_buffer[22];
char        buffer[20];


char        wifly_version_string[12];
uint16_t    wifly_version;


//----------------------------------------------------
// Texte
//----------------------------------------------------
static const char STR_WIFLY_NOT_FOUND[]     PROGMEM = "WiFly not found";

static const char RECEIVE_AOK[]             PROGMEM = "AOK";

#define REPLACE_SPACE_CHAR  '$'     // ersetzt Spaces in einem String-Parameter des WiFly - '$' ist WiFly default (0x24) fuer SSID und Pwd

//static const char []      PROGMEM = "";

//----------------------------------------------------


//#############################################################################
//#
//#############################################################################


//----------------------------------------------------
//----------------------------------------------------
void debug_begin( void )
{
    lcd_cls();
    lcd_setpos(0,0);
}


//----------------------------------------------------
//----------------------------------------------------
void debug_wait( void )
{
    set_beep( 25, 0xffff, BeepNormal );
    clear_key_all();
    while( !get_key_press(1 << KEY_ESC) && !get_key_press(1 << KEY_ENTER) );
    lcd_print_LF();
}


//----------------------------------------------------
//----------------------------------------------------
void debug_message( uint8_t *text )
{
    lcd_print_LF();
    lcd_print( text, MINVERS );
    debug_wait();
    lcd_print_LF();
}






//#############################################################################
//#
//#############################################################################


/** Read the next character from the WiFly serial interface.
 * Waits up to timeout milliseconds to receive the character.
 * @param chp pointer to store the read character in
 * @param timeout the number of milliseconds to wait for a character
 * @retval true - character read
 * @retval false - timeout reached, character not read
 */
bool readTimeout( char *chp, uint16_t timeout)
{
    char ch;
    timer2 = timeout;

    while( timer2>0 )
    {
        if( uart1_available() > 0 )
        {
            ch = uart1_getc();
            *chp = ch;

            #ifdef DEBUG_WIFLY
            lcd_print_char( ch, MNORMAL);
            #endif

            return (true);
        }
    }

    return (false);
}




//--------------------------------------------------------------
//--------------------------------------------------------------
void flushRx( int timeout )
{
    char ch;
    while( readTimeout( &ch, timeout) );
}




/**
 * Read characters from the WiFly and match them against the
 * progmem string. Ignore any leading characters that don't match. Keep
 * reading, discarding the input, until the string is matched
 * or until no characters are received for the timeout duration.
 * @param str The string to match, in progmem.
 * @param timeout fail if no data received for this period (in milliseconds).
 * @retval true - a match was found
 * @retval false - no match found, timeout reached
 */
bool match_P( const char *str, uint16_t timeout)
{
    const char *match = str;
    char ch, ch_P;

    ch_P = pgm_read_byte(match);
    if( ch_P == '\0' ) return true; /* Null string always matches */

    while( readTimeout( &ch, timeout) )
    {
        // DEBUG
        //lcd_print_char( ch, MNORMAL );

        if( ch == ch_P ) match++;
        else
        {
            match = str; // Restart match
            if( ch == pgm_read_byte(match) ) match++;
        }

        ch_P = pgm_read_byte( match );
        if( ch_P == '\0' ) return (true);
    }

    return (false);
}






//#############################################################################
//# progress
//#############################################################################

//--------------------------------------------------------------
// msg: PROGMEM
//--------------------------------------------------------------
void progress_begin( uint8_t maxsteps )
{
    progress_max = maxsteps;
    progress_act = 0;
}


//--------------------------------------------------------------
// wait4key: true / false
//--------------------------------------------------------------
void progress_end( uint8_t wait4key )
{
    progress_max = 0;
    progress_act = 0;

    if( wait4key )
    {
        lcd_printp_at( 12, 7, strGet(ENDE), MNORMAL);                   // Keyline
        set_beep( 25, 0xffff, BeepNormal );                             // kurzer Bestaetigungs-Beep
        clear_key_all();
        while( !get_key_press(1 << KEY_ESC) );                          // warte auf Taste...
    }
}



//--------------------------------------------------------------
// msg: PROGMEM
//--------------------------------------------------------------
void progress_show( const char *msg )
{
    int8_t  yoffs = 3;

    #ifdef DEBUG_WIFLY
    return;
    #endif

    if( progress_max )
    {
        progress_act++;

        lcdx_printf_center_P( 4, MNORMAL, 0,yoffs, PSTR("%d/%d"), progress_act, progress_max );

        if( msg )
        {
            strncpyat_P( progress_buffer, msg, 20, ' ', 2);

            lcd_frect( 2, (5*8)+yoffs, 124, 7, 0);                                          // Zeile 5 loeschen
            lcdx_print_center( 5, (uint8_t *)progress_buffer, MNORMAL, 0,yoffs);            // Text zentriert; String im RAM
        }

        lcd_rect_round( 0, 28+yoffs, 127, 22, 1, R2);                                       // Rahmen
        _delay_ms(700);                                                                     // fuer Anzeige (Entschleunigung)
    }
}



//--------------------------------------------------------------
// gibt zentriert einen Text und loescht vorher die Zeile
//--------------------------------------------------------------
void MsgCenter_P( uint8_t y, const char *text, uint8_t mode, int8_t yoffs )
{
    lcdx_cls_row( y, MNORMAL, yoffs );
    lcdx_printp_center( y, text, MNORMAL, 0, yoffs );
}



//#############################################################################
//# sendcmd
//#############################################################################


//--------------------------------------------------------------
//--------------------------------------------------------------
void cmd_showerror( const char *cmdstr)
{
    //void PKT_Message_P( const char *text, uint8_t error, uint16_t timeout, uint8_t beep, uint8_t clearscreen )
    PKT_Message_P( cmdstr, true, 3000, true, true );        // max. 30 Sekunden anzeigen
}



//--------------------------------------------------------------
//--------------------------------------------------------------
uint8_t find_Prompt()
{
    char c;
    uint8_t state = 0;

    while( readTimeout( &c, WIFLY_DEFAULT_TIMEOUT) )
    {
        if( state == 0 && (c == '>') )  { state = 1; continue; }
        if( state == 1 && (c == ' ') )  return true;

        if( state == 1 ) break;     // Fehler: nicht vorgesehen
    }

    return false;
}


//--------------------------------------------------------------
// nicht verwendet...
//--------------------------------------------------------------
uint8_t find_Prompt2()
{
    char c;

    timer2 = 800;  // 8 Sekunden max. suchen
    while( readTimeout( &c, WIFLY_DEFAULT_TIMEOUT) && timer2 )
    {
        if( c == '>' )  return true;
    }

    return false;
}



//--------------------------------------------------------------
// Das ist erstmal so uebernommen - ob das alles sein muss??
//
// Put the WiFly into command mode
//
// RUECKGABE:
//  >0: ok
//  =0: Fehler
//  -1: Abbruch Benutzer
//--------------------------------------------------------------
int8_t cmd_EnterCommandMode( void )
{
    uint8_t retry  = 2;
    int8_t  result = 0;     // false

    while( retry && !result )
    {
        //_delay_ms(250);
        uart1_puts_p( PSTR("$$$") );
        //_delay_ms(250);
        _delay_ms(100);
        uart1_putc('\r');

        if( get_key_short(1 << KEY_ESC) )       // Abbruch durch Benutzer?
        {
            return -1;                          //   -1 = Abbruch Benutzer
        }

        result = find_Prompt();                 // ggf. result = true
        if( !result )
        {
            retry--;
            _delay_ms(250);
        }
    }

    return result;
}



//--------------------------------------------------------------
// PARAMETER:
//   cmdstr       : PROGMEM
//   strvalue     : RAM
//   match_receive: PROGMEM
//   findprompt   : true/false
//--------------------------------------------------------------
uint8_t cmdsend( const char *cmdstr, const char *strvalue, const char *match_receive, uint8_t findprompt )
{
    uint8_t result = true;
    const char *p;
    char c;

    progress_show( cmdstr );

    //--------------------
    // Senden...
    //--------------------
    uart1_puts_p( cmdstr );

    if( strvalue != NULL )
    {
        uart1_putc(' ');

        p = strvalue;
        while( *p )
        {
            c = *p;
            if( c == ' ' ) c = REPLACE_SPACE_CHAR;
            uart1_putc( c );
            p++;
        }
    }
    uart1_putc('\r');


    //--------------------
    // Antwort...
    //--------------------
    if( match_receive )
    {
        //if( !match_P( p, WIFLY_DEFAULT_TIMEOUT) )
        if( !match_P( match_receive, 2000) )
        {
            cmd_showerror( cmdstr );                // Fehler auf PKT Screen anzeigen und durch Benutzer bestaetigen lassen
            result = false;
        }
        // DEBUG
        //lcdx_printf_at_P( 2, 7, MINVERS,  0,0, PSTR("%d"), result );
    }

    #ifdef DEBUG_WIFLY
    debug_wait();
    #endif

    // TODO: Kommandos die keinen Prompt zurueckliefern? (evtl. 'reset'?)
    //uart1_putc('\r'); // fuer cmd_Join ein extra Return... (bei den anderen Kommandos stoert es nicht)

    if( result && findprompt)
    {
        result = find_Prompt();
    }

    return( result );
}



//--------------------------------------------------------------
// cmdsend_P()
//
// PARAMETER:
//--------------------------------------------------------------
uint8_t cmdsend_P( const char *cmdstr, const char *strvalue, const char *match_receive, uint8_t findprompt )
{
    strncpy_P( buffer, strvalue, 20);
    return cmdsend( cmdstr, buffer, match_receive, findprompt );
}



//--------------------------------------------------------------
// Version wird anhand des Prompt's ermittelt
// Implementiert als State-Machine
//--------------------------------------------------------------
uint8_t cmd_GetVersion( void )
{
    char    c;
    uint8_t state = 0;
    char    *dst = wifly_version_string;
    uint8_t n = 0;

    wifly_version = 0;

    uart1_putc('\r');                                       // erzwinge Prompt

    while( readTimeout( &c, WIFLY_DEFAULT_TIMEOUT) )
    {
        if( (state == 0) && (c == '<') )
        {
            state = 1;
            continue;
        }

        if( (state == 1) && (c != '>') )
        {
            // TODO: Ende vom Prompt im Fehlerfall catchen...
            if( n+1 >= sizeof(wifly_version_string) )
                break;
            *dst = c;
            dst++;
            n++;
            continue;
        }

        if( (state == 1) && (c == '>') )                    //  Versionsstring-Ende erreicht
        {
            *dst = 0;
            state = 2;
            break;
        }

    }


    if( state == 2 )
    {
        //uint16_t  wifly_version;
        wifly_version  = (wifly_version_string[0]-'0') * 100;
        wifly_version += (wifly_version_string[2]-'0') * 10;
        wifly_version += (wifly_version_string[3]-'0') * 1;
    }
    else
    {
        cmd_showerror( PSTR("get version") );               // Fehler auf PKT Screen anzeigen und durch Benutzer bestaetigen lassen
    }

    return (state == 2);
}



//--------------------------------------------------------------
// cmd_Set()
//
// PARAMETER:
//   cmdstr: PROGMEM
//--------------------------------------------------------------
uint8_t cmd_Set( const char *cmdstr )
{
    return cmdsend( cmdstr, 0, RECEIVE_AOK, true );
}



//--------------------------------------------------------------
// cmd_SetStr()
//
// PARAMETER:
//   cmdstr: PROGMEM
//   str   : RAM
//--------------------------------------------------------------
uint8_t cmd_SetStr( const char *cmdstr, const char *str )
{
    return cmdsend( cmdstr, str, RECEIVE_AOK, true );
}



//--------------------------------------------------------------
// cmd_SetStr()
//
// PARAMETER:
//   cmdstr: PROGMEM
//   str   : PROGMEM
//--------------------------------------------------------------
uint8_t cmd_SetStr_P( const char *cmdstr, const char *str )
{
    return cmdsend_P( cmdstr, str, RECEIVE_AOK, true );
}



//--------------------------------------------------------------
// cmd_SetNum()
//
// PARAMETER:
//   cmdstr: PROGMEM
//   value : Dezimal
//--------------------------------------------------------------
uint8_t cmd_SetNum( const char *cmdstr, const uint16_t value )
{
    utoa( value, buffer, 10 );
    return cmd_SetStr( cmdstr, buffer);
}



//--------------------------------------------------------------
// cmd_Reset()
//--------------------------------------------------------------
uint8_t cmd_Reset( void )
{
    uint8_t result = true;

    //_delay_ms( 500 );
    result = cmdsend( PSTR("factory RESET"), 0, PSTR("Defaults"), false );
    _delay_ms( 1000 );

    return result;

//  return cmdsend( PSTR("factory RESET"), 0, PSTR("Defaults"), true );
}



//--------------------------------------------------------------
// cmd_Save()
//--------------------------------------------------------------
uint8_t cmd_Save( void )
{
    //_delay_ms( DELAY_BEFORE_SAVE );
    _delay_ms( 200 );
    return cmdsend( PSTR("save"), 0, PSTR("Storing"), true );
}


//--------------------------------------------------------------
// cmd_Join()
//--------------------------------------------------------------
uint8_t cmd_Join( const char *ssid )
{
    uint8_t result = true;

    result = cmdsend( PSTR("join"), ssid, PSTR("Associated"), false );
    if( result )    uart1_putc('\r');;
    if( result )    result = find_Prompt();

    return result;
}



//--------------------------------------------------------------
// cmd_Reboot()
//--------------------------------------------------------------
uint8_t cmd_Reboot( void )
{
    //_delay_ms( DELAY_BEFORE_REBOOT );
    _delay_ms( 300 );
    cmdsend( PSTR("reboot"), 0, 0, false );
    _delay_ms( 2000 );
    return true;
}



//#############################################################################
//# WiFly suchen
//#############################################################################


//--------------------------------------------------------------
//--------------------------------------------------------------
void find_WiFly_searchmsg( const char *text, uint8_t baud )
{
    lcd_frect( 0, 2*8, 126, 7, 0);                                                              // Zeile 2 loeschen
    lcdx_printf_center_P( 2, MNORMAL, 0,0, PSTR("%S %lu Baud"), text, Baud_to_uint32(baud) );   // Text zentriert
}



//--------------------------------------------------------------
// RUECKGABE:
//  >0: ok
//  =0: Fehler
//  -1: Abbruch Benutzer
//--------------------------------------------------------------
int8_t find_WiFly_baud( uint8_t baud )
{
    int8_t result = 1;

    find_WiFly_searchmsg( strGet(STR_SEARCH), baud );

    // Anmerkung 06.04.2014 OG:
    // SetBaudUart1() (in uart/uart1.c) setzt nur die Baudrate des Uart.
    // Config.PKT_Baudrate wird durch SetBaudUart1() NICHT geaendert!
    SetBaudUart1( baud );

    result = cmd_EnterCommandMode();

    if( result > 0 )
    {
        return baud;                        // Baud gefunden, ok!
    }

    return result;                          // nicht gefunden oder Abbruch durch Benutzer
}



//--------------------------------------------------------------
// ok = find_WiFly()
//
// Sucht das WiFly mit verschiedenen Baudraten.
//
// Wenn das WiFly mit von Config.PKT_Baudrate (= 57600) abweichender
// Baudrate gefunden wird (z.B. 9600 Baud) dann wird das WiFly
// automatisch auf 57600 Baud umkonfiguriert damit nachfolgende
// Funktionen das nicht mehr beachten muessen.
//
// RUECKGABE:
//  ok = true : WiFly gefunden und ist auf 57600 Baud
//  ok = false: kein WiFly gefunden
//
//  >0: ok
//  =0: Fehler
//  -1: Abbruch Benutzer
//
// Anmerkung:
//  #define Baud_9600       1
//  #define Baud_19200      2
//  #define Baud_38400      3
//  #define Baud_57600      4
//  #define Baud_115200     5
//  #define Baud_4800       6
//  #define Baud_2400       7
//--------------------------------------------------------------
int8_t find_WiFly( void )  // search Wifly with all Baudrates
{
    int8_t result = 1;                                                          // 1 == true
    int8_t baud   = 0;

    //inCommandMode = false;
    //exitCommand   = 0;

    set_Modul_On( Wlan );                                                       // WiFly einschalten

    timer2 = 60;                                                                // Delay damit das WiFly booten kann
    while( timer2 );

    lcd_printp_at(12, 7, strGet(ENDE), MNORMAL);                                // Keyline: "Ende"

    //------------------------
    // suche WiFly mit versch. Baudraten
    //------------------------
    if( baud==0 ) baud = find_WiFly_baud( Baud_57600  );
    if( baud==0 ) baud = find_WiFly_baud( Baud_9600   );
    if( baud==0 ) baud = find_WiFly_baud( Baud_115200 );
    if( baud==0 ) baud = find_WiFly_baud( Baud_38400  );
    if( baud==0 ) baud = find_WiFly_baud( Baud_19200  );
    if( baud==0 ) baud = find_WiFly_baud( Baud_4800   );
    if( baud==0 ) baud = find_WiFly_baud( Baud_2400   );

    lcdx_cls_row( 7, MNORMAL, 0);                                               // Keyline loeschen

    //------------------------
    // Abbruch durch Benutzer?
    //------------------------
    if( baud < 0 )
    {
        SetBaudUart1( Config.PKT_Baudrate );                                    // orginal Baudrate von Uart1 wieder herstellen
        set_Modul_On( USB );                                                    // wieder auf USB zurueckschalten
        return -1;                                                              // -1 = Abbruch Benutzer
    }


    //------------------------
    // kein WiFly gefunden...
    //------------------------
    if( baud==0 )                                                               // WiFly nicht gefunden :-(
    {
        SetBaudUart1( Config.PKT_Baudrate );                                    // orginal Baudrate von Uart1 wieder herstellen
        set_Modul_On( USB );                                                    // wieder auf USB zurueckschalten

        //PKT_Message_P( *text, error, timeout, beep, clearscreen )
        PKT_Message_P( STR_WIFLY_NOT_FOUND, true, 3000, true, true );           // max. 30 Sekunden anzeigen

        return false;                                                           // 0 = nicht gefunden
    }


    //------------------------
    // WiFly gefunden!
    //------------------------
    find_WiFly_searchmsg( strGet(STR_FOUND), baud );                            // Anzeige: "gefunden..."


    //------------------------
    // WiFly Baud ok?
    // -> ggf. WiFly Baud umkonfigurieren
    //------------------------
    if( Config.PKT_Baudrate != baud )
    {
        find_WiFly_searchmsg( strGet(STR_SET), Config.PKT_Baudrate );           // Anzeige: "setze..."

        if( result )    result = cmd_Set( PSTR("set u b 57600") );              // -> etwas unsauber hier einfach 57600 zu setzen und nicht Config.PKT_Baudrate
        if( result )    result = cmd_Save();
        if( result )    result = cmd_Reboot();                                  // true = uart-baud setzen

        SetBaudUart1( Config.PKT_Baudrate );                                    // orginal Baudrate von Uart1 wieder herstellen
        if( result )    result = cmd_EnterCommandMode();
    }


    return result;
}



//#############################################################################
//#
//#############################################################################


//--------------------------------------------------------------
// zeigt die WiFly-Version an
//--------------------------------------------------------------
uint8_t wifly_version_show( void )
{
    int8_t result = 1;                                                  // 1 = true

    ShowTitle_P( PSTR("WiFly Version"), true );

    result = find_WiFly();

    if( result < 0 )                                                    // Abbruch durch Benutzer?
    {
        clear_key_all();
        return result;
    }

    if( result ) result = cmd_GetVersion();                             // Ergebnis in 'wifly_version_string'

    set_Modul_On( USB );

    if( result )
    {
        // TODO:
        // Anzeige ob WiFly-Version kompatibel zum gewaehlten
        // Mode (AP-Mode, AdHoc) ist
        //lcd_frect( 0, 2*8, 126, 7, 0);                                // Zeile 2 loeschen

        lcd_rect_round( 0, 8*4+3, 127, 8*2, 1, R2);                     // Anzeige-Rahmen
        lcdx_printf_center_P( 5, MNORMAL, 0,0, PSTR("Version: v%s"), wifly_version_string );

        lcd_printp_at( 12, 7, strGet(ENDE), MNORMAL);                   // Keyline
        set_beep( 25, 0xffff, BeepNormal );                             // kurzer Bestaetigungs-Beep

        clear_key_all();
        while( !get_key_press(1 << KEY_ESC) );                          // warte auf Taste...
    }
    else
    {
        PKT_Message_P( PSTR("Version FAIL!"), true, 3000, true, true );     // max. 30 Sekunden anzeigen
    }

    clear_key_all();
    return result;
}




//--------------------------------------------------------------
//--------------------------------------------------------------
uint8_t _ask( uint8_t keyline )
{
    uint8_t     ask = 0;

    lcd_rect_round( 0, 34, 127, 16, 1, R2);                         // Rahmen

    lcd_printp_at( 12, 7, strGet(keyline), MNORMAL);                // Keyline

    if( keyline == ENDE )
        set_beep( 1000, 0xffff, BeepNormal );                       // langer Error-Beep
    else
        set_beep( 25, 0xffff, BeepNormal );                         // kurzer Bestaetigungs-Beep

    clear_key_all();
    while( !ask )                                                   // warte auf Ja/Nein
    {
        if( get_key_press(1 << KEY_ENTER) ) ask = 1;                // "Ja"
        if( get_key_press(1 << KEY_ESC) )   ask = 2;                // "Nein"
    };

    return( ask==1 );
}




//--------------------------------------------------------------
//--------------------------------------------------------------
uint8_t wifly_init_perform( uint8_t wiflymode ) //AP Mode
{
    uint8_t     result = true;

    #ifndef DEBUG_WIFLY
    MsgCenter_P( 2, PSTR("INIT"), MNORMAL, 0 );
    #endif


    //---------------
    // WIFLY_ADHOC
    //---------------
    if( wiflymode == WIFLY_ADHOC )
    {
        progress_begin( 9 );

        //if( result ) result = cmd_Set(    PSTR("set u m 1") );                        // ist das notwendig? - (turn off echo)
        //if( result ) result = cmd_Set(    PSTR("set sys printlvl 0") );               // ist das notwendig?
        //if( result ) result = cmd_Set(    PSTR("set comm remote 0") );                // ist das notwendig?

        if( result ) result = cmd_Set(      PSTR("set wlan join 4") );                  // 4 = AdHoc
        if( result ) result = cmd_Set(      PSTR("set ip dhcp 0") );

        if( result ) result = cmd_SetStr_P( PSTR("set ip address"), WIFLY_IP_ADDRESS );
        if( result ) result = cmd_SetStr_P( PSTR("set ip gateway"), WIFLY_IP_GATEWAY );
        if( result ) result = cmd_SetStr_P( PSTR("set ip netmask"), WIFLY_IP_NETMASK );

        if( result ) result = cmd_SetStr(   PSTR("set wlan ssid"), Config.Wlan_SSID );
        if( result ) result = cmd_SetNum(   PSTR("set wlan chan"), Config.Wlan_Channel );
    }


    //---------------
    // WIFLY_APMODE
    //---------------
    if( wiflymode == WIFLY_APMODE )
    {
        progress_begin( 13 );

        if( result ) result = cmd_Set(      PSTR("set wlan join 7") );
        if( result ) result = cmd_Set(      PSTR("set ip dhcp 4") );

        if( result ) result = cmd_SetStr_P( PSTR("set ip address"), WIFLY_IP_ADDRESS );
        if( result ) result = cmd_SetStr_P( PSTR("set ip gateway"), WIFLY_IP_GATEWAY );
        if( result ) result = cmd_SetStr_P( PSTR("set ip netmask"), WIFLY_IP_NETMASK );

        if( result ) result = cmd_Set(      PSTR("set wlan rate 3") );
        if( result ) result = cmd_SetStr(   PSTR("set apmode ssid") , Config.Wlan_SSID );
        if( result ) result = cmd_SetStr(   PSTR("set apmode pass") , Config.Wlan_Password );
        if( result ) result = cmd_SetNum(   PSTR("set wlan channel"), Config.Wlan_Channel );
        if( result ) result = cmd_Set(      PSTR("set ip protocol 0x3") );
        if( result ) result = cmd_Set(      PSTR("set apmode link_monitor 0") );
    }

    if( result ) result = cmd_Save();
    if( result ) result = cmd_Reboot();

    progress_end( false );                              // Progress abschalten

    return( result );
}




//--------------------------------------------------------------
// ok = wifly_init( wiflymode )
//
// initialisiert das WiFly fuer AP-Mode oder AdHoc
//
// PARAMETER:
//   wiflymode: WIFLY_APMODE = Firmware wifly7-441.img
//              WIFLY_ADHOC  = Firmware wifly7-2383.img
//--------------------------------------------------------------
uint8_t wifly_init( uint8_t wiflymode ) //AP Mode
{
    int8_t      result = 1;                                             // 1 = true
    uint8_t     ask = false;
    uint8_t     versionerror = false;
    char        *title = (char *)PSTR("Init WiFly");
    char        *strMode;

    if( wiflymode == WIFLY_ADHOC )  strMode = (char *)PSTR("AdHoc");
    else                            strMode = (char *)PSTR("AP-Mode");

    ShowTitle_P( title, true );

    result = find_WiFly();

    if( result < 0 )                                                    // Abbruch durch Benutzer?
    {
        clear_key_all();
        return result;
    }


    if( result ) result = cmd_GetVersion();

    //-----------------------------
    // Version pruefen
    //-----------------------------
    if( result )
    {
        //--------
        // ADHOC
        //--------
        if( wiflymode == WIFLY_ADHOC )
        {
            if(    (wifly_version != Adhoc_Version1)    // 2.32
                && (wifly_version != Adhoc_Version2)    // 2.38
              )
            {
                versionerror = true;
            }
        }

        //--------
        // APMODE
        //--------
        if( wiflymode == WIFLY_APMODE && wifly_version != Apmode_Version1 ) versionerror = true;

        if( versionerror )
        {
            set_Modul_On( USB );
            ShowTitle_P( title, true );
            lcdx_printf_center_P( 2, MNORMAL, 0, -4, PSTR("WiFly v%s"), wifly_version_string );
            lcdx_printf_center_P( 3, MNORMAL, 0, -1, PSTR("ERROR") );
            lcdx_printf_center_P( 5, MNORMAL, 0, -1, PSTR("Version no %S!"), strMode );         // "Version no AP-Mode" / "AdHoc"
            ask = _ask( ENDE );

            return false;
        }
    }


    //-----------------------------
    // Benutzer fragen
    //-----------------------------
    if( result )
    {
        ShowTitle_P( title, true );
        //lcdx_printf_at_P( 0, 1, MINVERS,  0,0, PSTR("%d"), wifly_version );

        lcdx_printf_center_P( 2, MNORMAL, 0, -4, PSTR("WiFly v%s"), wifly_version_string );     // WiFly Versionsanzeige
        lcdx_printf_center_P( 3, MNORMAL, 0, -2, PSTR("%S: OK")   , strMode );                  // "AP-Mode" oder "AdHoc"
        lcdx_printf_center_P( 5, MNORMAL, 0, -1, PSTR("%S?")      , title );                    // "Init WiFly?"

        ask = _ask( NOYES );

        if( !ask )  // "Nein"
        {
            set_Modul_On( USB );
            return false;
        }
    }


    //-----------------------------
    // Init durchfuehren
    //-----------------------------
    if( result )
    {
        ShowTitle_P( title, true );
        result = wifly_init_perform( wiflymode );
    }

    set_Modul_On( USB );

    if( !result )
        PKT_Message_P( PSTR("Init FAIL!"), true, 3000, true, true );        // max. 30 Sekunden anzeigen
    else
        PKT_Message_P( PSTR("Init OK!"), false, 3000, true, true );     // max. 30 Sekunden anzeigen

    clear_key_all();
    return( result );
}



//--------------------------------------------------------------
// PARAMETER:
//   wiflymode: WIFLY_APMODE = Firmware wifly7-441.img
//              WIFLY_ADHOC  = Firmware wifly7-2383.img
//--------------------------------------------------------------
uint8_t wifly_update( uint8_t wiflymode )
{
    int8_t      result = 1;                                             // 1 = true
    uint8_t     ask    = false;
    char        *title = (char *)PSTR("Update WiFly");
    const char  *pStr;


    ShowTitle_P( title, true );

    result = find_WiFly();

    if( result < 0 )                                                    // Abbruch durch Benutzer?
    {
        clear_key_all();
        return result;
    }


    if( result ) result = cmd_GetVersion();

    //-----------------------------
    // Benutzer fragen
    //-----------------------------
    if( result )
    {
        if( wiflymode == WIFLY_ADHOC )  pStr = WIFLY_VERSION_ADHOC;
        else                            pStr = WIFLY_VERSION_APMODE;

        ShowTitle_P( title, true );
        //lcdx_printf_at_P( 0, 1, MINVERS,  0,0, PSTR("%d"), wifly_version );
        lcdx_printf_center_P( 2, MNORMAL, 0, -4, PSTR("%S: v%s"), strGet(STR_VON), wifly_version_string );
        lcdx_printf_center_P( 3, MNORMAL, 0, -2, PSTR("%S: v%S"), strGet(STR_NACH), pStr );
        lcdx_printf_center_P( 5, MNORMAL, 0, -1, PSTR("%S?"), title );

        ask = _ask( NOYES );

        if( !ask )  // "Nein"
        {
            set_Modul_On( USB );
            return false;
        }
    }


    //-----------------------------
    // Update durchfuehren
    //-----------------------------
    if( result )
    {
        ShowTitle_P( title, true );
        progress_begin( 16 );
    }

    //----------------------------
    // 1. Reset
    //----------------------------
    #ifndef DEBUG_WIFLY
    if( result ) MsgCenter_P( 2, PSTR("Reset"), MNORMAL, 0 );
    #endif
    if( result ) result = cmd_Reset();
    if( result ) result = cmd_Reboot();
    if( result ) result = find_WiFly();

    //----------------------------
    // 2. mit Heimnetz verbinden
    //----------------------------
    #ifndef DEBUG_WIFLY
    if( result ) MsgCenter_P( 2, PSTR("verbinde Heimnetz"), MNORMAL, 0 );
    #endif
    if( result ) result = cmd_Set(      PSTR("set ip dhcp 1") );
    if( result ) result = cmd_SetStr(   PSTR("set wlan phrase"), Config.Wlan_HomePassword );
    if( result ) result = cmd_Set(      PSTR("set wlan channel 0") );

    if( result ) result = cmd_Join( Config.Wlan_HomeSSID );

    //----------------------------
    // 3. FTP Download & Update
    //----------------------------
    #ifndef DEBUG_WIFLY
    if( result ) MsgCenter_P( 2, PSTR("FTP Update"), MNORMAL, 0 );
    #endif
    if( result ) result = cmd_Set( PSTR("set ftp address 198.175.253.161") );
    if( result ) result = cmd_Set( PSTR("set ftp dir public") );
    if( result ) result = cmd_Set( PSTR("set ftp user roving") );
    if( result ) result = cmd_Set( PSTR("set ftp pass Pass123") );
    if( result ) result = cmd_Set( PSTR("set ftp timeout 800") );

    //if( result ) result = cmd_Set( PSTR("del config") );

    if( result )
    {
        //-----
        // das kann einiges gekuerzt werden wenn das denn zuverlaessig laufen wuerde...
        //
        // hier wird auf v4.40 geprueft da laut Refrenz-Manual erst seit dieser Version
        // "cupdate" unterstuetzt wird. Bei einem Downgraden von v4.4 auf eine aeltere
        // Version ist ein clean des Dateisystems ('c') erforderlich da sonst das WiFly
        // nur via Hardware-Taster an GPIO9 wiedre zum Leben erweckt werden kann.
        //
        // Anmerkung: evtl. wuer auch ein "del config" reichen - das ist aber nicht
        // hinreichend getestet
        //-----
        if( wifly_version >= 440 )
        {
        if( wiflymode == WIFLY_ADHOC )
            result = cmdsend( PSTR("ftp cupdate wifly7-2383.img"), 0, PSTR("UPDATE OK"), false );
        else
            result = cmdsend( PSTR("ftp cupdate wifly7-441.img") , 0, PSTR("UPDATE OK"), false );
        }
        else
        {
        if( wiflymode == WIFLY_ADHOC )
            result = cmdsend( PSTR("ftp update wifly7-2383.img"), 0, PSTR("UPDATE OK"), false );
        else
            result = cmdsend( PSTR("ftp update wifly7-441.img") , 0, PSTR("UPDATE OK"), false );
        }
    }

    if( result ) result = cmd_Reset();
    if( result ) { timer2 = 50; while(timer2); };


/*
    //-----
    // Baustelle / Experimente bzgl. FTP-Update
    //-----
    //debug_message("FTP END");
    //debug_message("RESET");
    //if( result ) result = cmd_Reboot();
    if( result ) { timer2 = 100; while(timer2); };
    if( result ) { timer2 = 200; while(timer2); };
    debug_message("FINDPR");
    uart1_putc('\r');
    if( result ) result = find_Prompt();
    if( result )    debug_message("FINDPR END: 1");
    else            debug_message("FINDPR END: 0");
*/


    //progress_end( true );                             // Progress abschalten & warten
    progress_end( false );                              // Progress abschalten


    //----------------------------
    // 4. Init
    //----------------------------
    /*
    if( result ) { timer2 = 10; while(timer2); };
    if( result ) set_Modul_On( USB );
    if( result ) { timer2 = 10; while(timer2); };
    if( result ) ShowTitle_P( title, true );
    if( result ) result = result = find_WiFly();
    if( result ) result = wifly_init_perform( wiflymode );
    */

    set_Modul_On( USB );


    if( !result )
        PKT_Message_P( PSTR("Update FAIL!"), true, 3000, true, true );      // max. 30 Sekunden anzeigen
    else
        PKT_Message_P( PSTR("Update OK!"), false, 3000, true, true );       // max. 30 Sekunden anzeigen


    clear_key_all();
    return result;
}



//--------------------------------------------------------------
//--------------------------------------------------------------
uint8_t wifly_reset( void )
{
    int8_t      result = 1;                                             // 1 = true
    uint8_t     ask    = false;
    char        *title = (char *)PSTR("Reset WiFly");

    ShowTitle_P( title, true );

    result = find_WiFly();

    if( result < 0 )                                                    // Abbruch durch Benutzer?
    {
        clear_key_all();
        return result;
    }


    //-----------------------------
    // Benutzer fragen
    //-----------------------------
    if( result )
    {
        ShowTitle_P( title, true );
        lcdx_printf_center_P( 5, MNORMAL, 0, -1, PSTR("%S?"), title );

        ask = _ask( NOYES );

        if( !ask )  // "Nein"
        {
            set_Modul_On( USB );
            return false;
        }
    }

    ShowTitle_P( title, true );

    if( result ) progress_begin( 2 );

    if( result ) result = cmd_Reset();
    if( result ) result = cmd_Reboot();

    progress_end( false );                              // Progress abschalten

    set_Modul_On( USB );

    if( result )
    {
        //PKT_Message_P( *text, error, timeout, beep, clearscreen )
        PKT_Message_P( PSTR("Reset OK!"), false, 3000, true, true );        // max. 30 Sekunden anzeigen
    }
    else
    {
        PKT_Message_P( PSTR("Reset FAIL!"), true, 3000, true, true );       // max. 30 Sekunden anzeigen
    }

    clear_key_all();
    return result;
}



//#############################################################################
//# TEST / DEBUG
//#############################################################################


//--------------------------------------------------------------
// nur zum testen!
//--------------------------------------------------------------
uint8_t wifly_test( void )
{
    uint8_t result = true;

    return result;
}

#endif

