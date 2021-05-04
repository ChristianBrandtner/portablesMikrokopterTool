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
//# HISTORY  connect.c
//#
//# 10.06.2014 OG
//# - del: Port_USB2CFG_Wi() - jetzt in Wi232.c als Wi232_ConfigPC()
//#
//# 13.04.2014 OG
//# - add: Baud_to_uint32()
//#
//# 10.04.2014 OG
//# - del: Port_FC2CFG_WL()
//#
//# 01.04.2014 OG
//# - add: wait_and_restore() redundanten Code einsparen
//# - add: Port_BLE2Wi() (Bluetooth 4 Low Energy Modul an SV2)
//#
//# 28.02.2014 OG
//# - chg: alle Anzeigescreens von 'PKT Connect' auf Connect_Screen() umgestellt
//# - add: Connect_Screen()
//#
//# 12.02.2014 OG
//# - chg: Port_WiFly2Wi() Auskommentiert: "unused variable 'i'"
//#
//# 15.07.2013 Cebra
//# - chg: Port_FC2CFG_WL, Konfiguration Wlan Modul mit PC, Texte geändert
//#
//# 02.07.2013 Cebra
//# - add: Port_FC2CFG_WL, Konfiguration Wlan Modul mit PC
//#
//# 26.06.2013 Cebra
//# - chg: Modulumschaltung für WiFlypatch geändert
//# - add: Port_WiFly2Wi, Wlan-Wi232 Verbindung
//#
//# 24.6.2013 Cebra
//# - fix: Fehler beim UART1 bei der Rückschaltung von USB2FC,USB2WI,BT2FC,BT2WI
//#
//# 19.06.2013 OG
//# - chg: Screen-Layout von Port_BT2Wi(), Port_USB2Wi()
//############################################################################


#include "cpu.h"
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <string.h>

#include "lcd/lcd.h"
#include "timer/timer.h"
#include "eeprom/eeprom.h"
#include "messages.h"
#include "lipo/lipo.h"
#include "main.h"
#include "bluetooth/bluetooth.h"
#include "wifly/PKT_WiFlyHQ.h"
#include "uart/uart1.h"
#include "utils/xutils.h"


//#if defined HWVERSION3_9
//--------------------------------------------------------------
void Change_Output(uint8_t UartMode)    // Schaltet die Rx/Tx Richtungen
{
    // hiermit werden die 74HTC125 (IC5) Gatter geschaltet
    clr_USB2FC();       // PC2 aus  IC5 A/D
    clr_USB2Wi();       // PB0 aus  IC5 B/C
    clr_Uart02FC();     // PC6 aus  IC4 C/D
    clr_Uart02Wi();     // PC5 aus  IC4 A/B


    switch (UartMode)
    {
        case USB2FC:
            UCSR1B &= ~(1<<RXEN1);
            UCSR1B &= ~(1<<TXEN1);
            UCSR1B &= ~(1<<RXCIE1);

            DDRD  &= ~(1<<DDD2);    // Pins auf Eingang setzen
            DDRD  &= ~(1<<DDD3);
            PORTD &= ~(1<<PD2);     // Pullup aus
            PORTD &= ~(1<<PD3);

            set_USB2FC();
            break;

        case Uart02Wi:
            set_Uart02Wi();
            break;

        case Uart02FC:
            set_Uart02FC();
            break;

        case USB2Wi:
            UCSR1B &= ~(1<<RXEN1);
            UCSR1B &= ~(1<<TXEN1);
            UCSR1B &= ~(1<<RXCIE1);

            DDRD  &= ~(1<<DDD2);    // Pins auf Eingang setzen
            DDRD  &= ~(1<<DDD3);
            PORTD &= ~(1<<PD2);     // Pullup aus
            PORTD &= ~(1<<PD3);

            set_USB2Wi();
            break;
    }
}



//--------------------------------------------------------------
// gibt eine uebergebene Baud-Rate als uint32_t zurueck
//--------------------------------------------------------------
uint32_t Baud_to_uint32( uint8_t baud )
{
    switch( baud )
    {
        case Baud_2400:   return( 2400   );
        case Baud_4800:   return( 4800   );
        case Baud_9600:   return( 9600   );
        case Baud_19200:  return( 19200  );
        case Baud_38400:  return( 38400  );
        case Baud_57600:  return( 57600  );
        case Baud_115200: return( 115200 );
    }
    return( 0 );
}



