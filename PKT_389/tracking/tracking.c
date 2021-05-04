/* ÄÖÜäöü Umlaute müssen korrekt sichtbar sein, ansonsten Codepageeinstellungen prüfen */

/*
 * tracking.c
 *
 *  Created on: 13.02.2012
 *      Author: cebra
 */


//############################################################################
//# HISTORY  tracking.c
//#
//# 27.06.2014 OG
//# - umgestellt auf gps/gps_nmea.c
//# - Codeformattierung
//# - chg: auf #include "../gps/mymath.h" angepasst
//# - del: #include "tools.h"
//#
//# 25.06.2014 OG
//# - chg: PKT_tracking() - auskommentierungen um Zugriffe auf Config.gps_UsedGPSMouse
//#        zu unterbinden. Anmerkung dazu siehe dort.
//#
//# 22.06.2014 OG
//# - chg: Tracking_NMEA() - do_tracking() auskommentiert (spaeter fuer
//#        Tracking wieder geradeziehen)
//# - Variable 'CheckGPS' hinzugefuegt weil aus anderen Sourcen (setup.c) entfernt
//#
//# 18.06.2014 OG
//# - chg: Code-Formattierung
//#
//# 16.06.2014 OG
//# - add: NMEA_GPGGA_counter (zaehlt empfangene GPGGA-Pakete)
//#
//# 01.06.2014 OG
//# - chg: PKT_trackingMK() - verschiedene Anzeigefunktionen auskommentiert und
//#        als DEPRICATED Klassifiziert wegen Cleanup der alten OSD Screens
//#
//# 30.05.2014 OG
//# - chg: calc_geo()- my_itoa() ersetzt durch writex_gpspos() und lcdx_printf_at_P()
//#        weil my_itoa() nicht mehr vorhanden
//#
//# 13.05.2014 OG
//# - chg: PKT_trackingBT() - Variable 'BT_WhasOn' auskommentiert
//#        wegen Warning: variable set but not used
//# - chg: PKT_trackingBT() - Variable 'flag' auskommentiert
//#        wegen Warning: variable set but not used
//#
//# 28.04.2014 OG
//# - chg: PKT_trackingBT(), PKT_trackingMK() - OSD_Timeout() entfernt (weil es
//#        das nicht mehr gibt) und erstmal durch ein PKT_Message_P() ersetzt
//#        * ACHTUNG: UNGETESTET! * (bei Bedarf anpassen, tracking hat niedrige Prio)
//# - add: include "../pkt/pkt.h"
//#
//# 12.02.2014 OG
//# - del: mk_param_struct entfernt
//#
//# 29.08.2013 Cebra
//# - chg: falsche Parameterübergabe bei getLatitude/getLongitude
//#
//# 25.08.2013 Cebra
//# - add: #ifdef USE_TRACKING . NMEA Routinen werden nur noch für BT_NMEA genutzt
//#
//# 26.06.2013 Cebra
//# - chg: Modulumschaltung für WiFlypatch geändert
//#
//############################################################################


#include "../cpu.h"
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <math.h>
#include "../main.h"


//++++++++++++++++++++++++++++++++++
#ifdef USE_TRACKING
//++++++++++++++++++++++++++++++++++


#include "../tracking/tracking.h"
#include "../tracking/ng_servo.h"
#include <avr/pgmspace.h>
#include "../bluetooth/fifo.h"
#include "../bluetooth/bluetooth.h"
#include "../lcd/lcd.h"

#include "../mk-data-structs.h"
#include "../messages.h"
#include "../lcd/lcd.h"
#include "../eeprom/eeprom.h"
#include "../timer/timer.h"
#include "../uart/uart1.h"
#include "../uart/usart.h"
#include "../osd/osd.h"
#include "../gps/mymath.h"
#include "../setup/setup.h"
#include "../pkt/pkt.h"
//#include "../gps/gps_nmea.h"
#include "../avr-nmea-gps-library/nmea.h"



uint8_t CheckGPS = true;        // Patch 22.06.2014 OG: hier integriert weil ais anseren Sourcen entfernt (u.a. setup.c)

char data_decode[RXD_BUFFER_SIZE];


#define DLEFT       0
#define DRIGHT      1
#define DEG_TO_RAD  0.0174533      // degrees to radians = PI      / 180
#define RAD_TO_DEG  57.2957795      // radians to degrees = 180 / PI
#define AltFaktor   22.5
#define TIMEOUT     200     // 2 sec

