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
//# HISTORY  mkdebugdata.c
//#
//# 04.06.2014 OG
//# komplett neues Layout
//# - chg: MK_DebugData(), GetAnalogNames() umgestellt auf PKT_Message_Datenverlust()
//# - chg: GetAnalogNames() Timeout reduziert
//# - add: #include "../lipo/lipo.h"
//# - add: #include "../pkt/pkt.h"
//# - add: #include <stdbool.h>
//#
//# 29.01.2014 OG
//# - chg: ehemals display_debug() jetzt MK_DebugData()
//# - chg: ehemals 'debug.c' jetzt 'mk/mkdebugdata.c'
//#
//# 03.04.2013 OG
//# - chg: define 'analognames' zu define 'USE_MKDEBUGDATA'
//# - add: Benutzer kann GetAnalogNames() via KEY_ESC abbrechen
//# - chg: Screen-Layout GetAnalogNames()
//# - chg: SwitchToNC/FC Reihenfolge
//#
//# 27.03.2013 OG
//# - chg: auf malloc umgestellt
//# - fix: diverse Anzeigefehler
//# - chg: teilweise redunten Code entfernt
//############################################################################


#include "../cpu.h"
#include <avr/io.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdbool.h>
#include <string.h>
#include "../main.h"

#ifdef USE_MKDEBUGDATA

#include "../lcd/lcd.h"
#include "../pkt/pkt.h"
#include "../lipo/lipo.h"
#include "../uart/usart.h"
#include "mkdebugdata.h"
#include "../timer/timer.h"
#include "../messages.h"
#include "../mk-data-structs.h"


//--------------------------------------------------------------


#define PAGELINES   5       // Anzahl der Zeilen pro Anzeigescreens
#define XOFFS       4
#define YOFFS       12

#define TIMEOUT     200     // 2 sec
#define ANALOGTIME  20      // 200 ms

#define MKDD_ANALOGNAME_SIZE   2*32*17  // MALLOC: 32 names, 16 characters + 1 0x00

// WARNING: this work for NC & FC only
//          if current_hardware == MK3MAG or MKGPS the access is outside of the array...
//uint8_t AnalogNames[2][32][16 + 1];   // 32 names, 16 characters + 1 0x00
uint8_t *AnalogNames;   // MALLOC: MKDD_ANALOGNAME_SIZE - 32 names, 16 characters + 1 0x00

uint8_t AnalogNamesRead[2] = {0,0};

static const char strFC[]     PROGMEM = "FC";
static const char strNC[]     PROGMEM = "NC";


