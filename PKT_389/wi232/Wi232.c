/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*****************************************************************************
 *   Copyright (C) 2008 Thomas Kaiser, thomas@ft-fanpage.de                  *
 *   Copyright (C) 2009 Peter "woggle" Mack, mac@denich.net                  *
 *   Copyright (C) 2011 Christian "Cebra" Brandtner, brandtner@brandtner.net *
 *   Copyright (C) 2011 Harald Bongartz                                      *
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
 *                                                                           *
 *   Credits to:                                                             *
 *   Holger Buss & Ingo Busker from mikrokopter.de for the MK project + SVN  *
 *                          http://www.mikrokopter.de                        *
 *   Gregor "killagreg" Stobrawa for his version of the MK code              *
 *   Thomas Kaiser "thkais" for the original project. See                    *
 *                          http://www.ft-fanpage.de/mikrokopter/            *
 *                          http://forum.mikrokopter.de/topic-4061-1.html    *
 *   Claas Anders "CaScAdE" Rathje for providing the font and his C-OSD code *
 *                          http://www.mylifesucks.de/oss/c-osd/             *
 *   Harald Bongartz "HaraldB" for providing his Ideas and Code for usibility*
 *****************************************************************************/


//############################################################################
//# HISTORY  Wi232.c
//#
//# 17.11.2015 Cebra
//# - chg: Funktion Wi232_Initalize um Rückgabewert erweitert, wird im Setup benötigt
//#
//# 15.06.2014 OG
//# -chg: umgestellt auf PKT_Progress_Init() / PKT_Progress_Next()
//#
//# 10.06.2014 OG
//#        KOMPLETTE SANIERUNG ab v3.80cX5
//#        (ehemaliger Code in v3.80cX4 oder frueher)
//# - add: Wi232_ConfigPC() - ehemals in connect.c als Port_USB2CFG_Wi()
//#
//# 06.06.2014 OG
//# - add: Wi232_Write_Progress()
//# - chg: Codeformattierung
//#
//# 05.06.2014 OG
//# -chg: Codeformattierung
//#
//# 14.05.2014 OG
//# - chg: InitErrorWi232(), InitWi232() - umgestellt auf dezimale Anzeige des
//#        Fehlercodes damit der einfacher im Source zu identifizieren ist
//# - add: Source-Historie ergaenzt
//############################################################################


#include "../cpu.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "../messages.h"
#include "../lcd/lcd.h"
#include "../uart/usart.h"
#include "../uart/uart1.h"
#include "../main.h"
#include "../wi232/Wi232.h"
#include "../timer/timer.h"
#include "../eeprom/eeprom.h"
#include "../setup/setup.h"
#include "../pkt/pkt.h"
#include "../utils/xutils.h"
#include "../lipo/lipo.h"
#include "../utils/menuctrl.h"


//############################################################################
//############################################################################


static const char STR_WI232INIT[]    PROGMEM = "Wi.232 Init";


//############################################################################
//############################################################################


//--------------------------------------------------------------
// lError = WriteWi232(  Wi232Register, RegisterValue)
//
// set Register to Wi232, Register, Value
//
// Returns:
//   0 = ACK, FF = NAK
//--------------------------------------------------------------
uint8_t Wi232_Write( uint8_t Wi232Register, uint8_t RegisterValue )
{
    uint8_t timeout = 10;
    uint8_t tc      = 0;
    unsigned int v;


    if( RegisterValue < 0x80 )
    {
        USART_putc(0xff);
        USART_putc(0x02);
        USART_putc(Wi232Register);
        USART_putc(RegisterValue);
    }
    else    // RegisterValue >= 0x80
    {
        USART_putc(0xff);
        USART_putc(0x03);
        USART_putc(Wi232Register);
        USART_putc(0xfe);
        USART_putc(RegisterValue - 0x80);
    }


    do
    {
        v = USART_getc();                                                   // ACK erwartet
        _delay_ms(100);
        tc++;
    } while( (v == 0) && (tc != timeout) );


    if( v != 0x06 )
    {
      //PKT_Message_P( text, error, timeout, beep, clearscreen)
      //PKT_Message_P( PSTR("Wi.232 NAK"), true, 2000, true, true);         // ERROR: max. 20 Sekunden anzeigen (wird nicht mehr angezeigt da uebernommen von hoeheren Funktionen)
        return 0xFF;                                                        // return: ERROR
    }

    return 0;                                                               // return: OK  (v==0x06)
}