NaviData_t *naviData;


HomePos_t   MK_pos;                         // Home position of station
GPS_Pos_t   currentPos;                     // Current position of flying object


uint8_t tracking = TRACKING_MIN;
uint8_t track_hyst = TRACKING_HYSTERESE;
uint8_t track_tx =0;
uint8_t coldstart;                              // Flag erstmaliger MK-Start(Motore) nur nach GPS-Fix
geo_t   geo;
int16_t     anglePan, angleTilt;




//// Berechnung von Distanz und Winkel aus GPS-Daten home(MK eingeschaltet)
//// zur aktuellen Position(nach Motorstart)
//geo_t calc_geo(HomePos_t *home, GPS_Pos_t *pos)
//{ double lat1, lon1, lat2, lon2, d1, dlat;
//        geo_t geo;
//
//        lon1 = MK_pos.Home_Lon;
//        lat1 = MK_pos.Home_Lat;
//        lon2 = (double)pos->Longitude   / 10000000.0;
//        lat2 = (double)pos->Latitude    / 10000000.0;
//
//        // Formel verwendet von http://www.kompf.de/gps/distcalc.html
//        // 111.3 km = Abstand zweier Breitenkreise und/oder zweier Längenkreise am Äquator
//        // es wird jedoch in Meter weiter gerechnet
//        d1       = 111300 * (double)cos((double)(lat1 + lat2) / 2 * DEG_TO_RAD) * (lon1 - lon2);
//        dlat = 111300 * (double)(lat1 - lat2);
//        // returns a value in metres http://www.kompf.de/gps/distcalc.html
//        geo.bearing = fmod((RAD_TO_DEG * (double)atan2(d1, dlat)) + 180, 360); // +180 besserer Vergleich mit MkCockpit
//        if (geo.bearing > 360) geo.bearing -= 360; // bekam schon Werte über 400
//        geo.distance = sqrt(d1 * d1 + dlat * dlat);
//        return(geo);
//}

// Berechnung von Distanz und Winkel aus GPS-Daten home(MK eingeschaltet)
// zur aktuellen Position(nach Motorstart)

//--------------------------------------------------------------
// bei Gelegenheit den Code auf gps/gps.c/gps_Deviation()  aendern
//--------------------------------------------------------------
geo_t calc_geo( HomePos_t *home, GPS_Pos_t *pos )
{
    int32_t lat1, lon1, lat2, lon2;
    int32_t d1, dlat;
    geo_t geo;

    lon1 = home->Home_Lon;
    lat1 = home->Home_Lat;
    lon2 = pos->Longitude;
    lat2 = pos->Latitude;

    if( !CheckGPS )
    {
        writex_gpspos(  0, 3, home->Home_Lat , MNORMAL,  0,0);    // Anzeige: Breitengrad (Latitude)
        writex_gpspos( 11, 3, home->Home_Lon , MNORMAL,  0,0);    // Anzeige: Laengengrad (Longitude)
        writex_gpspos(  0, 4, pos->Latitude  , MNORMAL,  0,0);    // Anzeige: Breitengrad (Latitude)
        writex_gpspos( 11, 4, pos->Longitude , MNORMAL,  0,0);    // Anzeige: Laengengrad (Longitude)

        //lcd_puts_at (0, 3, my_itoa(home->Home_Lat, 10, 7, 7), 0);     // 30.05.2014 OG: my_itoa() gibt es nicht mehr
        //lcd_puts_at (11, 3, my_itoa(home->Home_Lon, 10, 7, 7), 0);    // 30.05.2014 OG: my_itoa() gibt es nicht mehr
        //lcd_puts_at (0, 4, my_itoa(pos->Latitude, 10, 7, 7), 0);      // 30.05.2014 OG: my_itoa() gibt es nicht mehr
        //lcd_puts_at (11, 4, my_itoa(pos->Longitude, 10, 7, 7), 0);    // 30.05.2014 OG: my_itoa() gibt es nicht mehr
    }

    // Formel verwendet von http://www.kompf.de/gps/distcalc.html
    // 111.3 km = Abstand zweier Breitenkreise und/oder zweier Langenkreise am Äquator
    // es wird jedoch in dm Meter weiter gerechnet
    // (tlon1 - tlon2)/10) sonst uint32_t-Überlauf bei cos(0) gleich 1
    d1       = (1359 * (int32_t)(c_cos_8192((lat1 + lat2) / 20000000)) * ((lon1 - lon2)/10))/ 10000000;
    dlat = 1113 * (lat1 - lat2) / 10000;
    geo.bearing = (my_atan2(d1, dlat) + 540) % 360; // 360 +180 besserer Vergleich mit MkCockpit
    geo.distance = sqrt32(d1 * d1 + dlat * dlat);

    if( !CheckGPS )
    {
        lcd_printp_at (0, 5, PSTR("Bear:"), 0);

        lcdx_printf_at_P( 5, 5, MNORMAL, 0,0, PSTR("%3d"), geo.bearing );
        //lcd_puts_at (5, 5, my_itoa((uint32_t)geo.bearing, 3, 0, 0), 0);       // 30.05.2014 OG: my_itoa() gibt es nicht mehr

        lcd_printp_at (8, 5, PSTR("\x1e"), 0);
        lcd_printp_at (9, 5, PSTR("Dist:"), 0);

        lcdx_printf_at_P( 15, 5, MNORMAL, 0,0, PSTR("%3d"), geo.distance );
        //lcd_puts_at (15, 5, my_itoa((uint32_t)geo.distance, 3, 1, 1), 0);     // 30.05.2014 OG: my_itoa() gibt es nicht mehr

        lcd_printp_at (20, 5, PSTR("m"), 0);
    }

    return(geo);
}