//--------------------------------------------------------------
// Connect_Screen(
//
// Parameter:
//   title      : PROGMEM
//   message    : RAM oder PROGMEM je nach msg_progmem
//   msg_progmem: true/false
//                true = message im PROGMEM
//                false = message im RAM
//--------------------------------------------------------------
void Connect_Screen( const char *title, const char *message, uint8_t msg_progmem  )
{
    lcd_cls ();

    lcd_frect( 0, 0, 127, 7, 1);                                            // Titel: Invers
    lcd_printp_at( 1, 0, title, MINVERS);                                   // Titel
    show_Lipo();

    lcdx_printp_center( 2, title             , MNORMAL,  0,-3);             //
    lcdx_printp_center( 3, strGet(STR_ACTIVE), MNORMAL,  0,1);              // "AKTIV!"

    if( (!msg_progmem && strlen(message)>0) || (msg_progmem && strlen_P(message)>0) )
    {
        if( msg_progmem )
            lcdx_printp_center( 5, message, MNORMAL,  0,1);
        else
            lcdx_print_center( 5, (unsigned char *) message, MNORMAL,  0,1);

        lcd_rect_round( 0, 5*7+2, 127, 8+6, 1, R2);                         // Rahmen
    }

    lcd_printp_at(12, 7, strGet(ENDE)               , MNORMAL);             // Keyline
}



//--------------------------------------------------------------
// INTERN
//
// _port_wait_and_restore()
//
// Wartet bis der Benutzer beendet und restauriert Einstellungen
//
// gemeinsamer Code um Speicherplatz zu sparen
//--------------------------------------------------------------
void wait_and_restore( void )
{
    while( !get_key_press(1 << KEY_ESC) )
    {
        show_Lipo();
    }

    uart1_init( UART_BAUD_SELECT(57600,F_CPU) );

    SetBaudUart1( Config.PKT_Baudrate );

    if( Config.U02SV2 == 1 )
        Change_Output( Uart02FC );
    else
        Change_Output( Uart02Wi );

    set_Modul_On( USB );
}



//--------------------------------------------------------------
// Function: BT2FC()
// Purpose:  Connect BT direct to FC-Kabel (SV2 as MKUSB)
// Returns:
//--------------------------------------------------------------
void Port_BT2FC(void)
{
    /*
    // mit Anzeige von 'Baud'
    // verbraucht einige Bytes mehr...
    char *message;
    uint32_t baud;

    switch( Config.PKT_Baudrate )
    {
        case Baud_2400:   baud = 2400;      break;
        case Baud_4800:   baud = 4800;      break;
        case Baud_9600:   baud = 9600;      break;
        case Baud_19200:  baud = 19200;     break;
        case Baud_38400:  baud = 38400;     break;
        case Baud_57600:  baud = 57600;     break;
        case Baud_115200: baud = 115200;    break;
    }
    message = buffered_sprintf_P( PSTR("%lu Baud"), baud);
    Connect_Screen( PSTR("BT -> SV2 Kabel"), message, false );
    */

    // ohne Anzeige von 'Baud'
    Connect_Screen( PSTR("BT -> SV2 Kabel"), "", false );

    // set_BTOn();
    set_Modul_On(Bluetooth);
    _delay_ms(1000);
    //_delay_ms(2000);

    if( !Config.BTIsSlave==true )
        bt_set_mode(BLUETOOTH_SLAVE);

    Change_Output(USB2FC);
    wait_and_restore();
}



//--------------------------------------------------------------
// Function: BT2Wi()
// Purpose:  Connect BT direct to Wi.232
// Returns:
//--------------------------------------------------------------
void Port_BT2Wi( void )
{
    char *message = "RE-ID: 0000";

    strcpy( &message[7], Config.RE_ID);
    Connect_Screen( PSTR("BT Extender"), message, false );

    set_Modul_On( Bluetooth );      // set_BTOn();
    _delay_ms(1500);
    if( !Config.BTIsSlave==true ) bt_set_mode(BLUETOOTH_SLAVE);

    Change_Output(USB2Wi);
    wait_and_restore();
}