//--------------------------------------------------------------
//--------------------------------------------------------------
uint8_t Wi232_Write_Progress( uint8_t Wi232Register, uint8_t RegisterValue, uint8_t errorcode )
{
    uint8_t nError;

    // nError = 0: ok
    // nError > 0: Fehler
    nError = Wi232_Write( Wi232Register, RegisterValue );

    PKT_Progress_Next();                                        // Progressbar

    if( nError )
    {
        nError = errorcode;
    }

    return nError;
}



//--------------------------------------------------------------
// Registervalue = Wi232_Read( Wi232Register )
//
// send Readcommand to Wi232,
//
// Returns:
//   Registervalue: 0 = timeout 0xFF = Syntaxerror
//--------------------------------------------------------------
uint8_t Wi232_Read( uint8_t Wi232Register )
{
    uint8_t timeout = 10;
    uint8_t tc = 0;
    uint8_t v;


    v = USART_getc();                   // Zeichen loeschen
    USART_putc(0xff);
    USART_putc(0x02);
    USART_putc(0xfe);
    USART_putc(Wi232Register);
    _delay_ms(50);
  //lcd_printpns_at (0, 2, PSTR("read Wi232"),0);


    do
    {
        v = USART_getc();               //ACK erwartet
        _delay_ms(100);
        tc++;
    } while( v==0 && tc!=timeout );

    if( tc == timeout )
        return 0;                       // Timeout

    if( v != 0x06 )
        return 0xFF;                    // Syntaxerror

//  lcd_print_hex(v,0);
//  v = USART_getc(); /*Register*/
//  lcd_print_hex(v,0);
//  v = USART_getc(); /*Value*/
//  lcd_print_hex(v,0);

    return v;
}



//--------------------------------------------------------------
// foundbaud = Wi232_CheckBaud( Baudrate )
//
// RUECKGABE:
//  =0: Error (Wi.232 not found at given Baud)
//  >0: Baudrate
//
// INFO: Baudrate
//   Baud_9600       1
//   Baud_19200      2
//   Baud_38400      3
//   Baud_57600      4
//   Baud_115200     5
//   Baud_4800       6
//   Baud_2400       7
//--------------------------------------------------------------
uint16_t Wi232_CheckBaud( uint8_t Baudrate )                // Suche Wi232 mit entsprechender Baudrate
{
    uint8_t RegisterWi232;

  //lcdx_cls_row( y, mode, yoffs )
    lcdx_cls_row( 2, MNORMAL, 1 );                          // Zeile loeschen
    lcdx_printf_center_P( 2, MNORMAL, 0,1, PSTR("%S %lu Baud"), strGet(STR_SEARCH), Baud_to_uint32(Baudrate) );   // "suche nnn Baud"

    SetBaudUart0( Baudrate );                               // UART auf angegebene Baudrate setzen

    RegisterWi232 = Wi232_Read( regDiscover );

    //---------------------------------------------------
    // Hinweis zu RegisterWi232 nach regDiscover
    //   == 0   : "no Wi.232 found"
    //   == 0xFF: "Wi.232 Syntaxerror"
    //
    //   >0 && < 0xff: Wi.232 Version (Wi.232 gefunden)
    //---------------------------------------------------
    if( RegisterWi232 > 0 && RegisterWi232 < 0xFF )
    {
        return Baudrate;                                    // Wi.232 gefunden
    }

    return 0;                                               // Wi.232 nicht gefunden bei gegebener Baudrate
}