//--------------------------------------------------------------
//--------------------------------------------------------------
void do_tracking( void )
{
    static uint8_t hysteresis = 0;

    geo = calc_geo(&MK_pos, &currentPos);
    angleTilt = my_atan2((currentPos.Altitude - MK_pos.Home_Alt) / 100, geo.distance);

    if (geo.distance < 40 || (geo.distance < 60 && hysteresis))
    {                   // < 4m ==> Pan-Servo in Mittelstellung. Hysterese bis 6m, damit Servo im Grenzbereich nicht wild rumschl?gt
            geo.bearing = MK_pos.direction;
            if (currentPos.Altitude - MK_pos.Home_Alt < 4000) angleTilt = 0;        // man fliegt nicht direkt ?ber Kopf
            hysteresis = 1;
    }
    else
    {
            hysteresis = 0;
    }

    // egal wo der Übergangspunkt 359, 360, 1grd ist => Winkelübergangspunkt auf 0 bzw. 180grd des Servos bringen
    // 360 grd negative Winkelwerte als positive
    anglePan = (geo.bearing + 450 - MK_pos.direction) % 360; // 450 = 360 + 90

    if (angleTilt < 0)
        angleTilt = 0;

    if (angleTilt > 180)
        angleTilt = 180;

    if (anglePan >= 180)
    {                          // zwecks 360grd-Abdeckung flipt Pan-/Tilt-Servo
            anglePan = anglePan - 180;
            angleTilt = 180 - angleTilt;
    }

    servoSetAngle(0, anglePan);
    servoSetAngle(1, angleTilt);

    if (!CheckGPS)
    {
        lcd_printp_at (0, 6, PSTR("Pan :"), 0);
        write_ndigit_number_u (6, 6,  anglePan, 3, 1,0);
        lcd_printp_at (11, 6, PSTR("Tilt:"), 0);
        write_ndigit_number_u (17, 6,  angleTilt, 3, 1,0);
    }

    //  write_ndigit_number_u (0, 5, (uint16_t)(currentPos.Altitude/10000000), 2, 0,0);
    ////  lcd_printp_at (4, 4, PSTR("."), 0);
    //  write_ndigit_number_u (2, 5, (uint16_t)((currentPos.Altitude/1000) % 10000), 4, 1,0);
    //  write_ndigit_number_u (6, 5, (uint16_t)((currentPos.Altitude/10) % 100), 2, 1,0);
    //
    //  write_ndigit_number_u (10, 5, (uint16_t)(MK_pos.Home_Alt/10000000), 2, 0,0);
    ////  lcd_printp_at (4, 4, PSTR("."), 0);
    //  write_ndigit_number_u (12, 5, (uint16_t)((MK_pos.Home_Alt/1000) % 10000), 4, 1,0);
    //  write_ndigit_number_u (16, 5, (uint16_t)((MK_pos.Home_Alt/10) % 100), 2, 1,0);
}