//--------------------------------------------------------------
// Speicher wieder freigeben
//--------------------------------------------------------------
void MKDD_MemFree( void )
{
    free( AnalogNames );
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void showTitle( void )
{
    lcdx_printf_at_P( 0, 0, MINVERS, 0,0, PSTR(" %2S %16S"), (current_hardware==FC ? strFC : strNC), strGet(STR_DATA) );    // Titelzeile
    show_Lipo();
}



//--------------------------------------------------------------
// lOk = GetAnalogNames()
//
// Return:
//  true  = OK
//  false = User-Abort (KEY_ESC)
//--------------------------------------------------------------
uint8_t GetAnalogNames( void )
{
    uint8_t i    = AnalogNamesRead[current_hardware - 1];
    uint8_t t    = 0;
    uint8_t lOk  = TRUE;
    int8_t yoffs = -2;                                              // yoffs der Progress-Bar

    lcd_cls ();
    showTitle();                                                    // Titel
    lcdx_printf_center_P(  3, MNORMAL, 0,0, PSTR("%S %S %S"), strGet(STR_READING), (current_hardware==FC ? strFC : strNC), strGet(STR_LABELS) );

    lcd_rect_round( 13, 40+yoffs, 102, 6, 1, R1);                   // Rahmen fuer Progress
    lcd_printp_at(12, 7, strGet(ENDE), MNORMAL);                    // Keyline


    mode = 'A'; // read Names
    _delay_ms(200);
    rxd_buffer_locked = FALSE;

    timer = ANALOGTIME;

    while( (i < 32) && lOk )
    {
        SendOutData ('a', ADDRESS_ANY, 1, &i, 1);

        while( !rxd_buffer_locked && timer && lOk)
            lOk = !get_key_press (1 << KEY_ESC);

        if( timer )
        {
            Decode64 ();
            if( i == *pRxData )
            {
                lcd_frect( 16, 42+yoffs, (i+1)*3, 2,  1);           // Progress

                memcpy( (uint8_t *) (AnalogNames+((current_hardware-1)*32*17)+(i*17)), (uint8_t *) pRxData + 1, 16);
                *(AnalogNames+((current_hardware-1)*32*17)+(i*17)+16) = 0;
                i++;
                t = 0;
            }
            else
            {
                _delay_ms(100);
            }

            timer = ANALOGTIME;
            rxd_buffer_locked = FALSE;
        }
        else if( lOk )
        {   // timeout occured
            t++;
            timer = ANALOGTIME;

            if( t >= 15 ) // timeout?
            {
              //PKT_Message_Datenverlust( timeout, beep)
                PKT_Message_Datenverlust( 500, true);       // 500 = 5 Sekunden
                break;
            }
        }
        else
        {
            lOk = !get_key_press( 1 << KEY_ESC );
        }
    }

    if(lOk) AnalogNamesRead[current_hardware - 1] = i;

    //_delay_ms(4000);  // DEBUG

    return lOk;
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void showLabels( uint8_t page)
{
    uint8_t i = 0;

    for( i = 0; i < PAGELINES; i++ )
    {
        if( (i + (page * PAGELINES)) >= 32 )
        {
          //lcdx_cls_rowwidth( y, width, mode, xoffs, yoffs )
            lcdx_cls_rowwidth( i, 20, MNORMAL,  XOFFS,YOFFS );
        }
        else
        {
            lcdx_print_at( 0, i, AnalogNames+((current_hardware-1)*32*17)+(i + page * PAGELINES)*17, MNORMAL,  XOFFS,YOFFS);
        }
    }
}


//--------------------------------------------------------------
//
//--------------------------------------------------------------
void MK_DebugData(void)
{
    uint8_t tmp_dat;
    uint8_t i       = 0;
    uint8_t page    = 0;
    uint8_t pagemax = (32/PAGELINES);
    uint8_t redraw  = 2;

    DebugData_t *DebugData;


    // alloc ram
    AnalogNames = malloc( MKDD_ANALOGNAME_SIZE );

    if( !AnalogNames )
    {
        Show_PKTError_NoRAM();
        return;
    }

    memset( AnalogNames, 0, MKDD_ANALOGNAME_SIZE );     // init: AnalogNames
    AnalogNamesRead[0] = 0;
    AnalogNamesRead[1] = 0;

    SwitchToFC();

    timer = TIMEOUT;

    if( AnalogNamesRead[current_hardware - 1] < 32 )
    {
        if( !GetAnalogNames() )
        {
            MKDD_MemFree();
            return;
        }
    }

    if( !timer )
    {
        MKDD_MemFree();
        return;
    }

    mode              = 'D';    // Debug Data
    rxd_buffer_locked = FALSE;
    timer             = TIMEOUT;
    timer1            = 0;

    tmp_dat = 10;
    SendOutData( 'd', ADDRESS_ANY, 1, &tmp_dat, 1);
    abo_timer = ABO_TIMEOUT;


    do
    {
        //------------------------------------------
        //------------------------------------------
        if( redraw )
        {
            if( redraw==2 )	 lcd_cls();

            showTitle();                                                        // Titelzeile
            lcd_rect_round( 0, (1*8)+1, 127, (6*8)-4, 1, R2);                   // Rahmen fuer 5 Zeilen Display

            showLabels( page );

            lcd_printp_at( 0, 7, strGet(KEYLINE3), 0);

            lcd_write_number_u_at (5, 7, page + 1);
            lcd_printp_at( (current_hardware==FC ?  3 : 19), 7, strFC, 0);
            lcd_printp_at( (current_hardware==FC ? 19 :  3), 7, strNC, 0);

            redraw = false;
        }


        if( rxd_buffer_locked )
        {
            Decode64();
            DebugData = (DebugData_t *) pRxData;

            if( !timer1 )
            {
                for( i = 0; i < PAGELINES && (i + (page * PAGELINES)) < 32; i++)
                {
                    writex_ndigit_number_s( 21-6, i, DebugData->Analog[i + page * PAGELINES], 5, 0, MNORMAL,  XOFFS,YOFFS);
                }
                timer1 = 25;                // Anzeigeverzoegerung damit es nicht flackert
            }

            rxd_buffer_locked = FALSE;

            timer = TIMEOUT;
        }


        if( !abo_timer )
        {   // renew abo every ... sec
            tmp_dat = 10;
            SendOutData( 'd', ADDRESS_ANY, 1, &tmp_dat, 1);
            abo_timer = ABO_TIMEOUT;
        }


        //------------------------------------------
        // PKT LiPo anzeigen
        //------------------------------------------
        show_Lipo();


        //------------------------------------------
        // Pruefe PKT Update oder andere PKT-Aktion
        //------------------------------------------
        if( PKT_CtrlHook() )                                                                // Update vom Updatetool angefordert?
        {
            redraw = 2;
            timer  = TIMEOUT;
        }


        //------------------------------------------
        // Tasten
        //------------------------------------------
        if( get_key_press(1 << KEY_MINUS) )
        {
            page--;
            if( page > pagemax ) page = pagemax;
            redraw = true;
        }

        if( get_key_press(1 << KEY_PLUS) )
        {
            page++;
            if( page > pagemax ) page = 0;
            redraw = true;
        }

        if( (hardware == NC) && get_key_press (1 << KEY_ENTER) )
        {
            if( current_hardware == NC )    SwitchToFC();
            else                            SwitchToNC();

            if( AnalogNamesRead[current_hardware - 1] < 32 )
            {
                if( !GetAnalogNames() )
                {
                    MKDD_MemFree();
                    return;
                }
            }

            mode = 'D'; // Debug Data
            rxd_buffer_locked = FALSE;
            timer = TIMEOUT;

            tmp_dat = 10;
            SendOutData ('d', ADDRESS_ANY, 1, &tmp_dat, 1);

            //page = 0;
            redraw = true;
        }
    }
    while (!get_key_press(1 << KEY_ESC) && timer);      // Taste: Ende oder Timeout


    clear_key_all();

    tmp_dat = 0;
    SendOutData ('d', ADDRESS_ANY, 1, &tmp_dat, 1);     // Request Debugdata abschalten

    mode = 0;
    rxd_buffer_locked = FALSE;

    if( !timer )
    {   // timeout occured
      //PKT_Message_Datenverlust( timeout, beep)
        PKT_Message_Datenverlust( 500, true);           // 500 = 5 Sekunden
    }

    //SwitchToNC();

    // free ram
    MKDD_MemFree();
}

#endif   // end: #ifdef USE_MKDEBUGDATA