//--------------------------------------------------------------
// InitWI232( InitBaudrate )
//
// set Wi232Register for Mikrokopter
//
// Returns:
//   0 = OK, 1 = Abbruch, 2 = Error, 3 = undef
//--------------------------------------------------------------
uint8_t Wi232_Initalize( void )
{
    uint8_t foundbaud = 0;
    uint8_t nError  = 0;
    uint8_t InitBaudrate = Config.PKT_Baudrate;
    uint8_t RetVal = 3;                 // undefined


    //----------------------------
    // 1. Kopter ausgeschaltet?
    //----------------------------
    // Text1: "Kopter ausschalten!"
    // Text2: NULL
    // Head : "* ACHTUNG *"
    // Titel: "Wi.232 Init"

    if( !PKT_Ask_P( ASK_CANCEL_OK, strGet(STR_SWITCHOFFMK), NULL, strGet(STR_ATTENTION), STR_WI232INIT) )
    {
        return 1;
    }


    //----------------------------
    // 2. Wi.232 suchen
    //----------------------------
    PKT_Title_P( STR_WI232INIT, true, true );                               // Titel - mit ShowLipo und clearscreen

    Change_Output( Uart02Wi );                                              // Verbindung zu Wi232 herstellen
    SwitchToWi232();                                                        // Serielle Kanaele Wi232 mit USB verbinden
    set_WI232CMD();
    _delay_ms(100);
  //_delay_ms(200);

    if( !foundbaud )    foundbaud = Wi232_CheckBaud( Old_Baudrate );        // versuche zuerst mit der 'alten' Baudrate (hohe Trefferwahrscheinlichkeit)
    if( !foundbaud )    foundbaud = Wi232_CheckBaud( Baud_2400    );
    if( !foundbaud )    foundbaud = Wi232_CheckBaud( Baud_9600    );
    if( !foundbaud )    foundbaud = Wi232_CheckBaud( Baud_19200   );
    if( !foundbaud )    foundbaud = Wi232_CheckBaud( Baud_38400   );
    if( !foundbaud )    foundbaud = Wi232_CheckBaud( Baud_57600   );
    if( !foundbaud )    foundbaud = Wi232_CheckBaud( Baud_115200  );

    if( !foundbaud )                                                        // gefunden?
    {
      //PKT_Message_P( text, error, timeout, beep, clearscreen)
        PKT_Message_P( PSTR("Wi.232 not found"), true, 2000, true, true);   // ERROR: max. 20 Sekunden anzeigen
    }


    //----------------------------
    // 3. Wi.232 initialisieren
    //----------------------------
    if( foundbaud > 0 )
    {
        //--------------
        // Screen
        //--------------
        PKT_Title_P( STR_WI232INIT, true, true );                           // Titel - mit ShowLipo und clearscreen
        lcdx_printf_center_P( 2, MNORMAL, 0,1, STR_WI232INIT );

      //PKT_Progress_Init( max, yoffs, width, height);
        PKT_Progress_Init( 18, 0,  0,0);                                    // 18 Progress Steps


        // DEBUG - hier kann ggf. die _gefundene_ Baudrate vom Wi.232 angezeigt werden
        //lcdx_printf_center_P( 6, MNORMAL, 0,-4, PSTR("(found %lu Baud)"), Baud_to_uint32(foundbaud) );   // zeige Baudrate
        // DEBUG - und hier auf welche Baudrate das Wi.232 gesetzt werden soll
        //lcdx_printf_center_P( 7, MNORMAL, 0,-2, PSTR("(set %lu Baud)"), Baud_to_uint32(Config.PKT_Baudrate) );   // zeige Baudrate


        SetBaudUart0( foundbaud );

        // wenn sich ein EEPROM-Wert ändert wird auch das Ram beschrieben damit die Änderung sofort wirksam wird

        //--------------------------------------------------------------------------------------------------------------------
        //                                                                                + Errorcode
        //                                                                                |
        //                                                                                |
        if( !nError ) nError = Wi232_Write_Progress( regNVTXCHANNEL,Config.WiTXRXChannel, 1);   // TX Channel
        if( !nError ) nError = Wi232_Write_Progress( regTXCHANNEL,Config.WiTXRXChannel  , 2);   // TX Channel
        if( !nError ) nError = Wi232_Write_Progress( regNVRXCHANNEL,Config.WiTXRXChannel, 3);   // RX Channel
        if( !nError ) nError = Wi232_Write_Progress( regRXCHANNEL,Config.WiTXRXChannel  , 4);   // RX Channel
        if( !nError ) nError = Wi232_Write_Progress( regNVSLPMODE ,Sleep_Awake          , 5);   // Sleepmode
        if( !nError ) nError = Wi232_Write_Progress( regNVPWRMODE,WbModeP15             , 6);   // Transceiver Mode/Powermode


        //---------
        // 06.06.2014 OG: nicht mehr unterstuetzt via setup.c (ggf. spaeter festen Standardwert einsetzen statt Config.xyz)
        // Standard: TWaitTime16 (=0x10  // 16 ms Delay (default))
        if( !nError ) nError = Wi232_Write_Progress( regNVTXTO,Config.WiTXTO            , 7);   // UART Timeout
        if( !nError ) nError = Wi232_Write_Progress( regTXTO,Config.WiTXTO              , 8);   // UART Timeout

        //---------
        // 06.06.2014 OG: nicht mehr unterstuetzt via setup.c (ggf. spaeter festen Standardwert einsetzen statt Config.xyz)
        // Standard: UartMTU64 (=64  // default=64, valid 1-144)
        if( !nError ) nError = Wi232_Write_Progress( regNVUARTMTU,Config.WiUartMTU      , 9);   // UART Buffer
        if( !nError ) nError = Wi232_Write_Progress( regUARTMTU,Config.WiUartMTU        ,10);   // UART Buffer

        //---------
        // 06.06.2014 OG: nicht mehr unterstuetzt via setup.c (ggf. spaeter festen Standardwert einsetzen statt Config.xyz)
        // Standard: NetMode_Normal (=0x01  // Normalmode (default))
        if( !nError ) nError = Wi232_Write_Progress( regNVNETMODE,Config.WiNetworkMode  ,11);   // Networkmode
        if( !nError ) nError = Wi232_Write_Progress( regNETMODE,Config.WiNetworkMode    ,12);   // Networkmode

        if( !nError ) nError = Wi232_Write_Progress( regNVUSECRC ,CRC_Enable            ,13);   // CRC
        if( !nError ) nError = Wi232_Write_Progress( regNVCSMAMODE,CSMA_En              ,14);   // CSMA
        if( !nError ) nError = Wi232_Write_Progress( regNVNETGRP,Config.WiNetworkGroup  ,15);   // Networkgroup
        if( !nError ) nError = Wi232_Write_Progress( regNETGRP,Config.WiNetworkGroup    ,16);   // Networkgroup
        if( !nError ) nError = Wi232_Write_Progress( regNVDATARATE,InitBaudrate         ,17);   // Baudrate
        _delay_ms(200);
        if( !nError ) nError = Wi232_Write_Progress( regDATARATE,InitBaudrate           ,18);   // Baudrate
        _delay_ms(250);

        //_delay_ms(6000);  // DEBUG


        if( nError )
        {
          //PKT_Message( text, error, timeout, beep, clearscreen)
            PKT_Message( buffered_sprintf_P(PSTR("Wi.232 Error: %d"),nError), true, 2000, true, true);      // ERROR: 2000 = max. 20 Sekunden anzeigen
            RetVal = 2;
        }
        else
        {
          //PKT_Message_P( text, error, timeout, beep, clearscreen)
            PKT_Message_P( PSTR("Wi.232 Init OK!"), false, 1000, true, true);                               // OK: 1000 = max. 10 Sekunden anzeigen
            WriteWiInitFlag();                                                                              // erfolgreiches Init merken
            RetVal = 0;
        }
    }
    else RetVal = 2;


    clr_WI232CMD();
    SetBaudUart0( Config.PKT_Baudrate );                                // die richtige UART Baudrate wiederherstellen !!
    clear_key_all();
    return RetVal;
}