//--------------------------------------------------------------
//--------------------------------------------------------------
uint8_t PKT_trackingBT( void )  // Tracking mit NMEA-Daten von BT-Maus
{
    //uint8_t BT_WhasOn = 0;
    uint8_t BT_status;
    //uint8_t flag;
    uint8_t tmp_dat;
    coldstart =1;

    //lcd_printp_at(0,1, PSTR("try NMEA data from:"), 0);
    lcd_puts_at (0, 1,Config.gps_UsedDevName, 0);
    //set_BTOn();
    set_Modul_On(Bluetooth);

    //BT_WhasOn = true;
    if( Config.BTIsSlave==true )
    {
        bt_downlink_init();
    }

    lcd_printp_at (18, 1, PSTR(" ?? "), 0);
    BT_status = bt_connect(Config.gps_UsedMac);

    if( BT_status==true )
    {
        lcd_printp_at (18, 1, PSTR(" OK "), 0);
        receiveNMEA = true;
    }
    else
    {
        lcd_printp_at (17, 1, PSTR("FAIL"), 2);
    }


    if( receiveNMEA==true )
    {
        lcd_cls_line(  0, 1, 20);
        lcd_printp_at( 0, 2, PSTR("  Latitude  Longitude"), 2);
        lcd_printp_at( 0, 3, PSTR("H"), 0);
        lcd_printp_at( 0, 4, PSTR("M"), 0);

        bt_rx_ready = 0;

        SwitchToNC();
        mode = 'O';

        // disable debug...
        //      RS232_request_mk_data (0, 'd', 0);
        tmp_dat = 0;
        SendOutData( 'd', ADDRESS_ANY, 1, &tmp_dat, 1);

        // request OSD Data from NC every 100ms
        //      RS232_request_mk_data (1, 'o', 100);
        tmp_dat = 10;
        SendOutData( 'o', ADDRESS_NC, 1, &tmp_dat, 1);

        //OSD_active = true;              // benoetigt für Navidata Ausgabe an SV2
        //flag = 0;

        timer     = TIMEOUT;
        abo_timer = ABO_TIMEOUT;

        do
        {
            //bt_rx_ready = 0;

//            if( !NMEA_receiveBT() )
//                break;
            if (!NMEA_isdataready())
                  break;

            if( rxd_buffer_locked )
            {
                timer = TIMEOUT;
                Decode64 ();
                naviData = (NaviData_t *) pRxData;

                currentPos = naviData->CurrentPosition;

                //currentPos.Altitude = MK_pos.Home_Alt + (4000 * (int32_t)(naviData->Altimeter) / AltFaktor + currentPos.Altitude - MK_pos.Home_Alt) / 5;


                //-----------------
                // MK: Lat / Long
                //-----------------
                writex_gpspos( 2, 4, currentPos.Latitude , MNORMAL, 0,0 );                 // MK: Latitude
                writex_gpspos(12, 4, currentPos.Longitude, MNORMAL, 0,0 );                 // MK: Longitude



//                NMEA_GetNewData();          // neue NMEA GPGGA Daten von der BT GPA-Maus holen



                //-----------------
                // GPS-Maus: Lat / Long
                //-----------------
                writex_gpspos( 2, 3, NMEA.Latitude , MNORMAL, 0,0 );                 // GPS-Maus: Latitude
                writex_gpspos(12, 3, NMEA.Longitude, MNORMAL, 0,0 );                 // GPS-Maus: Longitude


                //do_tracking();   // das geht so noch nicht mit einer BT GPS-Maus!  do_tracking(); ueberarbeiten!


                if( !CheckGPS )
                {
                    //lcd_printp_at( 0, 2, PSTR("GPS Time: "), 0);
                    lcd_puts_at( 13, 0, NMEA.Time , 2);

                    lcd_printp_at( 16, 1, PSTR("Sat:"), 0);
                    write_ndigit_number_u( 19, 1,  NMEA.SatsInUse, 2, 1,0);

                    lcd_printp_at( 0, 1, PSTR("Fix:"), 0);
                    write_ndigit_number_u( 4, 1, NMEA.SatFix, 1, 1,0);

                    lcd_printp_at( 6, 1, PSTR("HDOP:"), 0);
                    write_ndigit_number_u_10th( 11, 1,  NMEA.HDOP, 3, 0,0);
                }


                rxd_buffer_locked = FALSE;


                if( !abo_timer )
                {
                    // renew abo every 3 sec
                    // request OSD Data from NC every 100ms
                    //      RS232_request_mk_data (1, 'o', 100);
                    tmp_dat = 10;
                    SendOutData( 'o', ADDRESS_NC, 1, &tmp_dat, 1);

                    abo_timer = ABO_TIMEOUT;
                }

            } // end: if (rxd_buffer_locked)



            if( !timer )
            {
                //OSD_Timeout(flag);  // <- 28.04.2014 OG: gibt es nicht mehr - ersetzt durch PKT_Message_P() (ungetestet)

                //void PKT_Message_P( const char *text, uint8_t error, uint16_t timeout, uint8_t beep, uint8_t clearscreen )
                PKT_Message_P( strGet(OSD_ERROR), true, 200, true, true );   // max. 2 Sekunden anzeigen

                //flag = 0;
                error = 1;
            }

        } while( !get_key_press (1 << KEY_ESC) || !receiveNMEA==true || error ==1);


        lcd_cls_line(0,1,21);
        lcd_cls_line(0,2,21);
        lcd_cls_line(0,3,21);
        lcd_cls_line(0,4,21);
        lcd_cls_line(0,5,21);
        lcd_cls_line(0,6,21);

        if( !receiveNMEA )
            lcd_printp_at (0, 2, PSTR("lost BT data"), 0);


          lcd_printp_at (0, 3, PSTR("GPS trennen"), 0);

    } // end: if( receiveNMEA==true )
    else
    {
        lcd_printp_at (0, 4, PSTR("Error at connecting"), 0);
        lcd_printp_at (0, 5, PSTR("switch on BT Mouse!!"), 0);
        while( !get_key_press (1 << KEY_ENTER) );
    }

    receiveNMEA = false;

    if( !bt_disconnect() )
        lcd_printp_at (0, 3, PSTR("Fehler beim Trennen"), 0);

    //set_BTOff();
    set_Modul_On(USB);
    return true;
}



