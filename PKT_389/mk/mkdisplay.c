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
//# HISTORY  mkdisplay.c
//#
//#
//# 05.04.2015 Cebra
//# - chg: SendOutData( 'h', ADDRESS_ANY, 2, &cmd, 1, 0x00 ,1) ergänzt um 2. Parameter wegen Fehlfunktion mit NC 2.09h
//#
//# 04.06.2014 OG
//# - chg: MK_Display() umgestellt auf PKT_Message_Datenverlust()
//#
//# 31.05.2014 OG
//# - chg: MK_Display() - umgestellt auf PKT_KeylineUpDown()
//#
//# 28.04.2014 OG
//# - chg: MK_Display() - leichte Optimierung der Display-Ausgabe um ein paar
//#        Bytes Code zu sparen
//#
//# 09.04.2014 OG  -  vervollstaendigte Steuerung des MK-Display's
//# - chg: die Tasten fuer 'Ende' und NC/FC-Umschaltung muessen jetzt lange
//#        gedruekckt werden (get_key_long) - keine Beschriftung mehr dafuer
//#        vorhanden da die Beschriftung jetzt hoch/runter zeigt!
//#        (aber 'Ende' ist da wo es immer im PKT ist)
//# - add: Unterstuetzung der hoch/runter Tasten
//# - chg: diverse Aenderungen bei der Kommunkation und Einbindung aktueller
//#        Funktionen von mkbase.c
//#
//# 29.01.2014 OG
//# - chg: ehemals display_data() jetzt MK_display()
//# - chg: ehemals 'display.c' jetzt 'mk/mkdisplay.c'
//#
//# 25.01.2014 OG
//# - optisches Facelift
//# - PKT_CtrlHook() eingeklinkt
//# - Versionshistorie hinzugefuegt
//############################################################################


#include "../cpu.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdbool.h>

#include "../main.h"
#include "../lcd/lcd.h"
#include "../uart/usart.h"
#include "../timer/timer.h"
#include "../messages.h"
#include "../lipo/lipo.h"
#include "../pkt/pkt.h"
#include "../mk-data-structs.h"
#include "mkbase.h"
#include "mkdisplay.h"

#define TIMEOUT 500 // 5 sec


uint8_t ACC_Display=false;


//--------------------------------------------------------------
//--------------------------------------------------------------
void MK_Display(void)
{
    uint8_t cmd;
    uint8_t redraw = true;
    uint8_t exit   = false;

    mode              = 'H';
    rxd_buffer_locked = FALSE;
    timer             = TIMEOUT;
//    cmd               = 0xfc;   // Home = first page
    cmd               = 0xff;   // aktuelle Seite

    redraw = true;
    MK_SwitchToFC();  // add 31.3.2014 Cebra, definierter Zustand

    if (ACC_Display==true)
      {
       cmd = 0x06;
       SendOutData( 'l', ADDRESS_ANY, 1, &cmd,1);
       cmd = 0xff;
       if( rxd_buffer_locked )
       {
           Decode64 ();
           //------------------------------------------
           // RX-Buffer freigeben
           //------------------------------------------
           rxd_buffer_locked = FALSE;
           timer = TIMEOUT;

       } // end: if( rxd_buffer_locked )
//       _delay_ms(250);
      }



    do
    {
        //------------------------------------------
        // Screen neu zeichnen
        //------------------------------------------
        if( redraw )
        {
            ShowTitle_P( ( current_hardware == NC ? PSTR("NC") : PSTR("FC") ), true );
            lcd_printp_at( 4, 0, PSTR("Display"), MINVERS);

            lcd_rect_round( 0, 2*7-3, 127, 5*7+3+3, 1, R2);                         // Rahmen
            lcdx_printp_at( 2, 7, PSTR("\x18    \x19"), MNORMAL,  0,0);             // Keyline: Links / Rechts
            PKT_KeylineUpDown( 18, 13,  0,0);                                       // Keyline: Down / Up

            redraw = false;
        }


        show_Lipo();


        SendOutData( 'h', ADDRESS_ANY, 2, &cmd, 1, 0x00 ,1); // 05.04.2015 Cebra, 2.er Parameter wg NC 2.09i
        cmd = 0xff;

        _delay_ms(250);

        if( rxd_buffer_locked )
        {
            Decode64 ();

            //------------------------------------------
            // Ausgabe auf PKT-Anzeige
            // 4 Zeilen a 20 Zeichen
            // (Anzeige von unten nach oben)
            //------------------------------------------
            rxd_buffer[83] = 0;
            lcdx_print_at( 0,5, (uint8_t *) &rxd_buffer[3+60],  MNORMAL,  5,2);

            rxd_buffer[63] = 0;
            lcdx_print_at( 0,4, (uint8_t *) &rxd_buffer[3+40],  MNORMAL,  5,1);

            rxd_buffer[43] = 0;
            lcdx_print_at( 0,3, (uint8_t *) &rxd_buffer[3+20],  MNORMAL,  5,0);

            rxd_buffer[23] = 0;
            lcdx_print_at( 0,2, (uint8_t *) &rxd_buffer[3+ 0],  MNORMAL,  5,-1);


            //------------------------------------------
            // RX-Buffer freigeben
            //------------------------------------------
            rxd_buffer_locked = FALSE;
            timer = TIMEOUT;

        } // end: if( rxd_buffer_locked )


        //------------------------------------------
        // Pruefe PKT Update oder andere PKT-Aktion
        //------------------------------------------
        if( PKT_CtrlHook() )                                                                // Update vom Updatetool angefordert?
        {
            redraw = true;
        }


        if( get_key_press(1 << KEY_MINUS) || get_key_long_rpt_sp((1 << KEY_MINUS),2) )
        {
            cmd = 0xfe;         // key: rechts (next page)
        }

        if( get_key_press(1 << KEY_PLUS) || get_key_long_rpt_sp((1 << KEY_PLUS),2) )
        {
            cmd = 0xfd;         // key: links (previous page)
        }

        if( get_key_short(1 << KEY_ESC) )
        {
            cmd = 0xfb;         // key: runter
        }

        if( get_key_short(1 << KEY_ENTER) )
        {
            cmd = 0xf7;         // key: hoch
        }


        if( (hardware == NC) && get_key_long(1 << KEY_ENTER) )
        {
            set_beep( 25, 0xffff, BeepNormal );                             // kurzer Bestaetigungs-Beep

            if( current_hardware == NC )
            {

                MK_SwitchToFC();
                redraw = true;
            }
            else
            {

                MK_SwitchToNC();

                MK_SwitchToNC();

                redraw = true;
            }
//            cmd = 0xff;

            timer2 = 50;
            while( timer2 > 0 );
        }


        if( get_key_long(1 << KEY_ESC) )
        {
            set_beep( 25, 0xffff, BeepNormal );                             // kurzer Bestaetigungs-Beep
            exit = true;
        }
    }
    while( (!exit) && timer );


    mode = 0;
    rxd_buffer_locked = FALSE;


    //------------------------------------------
    // Timeout?
    //------------------------------------------
    if( !timer )
    {   // timeout occured
      //PKT_Message_Datenverlust( timeout, beep)
        PKT_Message_Datenverlust( 500, true);       // 500 = 5 Sekunden
    }

    clear_key_all();
}