//--------------------------------------------------------------
// Function: WiFly2Wi()
// Purpose:  Connect WiFly direct to Wi.232
// Hinweis: SV2 RxTx Patch erforderlich!
// Returns:
//--------------------------------------------------------------
void Port_WiFly2Wi( void )
{
    Connect_Screen( PSTR("WLAN Extender"), PSTR("169.254.1.1:2000"), true );

    set_Modul_On( Wlan );
    Change_Output( USB2Wi );
    wait_and_restore();
}



//--------------------------------------------------------------
// Function: BLE2Wi()
// Purpose:  Connect BLE direct to Wi.232
// Hinweis: SV2 RxTx Patch erforderlich!
// Returns:
//--------------------------------------------------------------
void Port_BLE2Wi( void )
{
    Connect_Screen( PSTR("BLE Extender"), "", false );

    set_Modul_On(Wlan);     // verwendet wie Wlan/WiFly SV2
    Change_Output(USB2Wi);
    wait_and_restore();
}



//--------------------------------------------------------------
// Function: FC2CFG_BT()
// Purpose:  Connect FC (Tx1 Pin3, Rx1 Pin4) direct to BT
// Returns:
//--------------------------------------------------------------
void Port_FC2CFG_BT(void)
{
    lcd_cls ();
    lcd_printp_at (0, 0, PSTR("BTM-222 Konfigurieren"), 2);
    lcd_printp_at (0, 1, PSTR("FC > MK-USB > BTM-222"), 2);
    lcd_printp_at (0, 3, PSTR("MK-USB an PC anschl. "), 0);
    lcd_printp_at (0, 4, PSTR("Zwischen MK-USB und  "), 0);
    lcd_printp_at (0, 5, PSTR("PKT ein gekreuztes   "), 0);
    lcd_printp_at (0, 6, PSTR("Kabel anschliessen.  "), 0);
    lcd_printp_at(12, 7, strGet(ESC), 0);

//    set_BTOn();
    set_Modul_On(Bluetooth);
    Change_Output(USB2FC);

    while(!get_key_press (1 << KEY_ESC));

    if (Config.U02SV2 == 1)
        Change_Output(Uart02FC);
    else
        Change_Output(Uart02Wi);
    set_Modul_On(USB);
//    set_BTOff();
    return;
}



//--------------------------------------------------------------
// Function: USB2FC()
// Purpose:  Connect USB direct to FC-Kabel (SV2 as MKUSB)
// Returns:
//--------------------------------------------------------------
void Port_USB2FC(void)
{
    Connect_Screen( PSTR("USB -> SV2 Kabel"), PSTR("MK-USB"), true );

    Change_Output(USB2FC);
    wait_and_restore();

/*
//------
// 01.04.2014 OG: ersetzt durch: wait_and_restore()
//            Unterschied: zum Schluss wird bei wait_and_restore()
//            noch ein set_Modul_On(USB) gemacht - ist das relevant?
//------
    do
    {
        show_Lipo();
    } while(!get_key_press (1 << KEY_ESC));

    get_key_press(KEY_ALL);
    uart1_init (UART_BAUD_SELECT(57600,F_CPU));
    SetBaudUart1(Config.PKT_Baudrate);

    if (Config.U02SV2 == 1)
        Change_Output(Uart02FC);
    else
        Change_Output(Uart02Wi);

    return;
*/
}



//--------------------------------------------------------------
// Function: USB2Wi()
// Purpose:  Connect USB direct to Wi.232
// Returns:
//--------------------------------------------------------------
void Port_USB2Wi(void)
{
    Connect_Screen( PSTR("USB -> Wi.232"), "", false );

    Change_Output(USB2Wi);
    wait_and_restore();

/*
//------
// 01.04.2014 OG: ersetzt durch: wait_and_restore()
//            Unterschied: zum Schluss wird bei wait_and_restore()
//            noch ein set_Modul_On(USB) gemacht - ist das relevant?
//------
    do
    {
        show_Lipo();
    }
    while( !get_key_press(1 << KEY_ESC) );

    if( Config.U02SV2 == 1 )
            Change_Output(Uart02FC);
    else
            Change_Output(Uart02Wi);

    get_key_press(KEY_ALL);
    return;
*/
}


//#endif