//--------------------------------------------------------------
//--------------------------------------------------------------
uint8_t PKT_trackingMK( void )  // Tracking mit GPS-Daten vom Mikrokopter
{
    //uint8_t BT_WhasOn = 0;
    //uint8_t BT_status;
    uint8_t GPSfix=0;
    uint8_t tmp_dat;
    uint8_t toggletimer=0;
    coldstart = true;


    lcd_printp_at (0, 2, PSTR("S Latitude  Longitude"), 2);

    lcd_cls_line (0,1,20);

    //lcd_printp_at (0, 3, PSTR("H"), 0);
    //lcd_printp_at (0, 4, PSTR("M"), 0);

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
    timer = TIMEOUT;
    abo_timer = ABO_TIMEOUT;
    error = 0;

    do
    {
        if( rxd_buffer_locked )
        {
            timer = TIMEOUT;
            Decode64 ();
            naviData = (NaviData_t *) pRxData;
            //OSD_Screen_Element (18, 1, OSD_SATS_IN_USE,1);
            //if (GPSfix == true)  OSD_Screen_Element (0, 1, OSD_STATUS_FLAGS,1);

            //---
            // 01.06.2014 OG: DEPRICATED  (alte Funktionen in osd/osdold_screens.c - nicht mehr verwenden!)
            //---
            //OSD_Element_SatsInUse( 18, 1, 1);

            //---
            // 01.06.2014 OG: DEPRICATED  (alte Funktionen in osd/osdold_screens.c - nicht mehr verwenden!)
            //---
            //if (GPSfix == true)  OSD_Element_StatusFlags( 0, 1);

            if (!(naviData->NCFlags & NC_FLAG_GPS_OK))
            {
                toggletimer++;
                if (toggletimer == 50) toggletimer = 0;
                if (toggletimer == 25) lcd_printp_at(0,1, PSTR("Whait for GPS Fix "), 2);
                if (toggletimer == 1) lcd_printp_at(0,1, PSTR("Whait for GPS Fix "), 0);

                rxd_buffer_locked = false;
                GPSfix = false;
            }
            else GPSfix = true;


            if( GPSfix )
            {
                if( coldstart )
                {
                    // erst nach Neustart NGVideo und beim Motorstart werden Daten vom MK übernommen
                    if (naviData->FCStatusFlags & FC_FLAG_MOTOR_START)
                    {
                        MK_pos.Home_Lon = naviData->HomePosition.Longitude;
                        MK_pos.Home_Lat = naviData->HomePosition.Latitude;
                        MK_pos.Home_Alt = naviData->HomePosition.Altitude;
                        MK_pos.direction = naviData->CompassHeading;
                        coldstart = false;
                        rxd_buffer_locked = false;
                        lcd_printp_at(0,1, PSTR("                  "), 0);
                    }
                    else
                    {
                        lcd_printp_at(0,1, PSTR("GPS ok, start ok  "), 0);
                        rxd_buffer_locked = false;
                    }
                } // end: if( coldstart )
                else
                {
                    //run
                    currentPos = naviData->CurrentPosition;
                    currentPos.Altitude = MK_pos.Home_Alt + (4000 * (int32_t)(naviData->Altimeter) / AltFaktor + currentPos.Altitude - MK_pos.Home_Alt) / 5;
                    do_tracking();

                    //lcd_puts_at (13, 0, NMEAtime, 2);
                    //lcd_printp_at (16, 1, PSTR("Sat:"), 0);
                    //write_ndigit_number_u (19, 1,  NMEAsatsInUse, 2, 1,0);
                    //lcd_printp_at (0, 1, PSTR("Fix:"), 0);
                    //write_ndigit_number_u (4, 1,  posfix, 1, 1,0);
                    //lcd_printp_at (6, 1, PSTR("HDOP:"), 0);
                    //write_ndigit_number_u_10th (11, 1,  HDOP, 3, 0,0);

                    rxd_buffer_locked = FALSE;
                } // run
            }


            if( !abo_timer )
            {
                // renew abo every 3 sec
                // request OSD Data from NC every 100ms
                //      RS232_request_mk_data (1, 'o', 100);
                tmp_dat = 10;
                SendOutData( 'o', ADDRESS_NC, 1, &tmp_dat, 1);

                abo_timer = ABO_TIMEOUT;
            }

        } // end: if( rxd_buffer_locked )


        if( !timer )
        {
            //OSD_Timeout(1);  // <- 28.04.2014 OG: gibt es nicht mehr - ersetzt durch PKT_Message_P() (ungetestet)

            //void PKT_Message_P( const char *text, uint8_t error, uint16_t timeout, uint8_t beep, uint8_t clearscreen )
            PKT_Message_P( strGet(OSD_ERROR), true, 200, true, true );   // max. 2 Sekunden anzeigen

            error = 1;
        }

    } while( (!get_key_press (1 << KEY_ENTER)) && (error ==0) );


    lcd_cls_line(0,1,21);
    lcd_cls_line(0,2,21);
    lcd_cls_line(0,3,21);
    lcd_cls_line(0,4,21);
    lcd_cls_line(0,5,21);
    lcd_cls_line(0,6,21);

    if (error ==1)
    {
        lcd_printp_at (0, 2, PSTR("lost Wi.232 data"), 0);
        _delay_ms(2000);
    }

    return true;
}



