/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*
 * FollowMe.c
 *
 *  Created on: 18.05.2012
 *      Author: cebra
 */
/*****************************************************************************
 *   Copyright (C) 2011 Christian "Cebra" Brandtner, brandtner@brandtner.net *
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
//# HISTORY  followme.c
//#
//# 4.1.2017 Cebra
//# - Umlaute korrigiert
//#
//# 14.10.2015 Starter
//# - Added Offset to FollowMe
//# - Stuff in FollowMeStep2 only for debug!
//#
//# 22.09.2015 Starter
//# - FollowMeStep2 erweitert mit Kreisberechnung und test auf PKT
//# - PKT-Pos von lat lon auf latitude und longitude umbenannt
//#
//# 20.09.2015 Starter
//# FollowMeStep2 erweitert auf aktuelle GPS-Daten und followme_calculate_offset(...)
//#
//# 03.08.2015 Cebra
//# - add: void Debug_GPS (void) hinzugefÃ¼gt zur Test der GPS Berechnung FollowMe
//#
//# 20.07.2015 CB
//# - chg: FollowMe Datensatz an NC.211 angepasst
//#
//# 27.06.2014 OG
//# - chg: Anzeige von Satfix und Satanzahl auf MINVERSX, MNORMALX
//#
//# 26.06.2014 OG
//# - chg: angepasst auf neue NMEA-Datenstruktur (gps_nmea.h)
//#
//# 24.06.2014 OG
//# - chg: FollowMe() angepasst auf geaendertes GPSMouse_ShowData()
//#
//# 22.06.2014 OG
//# - chg: FollowMe() umgestellt auf GPSMouse_ShowData() (in gps/gpsmouse.c)
//# - del: Variable CheckGPS
//#
//# 21.06.2014 OG
//# - chg: verschiedene Layoutaenderungen am Anzeigescreen und Anzeige der
//#        Entfernung zwischen Kopter und Target
//# - chg: MK-Timeoutout Erkennung verbessert/angepasst
//#
//# 19.06.2014 OG
//# - erster Prototyp der Follow Me zum Kopter sendet
//# - etliche Aenderungen im Codeaufbau
//#
//# 01.06.2014 OG
//# - chg: FollowMe() - verschiedene Anzeigefunktionen auskommentiert und
//#        als DEPRICATED Klassifiziert wegen Cleanup der alten OSD Screens
//# - chg: FollowMe() - Check bzgl. NC-Hardware entfernt da das bereits durch das
//#        Hauptmenue erledigt wird
//#
//# 13.05.2014 OG
//# - chg: FollowMe() - Variable 'flag' auskommentiert
//#        wegen Warning: variable set but not used
//# - chg: FollowMe() - Variable 'old_FCFlags' auskommentiert
//#        wegen Warning: variable set but not used
//# - chg: FollowMe() - den Bereich in dem FC-Settings geladen werdeb
//#        auskommentiert weil man das a) vorallem nicht benoetigt
//#        und b) die load_setting() so nicht mehr existiert
//#        (der Codebereich kann meines erachtens geloescht werden)
//# - del: verschiedene Verweise auf 'mk_param_struct' entfernt, weil es
//#        das a) nicht mehr gibt und b) hier gar nicht benoetigt wird
//# - chg: FollowMe() - OSD_Timeout() entfernt (weil es das nicht mehr gibt)
//#        und erstmal durch ein PKT_Message_P() ersetzt
//#        * ACHTUNG: UNGETESTET! * (bei Bedarf anpassen, followme hat niedrige Prio)
//# - add: #include "../pkt/pkt.h"
//#
//# 05.05.2013 Cebra
//# - chg: #ifdef USE_FOLLOWME
//#
//############################################################################

#include "../cpu.h"
#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
//#include <stdbool.h>

#include "../main.h"

#ifdef USE_FOLLOWME

#include "../followme/followme.h"
#include "../osd/osd.h"
#include "../lcd/lcd.h"
#include "../timer/timer.h"
#include "../uart/usart.h"
#include "../eeprom/eeprom.h"
#include "../messages.h"
#include "../bluetooth/bluetooth.h"
#include "../setup/setup.h"
#include "../uart/uart1.h"
#include "../mk-data-structs.h"
#include "../pkt/pkt.h"
#include "../gps/gps.h"
//#include "../gps/gps_nmea.h"
#include "../avr-nmea-gps-library/nmea.h"
#include "../gps/gpsmouse.h"


//#######################################################################################################################


//--------------------
// Timings
//--------------------
//#define MK_TIMEOUT          300 // MK-Verbindungsfehler wenn fuer n Zeit keine gueltigen Daten hereinkommen (3 sec)
#define MK_TIMEOUT            400 // MK-Verbindungsfehler wenn fuer n Zeit keine gueltigen Daten hereinkommen (4 sec)



//--------------------
#define COSD_WASFLYING 4

// global definitions and global vars
NaviData_t *naviData;
unsigned char Element;
uint16_t heading_home;

// Hier HÃ¶henanzeigefehler Korrigieren
#define AltimeterAdjust 1.5


// Flags
//uint8_t COSD_FLAGS2 = 0;
//
//GPS_Pos_t last5pos[7];
uint8_t FM_error = 0;


//---------------------
// Waypoint Types
// TODO OG: verschieben nach: mk-data-structs.h
//---------------------
#define POINT_TYPE_INVALID  255
#define POINT_TYPE_WP       0
#define POINT_TYPE_POI      1


//---------------------
// Status
// GPS_Pos_t
// aus MK source
//
// TODO OG: verschieben nach: mk-data-structs.h
//---------------------
#define INVALID         0x00
#define NEWDATA         0x01
#define PROCESSED       0x02

#define MK_FREE 0x01	// TODO change to enum
#define MK_PH   0x02
#define MK_CH   0x03
//--------------------------------------------------------------
//--------------------------------------------------------------
void FollowMe( void )
{
    //uint8_t status;
    GPS_Pos_t currpos;
    uint8_t tmp_dat;
    uint8_t redraw;
    uint32_t NMEA_GPGGA_counter_old;                                   // Merker: zaehlt empfangene GPGGA-Pakete
    uint32_t send_followme_counter;

    int8_t ok;
    Point_t FollowMe;
    uint8_t mktimeout = false;
    uint8_t modus = 0;

	nmeaPOS NMEApos;
	nmeaPOS NMEAtarget;

    GPS_PosDev_t targetdev;

    positionOffset followMeOffset;
    followMeOffset.latitude = 0;
    followMeOffset.longitude = 0;
    followMeOffset.type = 1;

    nmeaPOS copterPosition;

    static uint8_t FmState = 0;		// 0: Kopterposition nicht erfasst
									// 1: Kopterposition erfasst
									// 2: Offset gespeichert und FollowMe aktiv


    //---------------------
    // 1. Daten GPS-Maus
    //---------------------
    ok = GPSMouse_ShowData( GPSMOUSE_SHOW_WAITSATFIX, 500 );            // 500 = 5 Sekunden Verzoegerung nach Satfix
    if( ok <= 0 )
    {
        return;                                                         // Fehler bzgl. BT GPS-Maus -> exit
    }


    //---------------------
    // 2. Follow Me
    //---------------------
    set_beep( 25, 0xffff, BeepNormal );                                 // kurzer Bestaetigungs-Beep

    lcd_cls ();

    SwitchToNC();

    mode = 'O';

    // disable debug...
    //      RS232_request_mk_data (0, 'd', 0);
    tmp_dat = 0;
    SendOutData ('d', ADDRESS_ANY, 1, &tmp_dat, 1);

    // request OSD Data from NC every 100ms
    //      RS232_request_mk_data (1, 'o', 100);
    tmp_dat = 10;
    SendOutData ('o', ADDRESS_NC, 1, &tmp_dat, 1);

    //OSD_active = true;              // benÃ¶tigt fÃ¼r Navidata Ausgabe an SV2


    // Init: Timer & Flags
    timer_mk_timeout      = MK_TIMEOUT;

    abo_timer = ABO_TIMEOUT;


    MKLiPoCells_Init();

    redraw = true;

    NMEA.Counter           = 0;
    NMEA_GPGGA_counter_old = 0;
    send_followme_counter  = 0;


    while( (receiveNMEA) )
    {
					//-----------------------------------------
					// Screen redraw
					//-----------------------------------------

					// -----> X
					//  ________________________________________________________
					// |               P0                       P2              |
					// |               |        FollowMe        |               |
					// | P4------------P1-----------------------P3--------P5    |
					// |                                                        |
					// |                                                        |
					// |                                                        |
					// | P6------Q1_l--------                   P7---Q2_l---    |
					// |Q1_b                 |P8--------------P9|         Q2_b  |
					// | --------------------                    -----------    |
					// |                                                        |
					// |                                                        |
					// |________________________________________________________|
#define x_min 0
#define x_max 127
#define y_min 0
//#define y_max

#define P0_x 33
#define P0_y y_min
#define P1_x P0_x
#define P1_y 11

#define P2_x 95
#define P2_y y_min
#define P3_x P2_x
#define P3_y P1_y

#define P4_x x_min
#define P4_y P1_y
#define P5_x x_max
#define P5_y P1_y

#define P6_x x_min
#define P6_y 33
#define Q1_l 66
#define Q1_b 12

#define P7_x 95
#define P7_y P6_y
#define Q2_l (x_max-P7_x)
#define Q2_b Q1_b

#define P8_x (P6_x+Q1_l)
#define P8_y (P6_y+Q1_b/2)
#define P9_x (x_max-Q2_l)
#define P9_y P8_y

        if( redraw )
        {
            lcd_cls();

            lcdx_printf_center_P( 0, MNORMAL, 1,0, PSTR("FollowMe") );  // Titel: oben, mitte

            lcd_line( P0_x, P0_y, P1_x, P1_y, 1 );                      // Linie Vertikal links
            lcd_line( P2_x, P2_y, P3_x, P3_y, 1 );                      // Linie Vertikal rechts
            lcd_line( P4_x, P4_y, P5_x, P5_y, 1 );                      // Linie Horizontal

            lcd_line( P8_x, P8_y, P9_x, P9_y, 1);                       // Linie Horizontal Mitte
            lcd_rect_round( P6_x, P6_y, Q1_l, Q1_b, 1, R1);             // Rahmen fuer "Di" (Distance)
            lcd_rect_round( P7_x, P7_y, Q2_l, Q2_b, 1, R1);             // Rahmen fuer Modus

            lcdx_printf_at_P( 3, 2, MNORMAL, 3,-1, PSTR("Al:")  );      // Label: "Al:"

            draw_icon_mk( 1, 18);
            draw_icon_target_round( 1, 50);

            redraw = false;
        }

        // neue MK Daten vorhanden
        if( rxd_buffer_locked )
        {
            Decode64 ();
            naviData = (NaviData_t *) pRxData;

            if( mktimeout ) redraw = true;
            mktimeout = false;

            FM_error = 0;   // noch benoetigt?

            currpos.Latitude  = naviData->CurrentPosition.Latitude;
            currpos.Longitude = naviData->CurrentPosition.Longitude;

            // MK: letzte GPS-Positione speichern
            Config.LastLatitude  = naviData->CurrentPosition.Latitude;
            Config.LastLongitude = naviData->CurrentPosition.Longitude;

            // MK: LiPo Cell Check
            MKLiPoCells_Check();                    // ggf. Zellenzahl ermitteln

            // MK: Batt Level (Volt)
            OSD_Element_BattLevel2( 0, 0, 0,0 );

            // MK: Batt Level (Bar)
            OSD_Element_Battery_Bar( 0, 9, 30, 1, ORIENTATION_H);

            // MK: Flugzeit
            writex_time( 16, 0, naviData->FlyingTime, MNORMAL, 2, 0 );

            // MK: Hoehe
            writex_altimeter( 7, 2, naviData->Altimeter, MNORMAL, 3, -1 );

            // MK: Sat Anzahl
            if( naviData->SatsInUse > 5 )
            	writex_ndigit_number_u( 17, 5, naviData->SatsInUse, 2, 0, MNORMALX, 3, -25);
            else
            	writex_ndigit_number_u( 17, 5, naviData->SatsInUse, 2, 0, MINVERSX, 3, -25);

            // sat-logo
            draw_icon_satmini( 119, 15);

            // MK: GPS
            writex_gpspos( 3, 4, currpos.Latitude , MNORMAL, -3, -8 );
            writex_gpspos(12, 4, currpos.Longitude, MNORMAL,  1, -8 );

            // Target: distance
            targetdev = gps_Deviation( FollowMe.Position, naviData->CurrentPosition);
            lcdx_printf_at_P( 0, 4, MNORMAL, 7, 4 , PSTR("Di: %3d m"), (targetdev.Distance / 10) );

            // Target: counter
            lcdx_printf_at_P( 4, 5, MNORMAL, -3, 8, PSTR("Tx:%5ld"), send_followme_counter );

            // Target: fix
            if( NMEA.SatFix == 1 || NMEA.SatFix == 2 )
            	lcdx_printf_at_P( 14, 5, MNORMALX, 1, 8, PSTR("F:%1d"), NMEA.SatFix );
            else
            	lcdx_printf_at_P( 14, 5, MINVERSX, 1, 8, PSTR("F:%1d"), NMEA.SatFix );

            // Target: Sat Anzahl
            if( NMEA.SatsInUse > 5 )
            	writex_ndigit_number_u( 17, 5, NMEA.SatsInUse, 2, 0, MNORMALX, 4, 8);
            else
            	writex_ndigit_number_u( 17, 5, NMEA.SatsInUse, 2, 0, MINVERSX, 4, 8);

            draw_icon_satmini( 120, 48);

            // Target: latitude / longitude
            writex_gpspos( 3, 7, NMEA.Latitude , MNORMAL, -3, 1 );
            writex_gpspos(12, 7, NMEA.Longitude, MNORMAL,  1, 1 );

            rxd_buffer_locked = FALSE;

            timer_mk_timeout = MK_TIMEOUT;

            // detect and display NC-mode (Free/PH/CH)
            if( naviData->NCFlags & NC_FLAG_FREE) {
            	modus = MK_FREE;
            	lcdx_printf_at_P( 16, 4, MNORMAL, 4, 4, PSTR("Free") );
            } else {
            	if( naviData->NCFlags & NC_FLAG_PH) {
            		modus = MK_PH;
            		lcdx_printf_at_P( 16, 4, MNORMAL, 4, 4, PSTR(" PH ") );
            	} else {
            		if( naviData->NCFlags & NC_FLAG_CH) {
            			modus = MK_CH;
            			lcdx_printf_at_P( 16, 4, MNORMAL, 4, 4, PSTR(" CH ") );
            		} else {
            			modus  = 0;
            		}
            	}
            }
        }


        // neue NMEA Daten?
        if(NMEA_isdataready() && receiveNMEA)
        {
            if( NMEA.Counter > NMEA_GPGGA_counter_old )
            {
                if( (NMEA.SatsInUse > 5) && (NMEA.SatFix == 1 || NMEA.SatFix == 2) )
                {

                	// FM Step3, Offset kann mit Kopter in PH eingestellt werden
                	switch(modus)
                	{
                	case MK_FREE:
                		FmState = 0;
                		break;

                	case MK_PH:
                		copterPosition.latitude = currpos.Latitude;
                		copterPosition.longitude = currpos.Longitude;
                		FmState = 1;
                		break;

                	case MK_CH:
                		if( FmState == 1) {		// Nur das erste mal den Offset berechnen
                			followMeOffset.type = 1;
                			followMeOffset.latitude = copterPosition.latitude - NMEA.Latitude;
                			followMeOffset.longitude = copterPosition.longitude - NMEA.Longitude;		// TODO: limit Offset
                			FmState = 2;
                		}
                		break;

                	default:
                		break;
                	}

                	//if(modus == MK_PH) {
                	//	copterPosition.latitude = currpos.Latitude;
                	//	copterPosition.longitude = currpos.Longitude;
                	//	FmState = 1;
                	//} else {
                	//	if( modus == MK_CH && FmState == 1) {		// Nur das erste mal den Offset berechnen
                	//		followMeOffset.type = 1;
                	//		followMeOffset.latitude = copterPosition.latitude - NMEA.Latitude;
                	//		followMeOffset.longitude = copterPosition.longitude - NMEA.Longitude;		// TODO: limit Offset
                	//		FmState = 2;
                	//	}
                		//else
                		//{
                		//	FmState = 0;
                		//
                		//}
                	//}


                	// FollowMeStep2 Offset wird nun aus Kopterposition berechnet
                	//followMeOffset.latitude = Config.FM_Offest_Latitude;
                	//followMeOffset.longitude = Config.FM_Offset_Longitude;

                	NMEApos.latitude = NMEA.Latitude;
                	NMEApos.longitude = NMEA.Longitude;

                	followme_add_offset(&NMEApos, &NMEAtarget, &followMeOffset);

                    FollowMe.Position.Status    = NEWDATA;
                    FollowMe.Position.Longitude = NMEAtarget.longitude;
                    FollowMe.Position.Latitude  = NMEAtarget.latitude;
                    FollowMe.Position.Altitude  = 1;                    // 20.7.2015 CB
//                  FollowMe.Position.Altitude  = NMEA.Altitude;        // ist das wirklich ok? NEIN C.B.

                    FollowMe.Heading = -1;                              // invalid heading
                    FollowMe.ToleranceRadius = Config.FM_Radius;        // 5 meter default
                    FollowMe.HoldTime = 60;                             // ????? go home after 60s without any update ??????
//                  FollowMe.Event_Flag = 0;                            // no event
                    FollowMe.Event_Flag = 1;                            //  20.7.2015 CB
                    FollowMe.Index = 1;                                 // 2st wp, 0 = Delete List, 1 place at first entry in the list
                    FollowMe.Type = POINT_TYPE_WP;                      // Typ des Wegpunktes
                    FollowMe.Name[0] = 'F';                             // Name des Wegpunktes (ASCII)
                    FollowMe.Name[1] = 'O';
                    FollowMe.Name[2] = 'L';
                    FollowMe.Name[3] = 'L';
//                  FollowMe.WP_EventChannelValue = 0;                  // Will be transferred to the FC and can be used as Poti value there
                    FollowMe.WP_EventChannelValue = 100;                // set servo value 20.7.2015
                    FollowMe.AltitudeRate = 0;                          // rate to change the Aetpoint
                    FollowMe.Speed = Config.FM_Speed;                   // rate to change the Position
                    FollowMe.CamAngle = 255;                            // Camera servo angle in degree (255 -> POI-Automatic)
                    FollowMe.reserve[0] = 0;                            // reserve
                    FollowMe.reserve[1] = 0;                            // reserve

                    if(FmState == 2) {									// send only waypoints when offset is set and copter mode is CH
                    	SendOutData( 's', ADDRESS_NC, 1, &FollowMe, sizeof(FollowMe) );       //'s' = target Position  'w' = Waypoint
                    	send_followme_counter++;
                    }
                }

                NMEA_GPGGA_counter_old = NMEA.Counter;
            }
        }

        // TASTEN
        if( get_key_press(1 << KEY_ESC) )
        {
            break;
        }

        if( get_key_press(1 << KEY_ENTER) )
        {
            break;
        }

        if( !abo_timer )
        {
            // renew abo every 3 sec
            // request OSD Data from NC every 100ms
            //      RS232_request_mk_data (1, 'o', 100);
            tmp_dat = 10;
            SendOutData ( 'o', ADDRESS_NC, 1, &tmp_dat, 1);

            abo_timer = ABO_TIMEOUT;
        }

        // Daten Timeout vom MK?
        if( timer_mk_timeout == 0 )
        {
            if( !mktimeout ) OSD_MK_ShowTimeout();              // nur anzeigen wenn noch nicht im mktimeout-Modus
            set_beep ( 200, 0x0080, BeepNormal);                // Beep
            mktimeout        = true;
            timer_mk_timeout = MK_TIMEOUT;
            //OSDScreenRefresh = OSD_SCREEN_REDRAW;
            // OSD_MK_Connect( MK_CONNECT );
        }

    } // end: while( (receiveNMEA) );



    //---------------------
    // BEENDEN
    //---------------------
    OSD_active = false;


    //---------------------
    // GPS beenden
    //---------------------
    GPSMouse_Disconnect();
}





//
#ifdef USE_FOLLOWME_STEP2
void Debug_GPS (void)
{
	uint8_t redraw = true;
	nmeaPOS NMEApos;
	nmeaPOS NMEATarget;

	set_beep( 25, 0xffff, BeepNormal );                                 // kurzer Bestaetigungs-Beep


#ifdef ONLINE
    int retcode = GPSMouse_Connect();                                   // Abfrage der GPS-Daten zum testen -> Quick an Dirty ;-)
    if( retcode <= 0 )
    {
        return;
    }
#endif

#ifdef DEBUG
//	NMEApos.lat = 520000000;
//	NMEApos.lon = 0;
//	Config.FM_Azimuth = 90;
//	Config.FM_Distance = 10000;
#endif

	while( true )
	{
		NMEApos.latitude = NMEA.Latitude;
		NMEApos.longitude = NMEA.Longitude;

		if( redraw )
		{
			lcd_cls();

			lcdx_printf_center_P( 0, MNORMAL, 1,0, PSTR("FollowMeStep2") );
			lcdx_printf_center_P( 1, MNORMAL, 1,0, PSTR(" Source Lat/Lon") );
			lcdx_printf_center_P( 3, MNORMAL, 1,0, PSTR(" Target Lat/Lon") );

			redraw = false;
		}

		writex_gpspos( 1, 2, NMEApos.latitude, MNORMAL, 0,0 );                 // GPS-Maus: Latitude
		writex_gpspos(10, 2, NMEApos.longitude, MNORMAL, 0,0 );                // GPS-Maus: Longitude

		followme_calculate_offset(Config.FM_Offset_Longitude, Config.FM_Offest_Latitude, &followMeOffset);

#ifdef DEBUG
		writex_gpspos(  1, 6, (int32_t)Config.FM_Offest_Latitude*100, MNORMAL, 0, 0 );
		writex_gpspos( 10, 6, (int32_t)Config.FM_Offset_Longitude*100, MNORMAL, 0, 0 );
		writex_gpspos(  1, 7, (int32_t)followMeOffset.latitude*100, MNORMAL, 0, 0 );
		writex_gpspos( 10, 7, (int32_t)followMeOffset.longitude*100, MNORMAL, 0, 0 );
#endif

		followme_add_offset(&NMEApos, &NMEATarget, &followMeOffset);

		writex_gpspos( 1, 4, (int32_t)NMEATarget.latitude, MNORMAL, 0, 0 );        // Ziel Latitude
		writex_gpspos(10, 4, (int32_t)NMEATarget.longitude, MNORMAL, 0, 0 );       // Ziel Longitude




		// Tasten
		if( get_key_press(1 << KEY_ESC) )
		{
#ifdef ONLINE
			GPSMouse_Disconnect();
#endif
			break;
		}

		if( get_key_press(1 << KEY_ENTER) )
		{
			redraw = true;
		}

		if( get_key_press(1 << KEY_MINUS) )
		{
			Config.FM_Offest_Latitude -= 10;
			redraw = true;
		}
		if( get_key_press(1 << KEY_PLUS) )
		{
			Config.FM_Offest_Latitude += 10;
			redraw = true;
		}
	}
}



#endif	// FOLLOW_ME_STEP2
#endif  // #ifdef USE_FOLLOWME


