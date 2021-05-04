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
//# HISTORY  mkgpsinfo.c
//#
//# 04.06.2014 OG
//# - chg: MK_Gps_Info() umgestellt auf PKT_Message_Datenverlust()
//#
//# 14.05.2014 OG
//# - chg: Code-Redesign; neues Screen-Layout; Code-Reduzierung;
//#        Anpassungen an neue PKT-Funktionen
//# - chg: umbenannt von 'gps.c' zu 'mkgpsinfo.c'
//# - chg: gps() ist jetzt MK_Gps_Info()
//# - add: Source-Historie ergaenzt
//############################################################################


#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <util/delay.h>

#include "../main.h"
#include "../lcd/lcd.h"
#include "../timer/timer.h"
#include "../uart/usart.h"
#include "../lipo/lipo.h"
#include "../pkt/pkt.h"
#include "../mk/mkbase.h"
#include "../messages.h"


//##############################################################
//##############################################################


//#define DEBUG_MKGPS           // erweiterte Anzeigen zum debuggen

#define GPSTIMEOUT          300 // 3 Sekunden
#define UBX_BUFFER_SIZE     100 // 100 Byte Buffer fuer GPS-Daten


uint8_t ck_a = 0;
uint8_t ck_b = 0;

union long_union
{
    uint32_t dword;
    uint8_t byte[4];
} longUnion;


union int_union
{
    uint16_t dword;
    uint8_t byte[2];
} intUnion;


//##############################################################
//##############################################################