//--------------------------------------------------------------
//--------------------------------------------------------------
void PKT_tracking(void)
{
    clear_key_all();
    lcd_cls ();

    // 25.06.2014 OG: auskommentiert weil erstmal kein Config.gps_UsedGPSMouse mehr unterstuetzt
    // wird. Falls tracking.c mal richtig implementiert wird sollte man sich auf eine BT-GPS-Maus
    // konzentrieren bis Tracking einwandfrei laeuft - wenn dann noch immer Bedarf nach MK-GPS ist
    // kann man das ja wieder einbauen
    //
    //if (Config.gps_UsedGPSMouse==GPS_Bluetoothmouse1)  lcd_printp_at(0,0, PSTR("Tracking Bluetooth   "), 2);
    //if (Config.gps_UsedGPSMouse==GPS_Mikrokopter)      lcd_printp_at(0,0, PSTR(" Tracking Mikrokopter"), 2);
    //if (Config.gps_UsedGPSMouse==GPS_Bluetoothmouse1) PKT_trackingBT();
    //if (Config.gps_UsedGPSMouse==GPS_Mikrokopter) PKT_trackingMK();

    lcd_printp_at( 0,0, PSTR("Tracking Bluetooth   "), MINVERS);
    lcd_printp_at(12, 7, strGet(ENDE), MNORMAL);                    // Keyline
    PKT_trackingBT();
    clear_key_all();
}




//++++++++++++++++++++++++++++++++++
#endif // #ifdef USE_TRACKING
//++++++++++++++++++++++++++++++++++