//--------------------------------------------------------------
// Wi232_ConfigPC()
//
// Connect USB direct to Wi.232 in Progmode
//--------------------------------------------------------------
void Wi232_ConfigPC( void )
{
    lcd_cls ();

    lcd_frect( 0, 0, 127, 7, 1);                                            // Titelzeile Invers
    lcdx_printf_at_P( 1, 0, MINVERS, 0,0, strGet(STR_WI232CONFIG) );        // Titeltext "Wi.232 Konfig."

    lcdx_printp_center( 2, strGet(STR_USBCONNECTED), MNORMAL,  0,-2);       // "mit USB verbunden"

    lcdx_printp_center( 4, strGet(STR_SEEMKWIKI)   , MNORMAL,  0,0);        // "siehe MK-Wiki:"
    lcdx_printp_center( 5, PSTR("\"RadioTronix\"") , MNORMAL,  0,2);        // "RadioTronix"
    lcd_rect_round( 0, 28, 127, 24, 1, R2);                                 // Rahmen um Text

    lcd_printp_at(12, 7, strGet(ENDE), MNORMAL);                            // Keyline "Ende"


    Change_Output(USB2Wi);

    set_WI232CMD();  // Port D6 = CMD

    while(!get_key_press(1 << KEY_ESC))
    {
        show_Lipo();
    }

    clr_WI232CMD();  // Port D6 = CMD

    if( Config.U02SV2 == 1 )
        Change_Output(Uart02FC);
    else
        Change_Output(Uart02Wi);


    // 10.06.2014 OG: das Folgende war im orginalen Port_USB2CFG_Wi() nicht
    //                vorhanden - das hatte den Effekt, dass wenn man diese
    //                Funktion aufgerufen und wieder beendet hatte das
    //                PKT-Updatetool kein PKT-Update mehr via PKT_CtrlHook()
    //                durchfuehren konnte. Die USB Umschaltung war wohl nicht sauber.
    //
    uart1_init( UART_BAUD_SELECT(USART_BAUD,F_CPU) );   // USB wieder zuruecksetzen damit PKT wieder normal via USB erreichbar (u.a. fuer PKT-Updatetool)
    SetBaudUart1( Config.PKT_Baudrate );                // USB wieder zuruecksetzen damit PKT wieder normal via USB erreichbar (u.a. fuer PKT-Updatetool)
}


//#endif