//--------------------------------------------------------------
//--------------------------------------------------------------
uint32_t join_4_bytes(uint8_t Buffer[])
{
    longUnion.byte[0] = *Buffer;
    longUnion.byte[1] = *(Buffer+1);
    longUnion.byte[2] = *(Buffer+2);
    longUnion.byte[3] = *(Buffer+3);
    return (longUnion.dword);
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void checksum(uint8_t data)
{
    ck_a += data;
    ck_b += ck_a;
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void MK_Gps_Info( void )
{
    uint8_t  UBX_buffer[UBX_BUFFER_SIZE];
    uint8_t  UBX_payload_counter = 0;
    uint8_t  UBX_class = 0;
    uint8_t  UBX_id    = 0;
    uint8_t  UBX_ck_a  = 0;
    uint8_t  data      = 0;
    uint8_t  length    = 0;
    uint8_t  state     = 0;

    uint8_t  redraw    = true;
    uint8_t  refresh   = false;
    uint8_t  yoffs     = 1;         // Anzeige-Verschiebung: der obere Bereich
    uint8_t  yoffs2    = 4;         // Anzeige-Verschiebung: GPS-Koordinaten
    int16_t  v_16;                  // tmp-Variable
    int32_t  v_32;                  // tmp-Variable

    #ifdef DEBUG_MKGPS
    uint8_t  maxlen = 0;
    #endif

    MK_SwitchToNC();                // Anmerkung OG: warum auch immer es besser ist erst auf die NC
    MK_SwitchToGPS();               // umzuschalten... es laeuft so scheinbar zuverlaessiger
    MK_SwitchToGPS();

    timer_mk_timeout = GPSTIMEOUT;

    do
    {
        //------------------------
        // REDRAW
        //------------------------
        if( redraw )
        {
            ShowTitle_P( PSTR("MK GPS Info"), true);

            lcdx_printp_at( 0,1, PSTR("Fix:"),  MNORMAL, 0,yoffs );
            lcdx_printp_at( 0,2, PSTR("Sat:"),  MNORMAL, 0,yoffs );
            lcdx_printp_at( 0,3, PSTR("Alt:"),  MNORMAL, 0,yoffs );

            lcdx_printp_at( 9,1, PSTR("PDOP:"), MNORMAL, 3,yoffs );
            lcdx_printp_at( 9,2, PSTR("Accu:"), MNORMAL, 3,yoffs );
            lcdx_printp_at( 9,3, PSTR("Sped:"), MNORMAL, 3,yoffs );

            lcd_frect( 0, (4*7)+4+yoffs2, 127, 7, 1);                                       // GPS: Rect: Invers
            lcdx_printp_at(1, 3, strGet(START_LASTPOS1), MINVERS, 0,8+yoffs2);              // GPS: "Breitengr  Längengr"
            lcd_rect( 0, (3*8)+7+yoffs2, 127, (2*8)+4, 1);                                  // GPS: Rahmen

            lcdx_printp_at(12,7, strGet(ENDE), MNORMAL, 0,1);                               // KEYLINE

            redraw = false;
        }


        //------------------------
        // REFRESH
        //------------------------
        if( refresh )
        {
            if((UBX_class == 1) && (UBX_id == 6))                                           // GPS: SVINFO
            {
                //--------------
                // GPS Status
                //--------------
                switch( UBX_buffer[10] )
                {
                    case 4:
                    case 3:     lcdx_printp_at( 5, 1, PSTR("3D"), MNORMAL, 1,yoffs);    break;
                    case 2:     lcdx_printp_at( 5, 1, PSTR("2D"), MNORMAL, 1,yoffs);    break;
                    default:    lcdx_printp_at( 5, 1, PSTR("no"), MNORMAL, 1,yoffs);
                }

                // GPS ok => ein Haken wird angezeigt
                if( (UBX_buffer[11] & 1) == 1 )     lcdx_putc( 7, 1, SYMBOL_CHECK, MNORMAL, 3,yoffs );
                else                                lcdx_putc( 7, 1, ' ',          MNORMAL, 3,yoffs );

                //---
                // Anzeige von "D" - evtl. DGPS (Differential GPS)?
                // aktuell nicht mehr unterstuetzt da kein Platz auf dem Screen
                //---
                //if( (UBX_buffer[11] & 3) == 3 )   lcd_printp_at (10,0, PSTR("D"), 0);
                //else                              lcd_printp_at (10,0, PSTR(" "), 0);

                //--------------
                // Sat
                //--------------
                lcdx_printf_at_P( 5, 2, MNORMAL, 0,yoffs, PSTR("%2u"), UBX_buffer[47] );

                //--------------
                // PDOP
                //--------------
                v_16 = UBX_buffer[44]+UBX_buffer[45]*255;
                lcdx_printf_at_P( 15, 1, MNORMAL, 0,yoffs, PSTR("%2.2d"), v_16 );

                //--------------
                // Accuracy
                //--------------
                v_32 = (int32_t)join_4_bytes(&UBX_buffer[24]);
                lcdx_printf_at_P( 15, 2, MNORMAL, 0,yoffs, PSTR("%2.2ldm"), v_32 );
            }

            if((UBX_class == 1) && (UBX_id == 18))                                          // GPS: VELNED
            {
                //--------------
                // Speed
                //--------------
                v_16 = (int16_t)((join_4_bytes(&UBX_buffer[20])*60*60)/100000);
                lcdx_printf_at_P( 15, 3, MNORMAL, 0,yoffs, PSTR("%3dkmH"), v_16 );

                //uint16_t speed = (uint16_t)((join_4_bytes(&UBX_buffer[20])*60*60)/100000);
                //write_ndigit_number_u (11, 4, speed, 3, 0,0);
                //lcd_printp_at (15,4, PSTR("km/h"), 0);
            }

            if((UBX_class == 1) && (UBX_id == 2))                                           // GPS: POSLLH
            {
                //--------------
                // Altitude
                //--------------
                v_16 = (int16_t)(join_4_bytes(&UBX_buffer[16])/1000);
                //v_16 = v_16 + 3000;
                lcdx_printf_at_P( 4, 3, MNORMAL, 1,yoffs, PSTR("%4d"), v_16 );

                //uint16_t height = (uint16_t)(join_4_bytes(&UBX_buffer[16])/1000);
                //write_ndigit_number_u (11, 7, height, 4, 0,0);
                //lcd_printp_at(16,7, PSTR("m"), 0);


                //--------------
                // Breitengrad - Lat (51.)
                //--------------
                v_32 = join_4_bytes(&UBX_buffer[8]);
                writex_gpspos(  1, 4, v_32 , MNORMAL,  0,10+yoffs2);                        // Anzeige: Breitengrad

                //write_ndigit_number_u ( 0, 7, (uint16_t)(lat/10000000), 2, 0,0);
                //lcd_printp_at         ( 2, 7, PSTR("."), 0);
                //write_ndigit_number_u ( 3, 7, (uint16_t)((lat/1000) % 10000), 4, 1,0);
                //write_ndigit_number_u ( 7, 7, (uint16_t)((lat/10) % 100), 2, 1,0);


                //--------------
                // Laengengrad - Long (7.)
                //--------------
                v_32 = join_4_bytes(&UBX_buffer[4]);
                writex_gpspos( 12, 4, v_32, MNORMAL, -1,10+yoffs2);                     // Anzeige: Laengengrad

                //write_ndigit_number_u (10, 7, (uint16_t)(lon/10000000), 2, 0,0);
                //lcd_printp_at         (12, 7, PSTR("."), 0);
                //write_ndigit_number_u (13, 7, (uint16_t)((lon/1000) % 10000), 4, 1,0);
                //write_ndigit_number_u (17, 7, (uint16_t)((lon/10) % 100), 2, 1,0);
            }

            //------------------------
            // PKT-LiPo Anzeige
            //------------------------
            show_Lipo();

            refresh = false;
        } // end: if( refresh )


        //--------------------------
        // PROCESS DATA
        //--------------------------
        if( uart_getc_nb( &data ) )
        {
            switch( state )
            {
                case 0: if( data == 0xB5 )                      // GPS: init 1
                        {
                            UBX_payload_counter = 0;
                            UBX_id = 0;
                            UBX_class = 0;
                            ck_a = 0;
                            ck_b = 0;
                            state++;
                        }
                        break;


                case 1: if( data == 0x62 )                      // GPS: init 2
                            state++;
                        else
                            state = 0;
                        break;


                case 2: if( data != 1 )                         // GPS: class
                            state = 0;
                        else
                        {
                            checksum(data);
                            UBX_class = data;
                            state++;
                        }
                        break;


                case 3: if(   (data != 48)                      // GPS: id
                            &&(data != 6 )
                            &&(data != 18)
                            &&(data != 2 ))
                        {
                            state = 0;
                        }
                        else
                        {
                            UBX_id = data;
                            checksum(data);
                            state++;
                        }
                        break;


                case 4: if( data >= UBX_BUFFER_SIZE )           // GPS: length lo
                            state = 0;
                        else
                        {
                            checksum(data);
                            length = data;
                            state++;
                        }

                        #ifdef DEBUG_MKGPS
                        // DEBUG - Anzeige der Paketgroesse
                        if( data > maxlen )
                        {
                            // groesstes gemessenes Paket lag bei 181
                            // ob man das Paket braucht ist eine andere Frage
                            maxlen = data;
                            lcdx_printf_at_P( 1, 7, MNORMAL, 0,1, PSTR("%3u"), maxlen );
                            set_beep( 25, 0xffff, BeepNormal ); // kurzer Beep
                        }
                        #endif
                        break;


                case 5: if( data != 0 )                         // GPS: length hi
                            state = 0;
                        else
                        {
                            checksum(data);
                            state++;
                        }
                        break;


                case 6: UBX_buffer[UBX_payload_counter] = data; // GPS: data
                        checksum(data);
                        UBX_payload_counter++;
                        length--;
                        if( length == 0 )
                        {
                            state++;
                        };
                        break;


                case 7: state++;                                // GPS: check lo
                        UBX_ck_a = data;
                        break;


                case 8: state = 0;                              // GPS: check hi
                        if((UBX_ck_a == ck_a) && (data == ck_b))
                        {
                            timer_mk_timeout = GPSTIMEOUT;
                            refresh = true;
                        }

            } // end: switch( state )
        } // end: if( uart_getc_nb(&data) )


        //------------------------------------------
        // Pruefe PKT-Update oder andere PKT-Aktion
        //------------------------------------------
        /*
        // funktioniert hier nicht - warum auch immer
        if( PKT_CtrlHook() )                                            // Update vom Updatetool angefordert?
        {
            redraw = true;
        }
        */


    } while( !get_key_press(1 << KEY_ESC) && timer_mk_timeout );



    //--------------------------
    // in den Timeout gelaufen?
    //--------------------------
    if( !timer_mk_timeout )
    {
      //PKT_Message_Datenverlust( timeout, beep)
        PKT_Message_Datenverlust( 500, true);       // 500 = 5 Sekunden
    }

    clear_key_all();

    SwitchToNC();
}

